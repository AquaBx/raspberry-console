#pragma once

#include <cstdint>

namespace controller {

    /**
     * @brief 
    */
    enum class button : uint8_t { up, down, right, left, b, a };

    /**
     * 
    */
    void initialize();

    /**
     * 
    */
    bool is_pressed(const button btn);

} // namespace controller
