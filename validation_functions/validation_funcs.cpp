#include <fstream>

#include "./validation_funcs.hpp"

auto validateNumOfArgs(int const& numOfArgs, int const& expectedNum) -> bool {
    if (numOfArgs < expectedNum) {
        std::cout << "Too few arguments.\n";
        return false;
    }
    if (numOfArgs > expectedNum) {
        std::cout << "Too many arguments.\n";
        return false;
    }
    return true;
}

auto validateImageExtension(std::string const& path) -> bool {
    if (path.ends_with(".ppm") || path.ends_with(".bmp")) {
        return true;
    }
    std::cout << "Wrong extension. Try with \".bmp\" or \".ppm\" extension.\n";
    return false;
}

auto checkIfFileExists(std::string const& path) -> bool {
    if (std::filesystem::exists(path)) {
        return true;
    }
    std::cout << "There is no such file.\n";
    return false;
}

auto checkFilePermissions(std::string const& path) -> bool {
    auto file = std::fstream(path, std::ios::binary | std::ios::in | std::ios::out);
    if (file.good()) {
        return true;
    }
    std::cout << "Do not have correct permissions for manipulate the file.\n";
    return false;
}

auto validateArguments(int const& numOfArgs, int const& expectedNum, std::string const& path) -> bool {
    return !validateNumOfArgs(numOfArgs, expectedNum) ||
           !validateImageExtension(path) ||
           !checkIfFileExists(path) ||
           !checkFilePermissions(path);
}