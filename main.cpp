#include "driver.hpp"
#include "sound.hpp"
#include "controller.hpp"
#include "colors.hpp"

int main(void) {
	ili9341::initialize();
	bool redraw = true;

	while(true) {
		if(redraw) {
			ili9341::clear(color::black);
			ili9341::fill_rect(100, 0, 200, 100, color::red);
			ili9341::rect(0, 0, 10, 100, color::white);
			redraw = false;
		}
	}
}
