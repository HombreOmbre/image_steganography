#pragma once

#include <iostream>

class Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

public:
    Pixel() : red(0), green(0), blue(0) {}

    Pixel(const uint8_t r, const uint8_t g, const uint8_t b)
        : red(r), green(g), blue(b) {}

    // Setters
    auto setRed(uint8_t value) -> void;
    auto setGreen(uint8_t value) -> void;
    auto setBlue(uint8_t value) -> void;

    // Getters
    auto getRed() const -> uint8_t;
    auto getGreen() const -> uint8_t;
    auto getBlue() const -> uint8_t;
};