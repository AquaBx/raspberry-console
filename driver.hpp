#pragma once

#include <cstdint>

namespace ili9341 {

    constexpr inline uint16_t width = 320, height = 240;

    void initialize();

    void line_y(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
    void line_x(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
    void rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

    void clear(uint16_t color);

} // namespace ili9341
