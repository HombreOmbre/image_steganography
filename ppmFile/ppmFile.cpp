#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "./ppmFile.hpp"
#include "../pixel/Pixel.hpp"

auto PPM_File::getFormat() const -> std::string {
    return format;
}

auto PPM_File::getWidth() const -> int {
    return width;
}

auto PPM_File::getHeight() const -> int {
    return height;
}

auto PPM_File::getMaxColorVal() const -> int {
    return maxColorVal;
}

auto PPM_File::getPixels() const -> std::vector<Pixel> {
    return pixels;
}

auto PPM_File::getFlag() const -> bool {
    return flag;
}


auto PPM_File::readPixelsFromFormatP6(std::fstream& file) -> void {
    auto tmpVec = std::vector<unsigned char>(width * height * 3);
    file.read(reinterpret_cast<char*>(tmpVec.data()), tmpVec.size());

    for (auto i = 0; i < tmpVec.size(); i += 3) {
        pixels.push_back(Pixel(
            tmpVec[i],
            tmpVec[i + 1],
            tmpVec[i + 2]));
    }
}

auto PPM_File::readPixelsFromFormatP3(std::fstream& file) -> void {
    auto red = 0;
    auto green = 0;
    auto blue = 0;

    while (file >> red >> green >> blue) {
        pixels.push_back(Pixel(red, green, blue));
    }
}

auto PPM_File::convertStringToBinary(std::string const& str) -> std::string {
    auto binaryStr = std::string();

    for (auto sign : str) {
        binaryStr += std::bitset<8>(sign).to_string();
    }

    binaryStr += "00000000";

    return binaryStr;
}

auto PPM_File::checkIfImageContainsMessage() -> bool {
    return (pixels[0].getRed() & 0b11111000) == 0b10100000;
}

auto PPM_File::checkIfMsgFitsIntoImage(int msgSize) -> bool {
    return msgSize > (width * height - 1) * 3;
}

auto PPM_File::insertMsgIntoFile(std::string const& msg) -> bool {
    if (checkIfImageContainsMessage()) {
        std::cout << "The image already contains a hidden message.\n";
        return false;
    }

    auto binMsg = convertStringToBinary(msg);

    if (checkIfMsgFitsIntoImage(binMsg.size())) {
        std::cout << "Too long message.\n";
        return false;
    }

    pixels[0].setRed((pixels[0].getRed() & ~0b11111000) | 0b10100000);

    auto indexOfPixel = 1;

    for (auto sign : binMsg) {
        if (indexOfPixel % 2 == 0) {
            if (sign == '0') {
                pixels[indexOfPixel].setGreen(pixels[indexOfPixel].getGreen() & ~1);
            } else {
                pixels[indexOfPixel].setGreen(pixels[indexOfPixel].getGreen() | 1);
            }
        } else if (indexOfPixel % 3 == 0) {
            if (sign == '0') {
                pixels[indexOfPixel].setBlue(pixels[indexOfPixel].getBlue() & ~1);
            } else {
                pixels[indexOfPixel].setBlue(pixels[indexOfPixel].getBlue() | 1);
            }
        } else {
            if (sign == '0') {
                pixels[indexOfPixel].setRed(pixels[indexOfPixel].getRed() & ~1);
            } else {
                pixels[indexOfPixel].setRed(pixels[indexOfPixel].getRed() | 1);
            }
        }
        indexOfPixel++;
    }

    return true;
}

auto PPM_File::readMsgFromFile() -> std::string {
    if (!checkIfImageContainsMessage()) {
        std::cout << "Image doesn't contain any hidden message.\n";
        return "";
    }

    auto startIndex = 1;
    auto hiddenMsgBin = std::string();
    auto isTrue = true;

    while (isTrue) {
        if (startIndex >= pixels.size()) {
            isTrue = false;
            std::cout << "Something gone wrong. File can be damaged.\n";
            return "";
        }

        if (startIndex % 2 == 0) {
            auto hiddenBit = pixels[startIndex].getGreen();
            hiddenMsgBin += (hiddenBit & 1) == 0 ? '0' : '1';
        } else if (startIndex % 3 == 0) {
            auto hiddenBit = pixels[startIndex].getBlue();
            hiddenMsgBin += (hiddenBit & 1) == 0 ? '0' : '1';
        } else {
            auto hiddenBit = pixels[startIndex].getRed();
            hiddenMsgBin += (hiddenBit & 1) == 0 ? '0' : '1';
        }

        if (hiddenMsgBin.size() % 8 == 0) {
            if (hiddenMsgBin.substr(hiddenMsgBin.size() - 8) == "00000000") {
                isTrue = false;
            }
        }
        startIndex++;
    }

    hiddenMsgBin = hiddenMsgBin.substr(0, hiddenMsgBin.size() - 8);

    auto hiddenMsgStr = std::string();
    for (auto i = 0; i < hiddenMsgBin.size(); i += 8) {
        auto sign = hiddenMsgBin.substr(i, 8);
        hiddenMsgStr += static_cast<char>(std::bitset<8>(sign).to_ulong());
    }

    return  hiddenMsgStr;
}

auto PPM_File::savePixelsInP6File(std::fstream& file) -> void {
    auto tmpPixelsData = std::vector<unsigned char>(width * height * 3);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tmpPixelsData[i * width * 3 + j * 3] = pixels[i * width + j].getRed();
            tmpPixelsData[i * width * 3 + j * 3 + 1] = pixels[i * width + j].getGreen();
            tmpPixelsData[i * width * 3 + j * 3 + 2] = pixels[i * width + j].getBlue();
        }
    }
    file.write(reinterpret_cast<char*>(tmpPixelsData.data()), tmpPixelsData.size());
}

auto PPM_File::savePixelsInP3File(std::fstream& file) -> void {
    auto widthCounter = 0;
    auto red = 0;
    auto green = 0;
    auto blue = 0;

    for (const auto& pix : pixels) {
        red = pix.getRed();
        green = pix.getGreen();
        blue = pix.getBlue();

        file << red << " "
             << green << " "
             << blue;

        ++widthCounter;
        if (widthCounter % width == 0) {
            file << "\n";
        } else {
            file << " ";
        }
    }
}

auto PPM_File::saveFileWithMsg(std::fstream& file) -> void {
    file.clear();
    file.seekp(0, std::ios::beg);

    file << format << "\n"
         << width << " "
         << height << "\n"
         << maxColorVal << "\n";

    if (format == "P6") {
        savePixelsInP6File(file);
    }
    if (format == "P3") {
        savePixelsInP3File(file);
    }
}

