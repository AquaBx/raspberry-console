#pragma once

#include <cstdint>

namespace controller {

    /**
     * @brief 
    */
    enum class button : uint8_t { up, down, right, left, b, a };

    void initialize();

    bool is_pressed(const button btn);

} // namespace controller

namespace global {

    constexpr inline controller::button buttons[] = {
        controller::button::up,
        controller::button::down,
        controller::button::right,
        controller::button::left,
        controller::button::b,
        controller::button::a
    };

} // namespace global
