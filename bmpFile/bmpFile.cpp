#include <iostream>

#include "./bmpFile.hpp"

// Getters
auto BMPFile::getHeader() const -> const Header& {
    return header;
}

auto BMPFile::getHeaderInfo() const -> const HeaderInfo& {
    return headerInfo;
}

auto BMPFile::getColorTable() const -> const std::vector<Pixel>& {
    return colorTable;
}

auto BMPFile::getFlag() const -> bool {
    return flag;
}


auto BMPFile::readPixelsFromFile(std::fstream& file) -> void {
    file.seekg(header.getDataOffset(), std::ios::beg);

    auto paddingForRowSize = (4 - (headerInfo.getWidth() * 3) % 4) % 4;
    colorTable.resize(headerInfo.getHeight() * headerInfo.getWidth());

    for (auto i = headerInfo.getHeight() - 1; i >= 0; i--) {
        auto tmpRowVec = std::vector<uint8_t>(3);

        for (auto j = 0; j < headerInfo.getWidth(); j++) {
            file.read(reinterpret_cast<char*>(tmpRowVec.data()), 3);

            colorTable[i * headerInfo.getWidth() + j] = Pixel(tmpRowVec[2],tmpRowVec[1],tmpRowVec[0]);
        }
        file.ignore(paddingForRowSize);
    }
}

auto BMPFile::convertStringToBinary(std::string const& str) -> std::string {
    auto binaryStr = std::string();

    for (auto sign : str) {
        binaryStr += std::bitset<8>(sign).to_string();
    }

    binaryStr += "00000000";

    return binaryStr;
}

auto BMPFile::checkIfImageContainsMessage() -> bool {
    return (colorTable[0].getRed() & 0b11111000) == 0b10100000;
}

auto BMPFile::checkIfMsgFitsIntoImage(int msgSize) -> bool {
    return msgSize > (headerInfo.getWidth() * headerInfo.getHeight() - 1) * 3;
}

auto BMPFile::insertMsgIntoFile(std::string const& msg) -> bool {
    if (checkIfImageContainsMessage()) {
        std::cout << "The image already contains a hidden message.\n";
        return false;
    }

    auto binMsg = convertStringToBinary(msg);

    if (checkIfMsgFitsIntoImage(binMsg.size())) {
        std::cout << "Too long message.\n";
        return false;
    }

    colorTable[0].setRed((colorTable[0].getRed() & ~0b11111000) | 0b10100000);

    auto indexOfPixel = 1;

    for (auto& sign : binMsg) {
        if (indexOfPixel % 2 == 0) {
            if (sign == '0') {
                colorTable[indexOfPixel].setGreen(colorTable[indexOfPixel].getGreen() & ~1);
            } else {
                colorTable[indexOfPixel].setGreen(colorTable[indexOfPixel].getGreen() | 1);
            }
        } else if (indexOfPixel % 3 == 0) {
            if (sign == '0') {
                colorTable[indexOfPixel].setBlue(colorTable[indexOfPixel].getBlue() & ~1);
            } else {
                colorTable[indexOfPixel].setBlue(colorTable[indexOfPixel].getBlue() | 1);
            }
        } else {
            if (sign == '0') {
                colorTable[indexOfPixel].setRed(colorTable[indexOfPixel].getRed() & ~1);
            } else {
                colorTable[indexOfPixel].setRed(colorTable[indexOfPixel].getRed() | 1);
            }
        }
        indexOfPixel++;
    }

    return true;
}

auto BMPFile::readMsgFromFile() -> std::string {
    if (!checkIfImageContainsMessage()) {
        std::cout << "Image doesn't contain any hidden message.\n";
        return "";
    }

    auto startIndex = 1;
    auto hiddenMsgBin = std::string();
    auto isTrue = true;

    while (isTrue) {
        if (startIndex >= colorTable.size()) {
            isTrue = false;
            return "";
        }

        if (startIndex % 2 == 0) {
            auto hiddenBit = colorTable[startIndex].getGreen();
            hiddenMsgBin += (hiddenBit & 1) == 0 ? '0' : '1';
        } else if (startIndex % 3 == 0) {
            auto hiddenBit = colorTable[startIndex].getBlue();
            hiddenMsgBin += (hiddenBit & 1) == 0 ? '0' : '1';
        } else {
            auto hiddenBit = colorTable[startIndex].getRed();
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

    return hiddenMsgStr;
}

auto BMPFile::saveFileWithMsg(std::fstream& file) -> void {
    file.seekp(0, std::ios::beg);

    auto paddingForRowSize = (4 - (headerInfo.getWidth() * 3) % 4) % 4;
    auto sizeOfPixelsData =
        headerInfo.getHeight() * headerInfo.getWidth() * 3 + paddingForRowSize * headerInfo.getHeight();

    header.setFileSize(sizeof(Header) + sizeof(HeaderInfo) + sizeOfPixelsData);
    headerInfo.setNewImageSize(sizeOfPixelsData);

    file.write(reinterpret_cast<const char*>(&header), sizeof(Header));
    file.write(reinterpret_cast<const char*>(&headerInfo), sizeof(HeaderInfo));

    file.seekp(header.getDataOffset(), std::ios::beg);

    for (auto i = headerInfo.getHeight() - 1; i >= 0; i--) {
        for (auto j = 0; j < headerInfo.getWidth(); j++) {
            auto row = std::vector{
                colorTable[i * headerInfo.getWidth() + j].getBlue(),
                colorTable[i * headerInfo.getWidth() + j].getGreen(),
                colorTable[i * headerInfo.getWidth() + j].getRed()
            };
            file.write(reinterpret_cast<char*>(row.data()),3);
        }
        for (auto j = 0; j < paddingForRowSize; j++) {
            file.put(0);
        }
    }
}
