#pragma once

#include "stdint.h"

namespace ili9341 {

	void initialize();

    void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
	void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
	void clear(uint16_t color) ;
	void draw_buffer();


	// draw_line();
	// draw_circle();
	// fill_circle();
	// draw_triangle();
	// fill_triangle()

} // namespace ili9341
