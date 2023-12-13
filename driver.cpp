#include "driver.hpp"

#include <pico/stdlib.h>

namespace ili9341 {
#define BYTE(bitfield, index) (0xFF & (bitfield >> (8 * index)))
#define BIT(bitfield, index) (0b1 & (bitfield >> index))

	/**
	 * @note at the moment we hard code the pins, but it would be better
	 * to take in an enum to describe the pins that the user wants to use
	*/
    enum class pin : uint8_t {
		/* screen */ /* gpio */ /* raspberry pin */
		data0         = 14,       // pin 19
		data1         = 15,       // pin 20
		data2         = 8 ,       // pin 11
		data3         = 9 ,       // pin 12
		data4         = 10,       // pin 14
		data5         = 11,       // pin 15
		data6         = 12,       // pin 16
		data7         = 13,       // pin 17

		rd            = 16,       // pin 21
		wr            = 17,       // pin 22
		rs            = 18,       // pin 24
		cs            = 19,       // pin 25
		rst           = 20,       // pin 26
	};

	/**
	 * @brief commands hexacodes for the ili9341 display. (this enum allows to use the `send` function overload to send commands)
	*/
	enum class command : uint8_t {
		sleep_out             = 0x11,
		display_off           = 0x28,
		display_on            = 0x29,
        column_address_set    = 0x2A,
        page_address_set      = 0x2B,
		memory_write          = 0x2C,
		memory_access_control = 0x36,
		pixel_format_set      = 0x3A,
		vcom1                 = 0xC5,
	};

	/**
	 * @brief simple bitshift overload to avoid casting `command` enum to `uint8_t`
	*/
    uint8_t operator>>(const command cmd, int shift) {
		const uint8_t c = static_cast<uint8_t>(cmd);
		return c >> shift;
	}

	/**
	 * 
	*/
    void send(const command byte) {
		// send command ==> rs = 0
		gpio_put((uint)pin::rs, false);

		// si j'ai bien compris, il faut que le signal
		// soit dans la bonne position lorsqu'on active
		// MOSI ou MISO

		// activate MOSI (we begin sending data to the slave)
		gpio_put((uint)pin::wr, false);

		/*  SEND THE DATA TO THE PINS */
		gpio_put((uint)pin::data0, BIT(byte, 0));
		gpio_put((uint)pin::data1, BIT(byte, 1));
		gpio_put((uint)pin::data2, BIT(byte, 2));
		gpio_put((uint)pin::data3, BIT(byte, 3));
		gpio_put((uint)pin::data4, BIT(byte, 4));
		gpio_put((uint)pin::data5, BIT(byte, 5));
		gpio_put((uint)pin::data6, BIT(byte, 6));
		gpio_put((uint)pin::data7, BIT(byte, 7));

		// deactivate MOSI (we have finished sending the data)
		gpio_put((uint)pin::wr, true);
	}

	/**
	 * 
	*/
    void send(const uint8_t byte) {
		// send data ==> rs = 1
		gpio_put((uint)pin::rs, true);

		// si j'ai bien compris, il faut que le signal
		// soit dans la bonne position lorsqu'on active
		// MOSI ou MISO

		// activate MOSI (we begin sending data to the slave)
		gpio_put((uint)pin::wr, false);

		/*  SEND THE DATA TO THE PINS */
		gpio_put((uint)pin::data0, BIT(byte, 0));
		gpio_put((uint)pin::data1, BIT(byte, 1));
		gpio_put((uint)pin::data2, BIT(byte, 2));
		gpio_put((uint)pin::data3, BIT(byte, 3));
		gpio_put((uint)pin::data4, BIT(byte, 4));
		gpio_put((uint)pin::data5, BIT(byte, 5));
		gpio_put((uint)pin::data6, BIT(byte, 6));
		gpio_put((uint)pin::data7, BIT(byte, 7));

		// deactivate MOSI (we have finished sending the data)
		gpio_put((uint)pin::wr, true);
	}

