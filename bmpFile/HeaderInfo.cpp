#include <fstream>

#include "./HeaderInfo.hpp"

auto HeaderInfo::readHeaderInfoFromFile(std::fstream& file) -> void {
    file.read(reinterpret_cast<char*>(this), sizeof(HeaderInfo));
}

// Setters
auto HeaderInfo::setNewImageSize(int newVal) -> void {
    imageSize = newVal;
}

// Getters
auto HeaderInfo::getSize() const -> uint32_t {
    return size;
};

auto HeaderInfo::getWidth() const -> int32_t {
    return width;
};

auto HeaderInfo::getHeight() const -> int32_t {
    return height;
};

auto HeaderInfo::getPlanes() const -> uint16_t {
    return planes;
};

auto HeaderInfo::getBitsPerPixel() const -> uint16_t {
    return bitsPerPixel;
};

auto HeaderInfo::getCompression() const -> uint32_t {
    return compression;
};

auto HeaderInfo::getImageSize() const -> uint32_t {
    return imageSize;
};

auto HeaderInfo::getXPixelsPerM() const -> int32_t {
    return xPixelsPerM;
};

auto HeaderInfo::getYPixelsPerM() const -> int32_t {
    return yPixelsPerM;
};

auto HeaderInfo::getColorsUsed() const -> uint32_t {
    return colorsUsed;
};

auto HeaderInfo::getImportantColors() const -> uint32_t {
    return importantColors;
};