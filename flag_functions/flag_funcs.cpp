#include <iostream>
#include <fstream>

#include "flag_funcs.hpp"
#include "../ppmFile/ppmFile.hpp"
#include "../bmpFile/bmpFile.hpp"

auto showInfoAboutFile(std::string const& path) -> void {
    auto file = std::fstream(path, std::ios::binary | std::ios::in | std::ios::out);

    auto timestampOfLastModification =
        std::chrono::time_point_cast<std::chrono::seconds>(std::filesystem::last_write_time(path))
            .time_since_epoch().count();

    if (path.ends_with(".ppm")) {
        auto ppmFile = PPM_File(file);
        if (ppmFile.getFlag()) {
            std::cout << "PPM file: \n"
                      << "  Format: " << ppmFile.getFormat() << "\n"
                      << "  Widtht: " << ppmFile.getWidth() << "\n"
                      << "  Height: " << ppmFile.getWidth() << "\n"
                      << "  Last update: " << timestampOfLastModification << "\n"
                      << "  File size: " << (std::filesystem::file_size(path) / (1024.0 * 1024.0)) << " MB"<< "\n";
        }
    } else {
        auto bmpFile = BMPFile(file);
        if (bmpFile.getFlag()) {
            std::cout << "BMP file:\n"
                      << "  Width: " << bmpFile.getHeaderInfo().getWidth() << "\n"
                      << "  Height: " << bmpFile.getHeaderInfo().getHeight() << "\n"
                      << "  Last update: " << timestampOfLastModification << "\n"
                      << "  File size: " << (std::filesystem::file_size(path) / 1024.0) << " MB" << "\n";
        }
    }
}

auto encryptFile(std::string const& path, std::string& msg) -> void {
    auto file = std::fstream(path, std::ios::binary | std::ios::in | std::ios::out);

    if (path.ends_with(".ppm")) {
        auto ppmFile = PPM_File(file);
        if (ppmFile.getFlag()) {
            if (!ppmFile.insertMsgIntoFile(msg)) {
                return;
            }
            ppmFile.saveFileWithMsg(file);
            std::cout << "Message inserted and saved successfully.\n";
        }
    } else {
        auto bmpFile = BMPFile(file);
        if (bmpFile.getFlag()) {
            if (!bmpFile.insertMsgIntoFile(msg)) {
                return;
            }
            bmpFile.saveFileWithMsg(file);
            std::cout << "Message inserted and saved successfully.\n";
        }
    }
}

auto decryptFile(std::string const& path) -> void {
    auto file = std::fstream(path, std::ios::binary | std::ios::in | std::ios::out);

    if (path.ends_with(".ppm")) {
        auto ppmFile = PPM_File(file);
        if (ppmFile.getFlag()) {
            auto msg = ppmFile.readMsgFromFile();
            if (!msg.empty()) {
                std::cout << "Decrypted message: ";
                std::cout << msg << "\n";
            }
        }
    } else {
        auto bmpFile = BMPFile(file);
        if (bmpFile.getFlag()) {
            auto msg = bmpFile.readMsgFromFile();
            if (!msg.empty()) {
                std::cout << "Decrypted message: ";
                std::cout << msg << "\n";
            }
        }
    }
}

auto checkIfMessageCanBeInsertedIntoFile(std::string const& path, std::string& msg) -> void {
    auto file = std::fstream(path, std::ios::binary | std::ios::in | std::ios::out);
    auto lengthOfBinMsg = msg.size() * 8 + 8;

    if (path.ends_with(".ppm")) {
        auto ppmFile = PPM_File(file);
        if (ppmFile.getFlag()) {
            if (ppmFile.checkIfImageContainsMessage()) {
                std::cout << "The image already contains a hidden message.\n";
                return;
            }
            if (ppmFile.checkIfMsgFitsIntoImage(lengthOfBinMsg)) {
                std::cout << "Message is too long to hide into this picture.\n";
                return;
            }
            std::cout << "Message can be inserted into image.\n";
        }
    } else {
        auto bmpFile = BMPFile(file);
        if (bmpFile.getFlag()) {
            if (bmpFile.checkIfImageContainsMessage()) {
                std::cout << "The image already contains a hidden message.\n";
                return;
            }
            if (bmpFile.checkIfMsgFitsIntoImage(lengthOfBinMsg)) {
                std::cout << "Message is too long to hide into this picture.\n";
                return;
            }
            std::cout << "Message can be inserted into image.\n";
        }
    }
}

auto showHelpInfoAboutApp() -> void {
    std::cout << "Instruction for using steganography app:\n"
              << " * For getting information about image, type in the console:\n"
              << "      path/to/the/app -i or --info path/to/the/image\n"
              << "-----------------------------------------------------------------------------------------------------------------------\n"
              << " * For encrypting message into image, type in the console:\n"
              << "      path/to/the/app -e or --encypt path/to/the/image \"message you want to insert into image\"\n"
              << "-----------------------------------------------------------------------------------------------------------------------\n"
              << " * For decrypting message from the image, type in the console:\n"
              << "      path/to/the/app -d or --decrypt path/to/the/image\n"
              << "-----------------------------------------------------------------------------------------------------------------------\n"
              << " * For checking if message can be inserted into file, type in the console:\n"
              << "      path/to/the/app -c or --check path/to/the/image \"message you want to insert into image\"\n"
              << "-----------------------------------------------------------------------------------------------------------------------\n"
              << " * For getting information about application flags/aliases, what they do and how to write command, type in the console:\n"
              << "      path/to/the/app -h or --help\n"
              << "-----------------------------------------------------------------------------------------------------------------------\n"
              << "This application to the steganography, only support these extensions of the file:\n"
              << "  * .bmp\n"
              << "  * .ppm\n";
}