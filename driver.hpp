#pragma once

#include <cstdint>

#include "colors.hpp"

namespace ili9341 {

    constexpr inline uint16_t width = 320, height = 240;

    /**
	 * @brief sets up all gpio ports and initialize the ili9341 for rendering
     * 
     * @note at the moment gpio are hard coded:
     * ```
     *  (#-- screen --# | #-- gpio --# | #-- raspberry --#)
     * 
     *  (      D0       |      14      |        19        )
     *  (      D1       |      15      |        20        )
     *  (      D2       |       8      |        11        )
     *  (      D3       |       9      |        12        )
     *  (      D4       |      10      |        14        )
     *  (      D5       |      11      |        15        )
     *  (      D6       |      12      |        16        )
     *  (      D7       |      13      |        17        )
     * 
     *  (      RD       |      16      |        21        )
     *  (      WR       |      17      |        22        )
     *  (      RS       |      18      |        24        )
     *  (      CS       |      19      |        25        )
     *  (      RST      |      20      |        26        )
     * ```
    */
    void initialize();

    /**
	 * @brief draw a line horizontally
	 * 
	 * @param x distance from the left of the screen : must satisfy 0 <= x < 320
	 * @param y distance from the top of the screen : must satisfy 0 <= y < 240 - length
	 * @param length length of the line in pixels : must satisfy 0 < length < 240 - y
	 * @param color rgb color 16 bit depth
	*/
    void line_y(uint16_t x, uint16_t y, uint16_t length, uint16_t c);
    void line_y(uint16_t x, uint16_t y, uint16_t length, color c);

    /**
	 * @brief draw a line vertically
	 * 
	 * @param x distance from the left of the screen : must satisfy 0 <= x < 320 - length
	 * @param y distance from the top of the screen : must satisfy 0 <= y < 240
	 * @param length length of the line in pixels : must satisfy 0 < length < 320 - x
	 * @param color rgb color 16 bit depth
	*/
    void line_x(uint16_t x, uint16_t y, uint16_t length, uint16_t c);
    void line_x(uint16_t x, uint16_t y, uint16_t length, color c);
    
    /**
	 * @brief draw a rectangular frame
	 * 
	 * @param x distance from the left of the screen : must satisfy 0 <= x < 320 - width
	 * @param y distance from the top of the screen : must satisfy 0 <= y < 240 - height
	 * @param width width of the rectangle in pixels : must satisfy 0 < width < 320 - x
	 * @param height height of the rectangle in pixels : must satisfy 0 < height < 240 - y
	 * @param color rgb color 16 bit depth
	*/
    void rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t c);
    void rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color c);
    
    /**
	 * @brief fill a rectangle
	 * 
	 * @param x distance from the left of the screen : must satisfy 0 <= x < 320 - width
	 * @param y distance from the top of the screen : must satisfy 0 <= y < 240 - height
	 * @param width width of the rectangle in pixels : must satisfy 0 < width < 320 - x
	 * @param height height of the rectangle in pixels : must satisfy 0 < height < 240 - y
	 * @param color rgb color 16 bit depth
	*/
    void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t c);
    void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color c);

    /**
     * @brief clear entire screen with the provided color
     * 
     * @param color rgb color 16 bit depth
    */
    void clear(uint16_t c);
    void clear(color c);

} // namespace ili9341
