#include <fstream>

#include "./Header.hpp"

auto Header::readHeaderFromFile(std::fstream& file) -> void {
    file.read(reinterpret_cast<char*>(this), sizeof(Header));
}

// Setters
auto Header::setFileSize(int newVal) -> void {
    fileSize = newVal;
}

// Getters
auto Header::getSignature() const -> uint16_t {
    return signature;
}

auto Header::getSignatureAsString() const -> std::string {
    return std::string(1, static_cast<char>(signature & 0b11111111)) + std::string(1, (signature >> 8 & 0b11111111));
}


auto Header::getFileSize() const -> uint32_t {
    return fileSize;
}

auto Header::getReserved() const -> uint32_t {
    return reserved;
}

auto Header::getDataOffset() const -> uint32_t {
    return dataOffset;
}