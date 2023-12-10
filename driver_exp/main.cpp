#include "stdio.h"
#include "stdlib.h"
#include <stdlib.h>
#include "pico/stdlib.h"

#include "driver.hpp"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define RED2    0x4000
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREEN2  0x2FA4
#define CYAN2   0x07FF

int main() {
  ili9341::initialize();
  renderer::initialize(0,0);

	while (true) {
	  renderer::set_clear_color(0xffff);
    renderer::clear();
    renderer::draw();
	}
  
  return 0;
}