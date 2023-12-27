#include <pico/stdlib.h>

#include "driver.hpp"
#include "controller.hpp"
#include "colors.hpp"

#include "pico_tone.hpp"

int main(void) {
	ili9341::initialize();
	controller::initialize();

	Tone player(/* pin: */ 28, 80, 5, 5, 10);
	player.init(/* blocking: */ false);

	ili9341::clear(color::black);

	while(true) {}
}
