#pragma once

#include <fstream>
#include <iostream>

#include "./Header.hpp"
#include "./HeaderInfo.hpp"
#include "../pixel/Pixel.hpp"

class BMPFile {
    Header header;
    HeaderInfo headerInfo;
    std::vector<Pixel> colorTable;
    std::fstream filePointer;
    bool flag;

public:
    BMPFile(std::fstream& file) {
        flag = false;
        header.readHeaderFromFile(file);
        if (header.getSignatureAsString() != "BM") {
            std::cout << "Wrong file. Try with another one.\n";
            return;
        }
        headerInfo.readHeaderInfoFromFile(file);
        readPixelsFromFile(file);
        flag = true;
    }

    auto insertMsgIntoFile(std::string const& msg) -> bool;
    auto readMsgFromFile() -> std::string;
    auto saveFileWithMsg(std::fstream& file) -> void;

    // Getters
    auto getHeader() const -> const Header&;
    auto getHeaderInfo() const -> const HeaderInfo&;
    auto getColorTable() const -> const std::vector<Pixel>&;
    auto checkIfMsgFitsIntoImage(int msgSize) -> bool;
    auto checkIfImageContainsMessage() -> bool;
    auto getFlag() const -> bool;

private:
    auto readPixelsFromFile(std::fstream& file) -> void;
    auto convertStringToBinary(std::string const& str) -> std::string;
};
