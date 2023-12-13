#include <pico/stdlib.h>

#include "driver.hpp"
#include "sound.hpp"
#include "controller.hpp"
#include "colors.hpp"

int main(void) {
	ili9341::initialize();
	controller::initialize();

	while(true) {
		if(controller::is_pressed(controller::button::up)) {
			ili9341::clear(color::red);
		} else if(controller::is_pressed(controller::button::down)) {
			ili9341::clear(color::magenta);
		} else if(controller::is_pressed(controller::button::right)) {
			ili9341::clear(color::yellow);
		} else if(controller::is_pressed(controller::button::left)) {
			ili9341::clear(color::green);
		} else if(controller::is_pressed(controller::button::a)) {
			ili9341::clear(color::black);
		} else if(controller::is_pressed(controller::button::b)) {
			ili9341::clear(color::white);
		}
		sleep_ms(100);
	}
}
