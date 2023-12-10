#pragma once

#include "stdint.h"

namespace ili9341 {

	void initialize();

	void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
	// draw_rect();
	// fill_rect();
	// draw_line();
	// draw_circle();
	// fill_circle();
	// draw_triangle();
	// fill_triangle()

} // namespace ili9341
