#pragma once

#include <cstdint>

enum class color : uint16_t {
    black   = 0x0000,
    white   = 0xFFFF,

    red     = 0xF800,
    green   = 0x0FF0,
    blue    = 0x001F,

    cyan    = 0x07FF,
    magenta = 0xF81F,
    yellow  = 0xFFE0,
};

namespace global {

    constexpr inline color colors[] = { color::black, color::white, color::red, color::green, color::blue, color::cyan, color::magenta, color::yellow };

} // namespace global