    void initialize() {

        { 	// activate gpio_pin
			for (int i = 8 ; i <= 20 ; i++){
				gpio_init(i);
				gpio_set_dir(i, GPIO_OUT);
			}
		}	// activate gpio_pin

        { // reset display initial state

			// make sure reset is not active
			gpio_put((uint)pin::rst, true);
			sleep_ms(50);

			// initiate reset
			gpio_put((uint)pin::rst, false);
			sleep_ms(150);

			gpio_put((uint)pin::rst, true);
			sleep_ms(150);

		} // reset display initial state

        // set default state to write and read pin
		gpio_put((uint)pin::wr, true);
		gpio_put((uint)pin::rd, true);

        // set color contrast (?) using the VCOM Control 1
		{ // VCOM Control 1 (ili9341-datasheet.pdf page 180)

			send(command::vcom1);
			send(0b1111111); // est-ce que on a vraiment besoin de maxer ça ?
			send(0b0000000);

		} // VCOM Control 1

		// set
		// display position base (x = 0bX1XXXX00 | y = 0b1XXXXX00)
		// and
		// color format ( bgr = 0bXXXX0X00 | rgb = 0bXXXX1X00)

		{ // Memory Access Control (ili9341-datasheet.pdf page 127)

			send(command::memory_access_control);

			// dans son exemple il utilise le code 0x48 <=> 0b0101000
			// il active donc les bits MX et color format RGB
			send(0b11001000);

		} // Memory Access Control

		{ // COLMOD : Pixel Format Set (ili9341-datasheet.pdf page 134)

			// color depth (0b0 101 0 101)
			//                  ^^^   ^^^
			//                  RGB   MCU
			send(command::pixel_format_set);
			send(0b001010101);

		} // COLMOD : Pixel Format Set

		{ // sleep out (ili9341-datasheet.pdf page 101)

			send(command::sleep_out);
			send(0b00010001);
			sleep_ms(10);

		} // sleep out

		{ // display on (ili9341-datasheet.pdf page 109)

			send(command::display_on);

		} // display on

    }

    /* ------------------------ DRAW COMMANDS ------------------------ */

	/**
	 * @brief this function tells the screen where in memory we are going to send data
	 * @param x_start distance from top of the screen : the must satisfy 0 <= x_start < x_end < 240
	 * @param x_end distance from the top of the screen : must satisfy 0 <= x_start < x_end < 240
	 * @param y_start distance from the left of the screen : must satisfy 0 <= y_start < y_end < 320
	 * @param y_end distance from the left of the screen : must satisfy 0 <= y_start < y_end < 320
	*/
    static void set_draw_region(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end) {
        // coordonnées y de 0 à 320
        send(command::column_address_set);
        send(BYTE(y_start, 1));
        send(BYTE(y_start, 0));
        send(BYTE(y_end,   1));
        send(BYTE(y_end,   0));

        // coordonnées x de 0 à 240
        send(command::page_address_set);
        send(BYTE(x_start, 1));
        send(BYTE(x_start, 0));
        send(BYTE(x_end,   1));
        send(BYTE(x_end,   0));
    }

    void line_y(uint16_t x, uint16_t y, uint16_t length, uint16_t c) {
        set_draw_region(x, x+1, y, y+length);
        send(command::memory_write);

        for(uint16_t i = y; i < length; i++) {
            send(BYTE(c, 1));
            send(BYTE(c, 0));
        }
    }

    void line_y(uint16_t x, uint16_t y, uint16_t length, color c) {
		line_y(x, y, length, (uint16_t)c);
	}

    void line_x(uint16_t x, uint16_t y, uint16_t length, uint16_t c) {
        set_draw_region(x, x+length, y, y);
        send(command::memory_write);

        for(uint16_t i = x; i < length; i++) {
            send(BYTE(c, 1));
            send(BYTE(c, 0));
        }
    }

    void line_x(uint16_t x, uint16_t y, uint16_t length, color c) {
		line_x(x, y, length, (uint16_t)c);
	}

    void rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t c) {
        line_x(x        , y         , width , c);
        line_x(x        , y + height, width , c);
        line_y(x        , y         , height, c);
        line_y(x + width, y         , height, c);
    }

    void rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color c) {
		rect(x, y, width, height, (uint16_t)c);
	}

    void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t c) {
        set_draw_region(x, x+width, y, y+height);
        send(command::memory_write);

        for(uint32_t i = 0; i < width*height; i++) {
            send(BYTE(c, 1));
            send(BYTE(c, 0));
        }
    }

    void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color c) {
		fill_rect(x, y, width, height, (uint16_t)c);
	}

    void clear(uint16_t c) {
        set_draw_region(0, width, 0, height);
        send(command::memory_write);

        for(uint32_t i = 0; i < width * height; i += 4) {
            send(BYTE(c, 1));
            send(BYTE(c, 0));

			send(BYTE(c, 1));
            send(BYTE(c, 0));

			send(BYTE(c, 1));
            send(BYTE(c, 0));

			send(BYTE(c, 1));
            send(BYTE(c, 0));
        }
    }

    void clear(color c) {
		clear((uint16_t)c);
	}

    /* --------------------------------------------------------------- */

#undef BIT
#undef BYTE
} // namespace ili9341
