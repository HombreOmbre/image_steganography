#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "../pixel/Pixel.hpp"

class PPM_File {
    std::string format;
    int width;
    int height;
    int maxColorVal;
    bool flag;
    std::vector<Pixel> pixels;

public:
    PPM_File(std::fstream& file) {
        flag = false;
        file >> format;
        if (!(format == "P3" || format == "P6")) {
            std::cout << "Wrong file. Try with another one.\n";
            return;
        }

        file >> width;
        file >> height;
        if (width == 0 || height == 0) {
            std::cout << "Wrong file. Try with another one.\n";
            return;
        }

        file >> maxColorVal;
        if (maxColorVal != 255) {
            std::cout << "Wrong file. Try with another one.\n";
            return;
        }

        file.ignore(1);

        if (format == "P6") {
            readPixelsFromFormatP6(file);
        }

        if (format == "P3") {
            readPixelsFromFormatP3(file);
        }
        flag = true;
    }

    // Getters
    auto getFormat() const -> std::string;
    auto getWidth() const -> int;
    auto getHeight() const -> int;
    auto getMaxColorVal() const -> int;
    auto getPixels() const -> std::vector<Pixel>;
    auto getFlag() const -> bool;

    auto insertMsgIntoFile(std::string const& msg) -> bool;
    auto readMsgFromFile() -> std::string;
    auto saveFileWithMsg(std::fstream& file) -> void;
    auto checkIfMsgFitsIntoImage(int msgSize) -> bool;
    auto checkIfImageContainsMessage() -> bool;

private:
    auto readPixelsFromFormatP6(std::fstream& file) -> void;
    auto readPixelsFromFormatP3(std::fstream& file) -> void;
    auto convertStringToBinary(std::string const& str) -> std::string;
    auto savePixelsInP3File(std::fstream& file) -> void;
    auto savePixelsInP6File(std::fstream& file) -> void;

};
