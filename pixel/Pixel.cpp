#include "./Pixel.hpp"

auto Pixel::setRed(uint8_t value) -> void {
    red = value;
}

auto Pixel::setGreen(uint8_t value) -> void {
    green = value;
}

auto Pixel::setBlue(uint8_t value) -> void {
    blue = value;
}

auto Pixel::getRed() const -> uint8_t {
    return red;
}

auto Pixel::getGreen() const -> uint8_t {
    return green;
}

auto Pixel::getBlue() const -> uint8_t {
    return blue;
}
