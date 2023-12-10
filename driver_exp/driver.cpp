#include "driver.hpp"

#include "stdlib.h"
#include "pico/stdlib.h"

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
		display_off           = 0x28,
		display_on            = 0x29,
        column_address_set    = 0x2A,
        page_address_set      = 0x2B,
		memory_write          = 0x2C,
		memory_access_control = 0x36,
		pixel_format_set      = 0x3A,
		vcom1                 = 0xC5,
	};

	uint8_t operator>>(const command cmd, int shift) {
		const uint8_t c = static_cast<uint8_t>(cmd);
		return c >> shift;
	}

#define BYTE_AT(buf, off) (buf >> (8 * off))
#define BIT_AT(buf, off) (0b1 & (buf >> off))

    void send(const command cmd);
	void send(const uint8_t data);

    void initialize() {

		{ 	// activate gpio_pin
			for (int i = 8 ; i<=20 ; i++){
				gpio_init(i);
				gpio_set_dir(i, GPIO_OUT);
			}
		}	// activate gpio_pin

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
		// color format ( bgr = 0bXXXX0X00 | rgb = 0bXXXX1X00)

		{ // Memory Access Control (ili9341-datasheet.pdf page 127)

			send(command::memory_access_control);

			// dans son exemple il utilise le code 0x48 <=> 0b0101000
			// il active donc les bits MX et color format RGB
			send(0b00001000);

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

    void send(const command byte) {
		// send command ==> rs = 0
		gpio_put((uint)pin::rs, false);

		// si j'ai bien compris, il faut que le signal
		// soit dans la bonne position lorsqu'on active
		// MOSI ou MISO

		// activate MOSI (we begin sending data to the slave)
		gpio_put((uint)pin::wr, false);

		/*  SEND THE DATA TO THE PINS */
		gpio_put((uint)pin::data0, BIT_AT(byte, 0));
		gpio_put((uint)pin::data1, BIT_AT(byte, 1));
		gpio_put((uint)pin::data2, BIT_AT(byte, 2));
		gpio_put((uint)pin::data3, BIT_AT(byte, 3));
		gpio_put((uint)pin::data4, BIT_AT(byte, 4));
		gpio_put((uint)pin::data5, BIT_AT(byte, 5));
		gpio_put((uint)pin::data6, BIT_AT(byte, 6));
		gpio_put((uint)pin::data7, BIT_AT(byte, 7));

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
		gpio_put((uint)pin::data0, BIT_AT(byte, 0));
		gpio_put((uint)pin::data1, BIT_AT(byte, 1));
		gpio_put((uint)pin::data2, BIT_AT(byte, 2));
		gpio_put((uint)pin::data3, BIT_AT(byte, 3));
		gpio_put((uint)pin::data4, BIT_AT(byte, 4));
		gpio_put((uint)pin::data5, BIT_AT(byte, 5));
		gpio_put((uint)pin::data6, BIT_AT(byte, 6));
		gpio_put((uint)pin::data7, BIT_AT(byte, 7));

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

} // namespace ili9341

namespace renderer {

	uint16_t *buffer1, *buffer2;

	struct Self {
		uint16_t fill_color, clear_color;
		uint16_t *front_buffer, *back_buffer;

		struct Draw_Call {
			uint16_t x, w;
			uint8_t y, h;
		} draw_call;

	};

	Self self;

	void initialize(uint16_t fill_color, uint16_t clear_color) {
		self.fill_color = fill_color;
		self.clear_color = clear_color;

		buffer1 = (uint16_t*)malloc(76800 * sizeof(uint16_t));
		buffer2 = (uint16_t*)malloc(76800 * sizeof(uint16_t));

		self.front_buffer = &buffer1[0];
		self.back_buffer = &buffer2[0];
	}

	void set_fill_color(uint16_t c) {
		self.fill_color = c;
	}

	void set_clear_color(uint16_t c) {
		self.clear_color = c;
	}

	void clear() {
		for(uint32_t i = 0; i < 76800; i += 4) {
			self.back_buffer[i+0] = self.clear_color;
			self.back_buffer[i+1] = self.clear_color;
			self.back_buffer[i+2] = self.clear_color;
			self.back_buffer[i+3] = self.clear_color;
		}

		self.draw_call.x = 0;
		self.draw_call.y = 0;
		self.draw_call.w = 320;
		self.draw_call.x = 240;

		draw();
	}

	void draw() {
		ili9341::set_draw_region(
			self.draw_call.x,
			self.draw_call.x + self.draw_call.w,
			self.draw_call.y,
			self.draw_call.y + self.draw_call.h
		);
		ili9341::send(ili9341::command::memory_write);

		for(uint16_t x = self.draw_call.x; x < self.draw_call.w; x++) {
			for(uint8_t y = self.draw_call.y; y < self.draw_call.h; y++) {
				ili9341::send(BYTE_AT(self.back_buffer[x * 240 + y],1));
				ili9341::send(BYTE_AT(self.back_buffer[x * 240 + y],0));
			}
		}

		uint16_t* temp = self.front_buffer;
		self.front_buffer = self.back_buffer;
		self.back_buffer = temp;
	}

} // namespace renderer

#undef BYTE_AT
#undef BIT_AT
