#include "controller.hpp"

#include <pico/stdlib.h>

namespace controller {

    void initialize() {
        for(button btn : global::buttons) {
            gpio_init((uint8_t)btn);
            gpio_set_dir((uint8_t)btn, GPIO_IN);
            gpio_pull_up((uint8_t)btn);
        }
    }

    bool is_pressed(const button btn) {
        return gpio_get((uint8_t)btn);
    }

} // namespace controller
