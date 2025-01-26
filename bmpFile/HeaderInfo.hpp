#pragma once

#include <fstream>


#pragma pack(push,1)
class HeaderInfo {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t importantColors;

public:
    auto readHeaderInfoFromFile(std::fstream& file) -> void;

    // Setters
    auto setNewImageSize(int newVal) -> void;

    // Getters
    auto getSize() const -> uint32_t;
    auto getWidth() const -> int32_t;
    auto getHeight() const -> int32_t;
    auto getPlanes() const -> uint16_t;
    auto getBitsPerPixel() const -> uint16_t;
    auto getCompression() const -> uint32_t;
    auto getImageSize() const -> uint32_t;
    auto getXPixelsPerM() const -> int32_t;
    auto getYPixelsPerM() const -> int32_t;
    auto getColorsUsed() const -> uint32_t;
    auto getImportantColors() const -> uint32_t;
};
#pragma pack(pop)