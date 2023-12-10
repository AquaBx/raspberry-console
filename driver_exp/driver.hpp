#pragma once

#include "stdint.h"

namespace ili9341 {

	void initialize();

	// draw_rect();
	// fill_rect();
	// draw_line();
	// draw_circle();
	// fill_circle();
	// draw_triangle();
	// fill_triangle()

} // namespace ili9341

namespace renderer {

	void initialize(uint16_t fill_color, uint16_t clear_color = 0);

	void set_fill_color(uint16_t c);
	void set_clear_color(uint16_t c);

	void clear();
	void draw();

} // namespace renderer
