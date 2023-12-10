#include "driver.hpp"

#include "pico/stdlib.h"

#define BYTE_AT(buf, off) (buf >> (8 * off))

namespace ili9341 {

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

	enum class command : uint8_t {
		sleep_out             = 0x11,
		display_on            = 0x29,
        column_address_set    = 0x2A,
        page_address_set      = 0x2B,
		memory_write          = 0x2C,
		memory_access_control = 0x36,
		pixel_format_set      = 0x3A,
		vcom1                 = 0xC5,
	};

    void send(const command cmd);
	void send(const uint8_t data);

    void initialize() {

		{ // reset display initial state

			// voir si ça à un impact sur le init
			// pour le code arduino les temps sont
			// multipliés par 10
			const unsigned int multiplier = (true) ? 10 : 1;

			// make sure reset is not active
			gpio_put((uint)pin::rst, true);
			sleep_ms(5 * multiplier);

			// initiate reset
			gpio_put((uint)pin::rst, false);
			sleep_ms(15 * multiplier);

			gpio_put((uint)pin::rst, true);
			sleep_ms(15 * multiplier);

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
		// color format (rgb = 0bXXXX0X00 | bgr = 0bXXXX1X00)

		{ // Memory Access Control (ili9341-datasheet.pdf page 127)

			send(command::memory_access_control);

			// dans son exemple il utilise le code 0x48 <=> 0b0101000
			// il active donc les bits MX et color format BGR
			send(0b00000000);

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
			send(0b00101001);

		} // display on
	}

    void send(const command byte) {
		// send command ==> rs = 0
		gpio_put((uint)pin::rs, false);

		// si j'ai bien compris, il faut que le signal
		// soit dans la bonne position lorsqu'on active
		// MOSI ou MISO

		// activate MOSI (we begin sending data to the slave)
		gpio_put((uint)pin::wr, false);

		/*  SEND THE DATA TO THE PINS */

		// deactivate MOSI (we have finished sending the data)
		gpio_put((uint)pin::wr, true);
	}

	void send(const uint8_t byte) {
		// send data ==> rs = 1
		gpio_put((uint)pin::rs, true);

		// si j'ai bien compris, il faut que le signal
		// soit dans la bonne position lorsqu'on active
		// MOSI ou MISO

		// activate MOSI (we begin sending data to the slave)
		gpio_put((uint)pin::wr, false);

		/*  SEND THE DATA TO THE PINS */

		// deactivate MOSI (we have finished sending the data)
		gpio_put((uint)pin::wr, true);
	}

    void set_draw_region(uint16_t start_column, uint16_t end_column, uint16_t start_row, uint16_t end_row) {
        send(command::column_address_set);
        send(BYTE_AT(start_column, 1));
        send(BYTE_AT(start_column, 0));
        send(BYTE_AT(end_column,   1));
        send(BYTE_AT(end_column,   0));

        send(command::page_address_set);
        send(BYTE_AT(start_row, 1));
        send(BYTE_AT(start_row, 0));
        send(BYTE_AT(end_row,   1));
        send(BYTE_AT(end_row,   0));
    }

    /* ------------ DRAW COMMANDS ------------ */

    void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
        set_draw_region(y, y+1, x, x+1);
        send(command::memory_write);
        send(BYTE_AT(color, 1));
        send(BYTE_AT(color, 0));
    }

    /* --------------------------------------- */

} // namespace ili9341

#undef BYTE_AT
