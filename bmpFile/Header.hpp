#pragma once

#include <fstream>

#pragma pack(push,1)
class Header {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;

public:
    auto readHeaderFromFile(std::fstream& file) -> void;

    // Setters
    auto setFileSize(int newVal) -> void;

    // Getters
    auto getSignature() const -> uint16_t;
    auto getSignatureAsString() const -> std::string;
    auto getFileSize() const -> uint32_t;
    auto getReserved() const -> uint32_t;
    auto getDataOffset() const -> uint32_t;
};
#pragma pack(pop)
