#include "controller.hpp"

#include <pico/stdlib.h>

namespace controller {

    static const uint8_t buttons[] = {
        (uint8_t)button::up,
        (uint8_t)button::down,
        (uint8_t)button::right,
        (uint8_t)button::left,
        (uint8_t)button::b,
        (uint8_t)button::a,
    };

    void initialize() {
        for(uint8_t btn : buttons) {
            gpio_init(btn);
            gpio_set_dir(btn, GPIO_IN);
            gpio_pull_up(btn);
        }
    }

    bool is_pressed(const button btn) {
        // buttons are input pull_up so it is down when we press it
        return !gpio_get((uint8_t)btn);
    }

} // namespace controller
