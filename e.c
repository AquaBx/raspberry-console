#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

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

#define LCD_RD   A0 //Serves as read signal/MCU read data at the rising edge. Pg11 Datasheet
#define LCD_WR   17 //Serves as write signal/command at the rising edge
#define LCD_RS   A2 //D/CX (0=Command/1=Data)       
#define LCD_CS   A3 //Chip Select Pin : Active Low
#define LCD_RST  A4 //Shield Reset

void LCD_write(uint8_t d) {
  // Serves as write signal/command at the rising edge

  gpio_put(LCD_WR,0x0)


  // LCD Pins |7|6|5|4|3|2|1|0|
  // Uno Pins |7|6|5|4|3|2|9|8|

  //Arduino Port D : Pin 0 - 7, but Pin 0,1 not used in the LCD Shield
  //Arduino Port B : Pin 8 - 13,but Pin, 10,11,12,13 not used in the LCD Shield
  //Arduino Port C : Analog Pins

  PORTD = (PORTD & B00000011) | ((d) & B11111100);
  PORTB = (PORTB & B11111100) | ((d) & B00000011);

  digitalWrite(LCD_WR, HIGH); // WR 1
}

void LCD_command_write(uint8_t command) {
  // LCD_RS = 0, A2=0, D/CX (0=Command/1=Data) | DataSheet Page 11
  digitalWrite(LCD_RS, LOW);
  LCD_write(command);
}

void LCD_data_write(uint8_t data) {
  // LCD_RS = 1, A2=1, D/CX (0=Command/1=Data) | DataSheet Page 11
  digitalWrite(LCD_RS, HIGH);
  LCD_write(data);
}

void Lcd_Init(void) {
  //void does not return any value
  //void only execute instruction within it
  //similar to void setup and loop
  //This function will have LCD initialization measures
  //Only the necessary Commands are covered
  //Eventho there are so many more in DataSheet

  //Reset Signal is Active LOW
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW); //Actual Reset Done Here
  delay(15);
  digitalWrite(LCD_RST, HIGH);
  delay(15);

  //The below is just preparation for Write Cycle Seq
  digitalWrite(LCD_CS, HIGH); //Chip-Select Active Low Signal
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_CS, LOW);  //Chip-Select Active Low Signal

  LCD_command_write(0xC5);    //Test this Out | VCOM Control 1 : Colour Contrast Maybe
  LCD_data_write(0x54);       //VCOM H 1111111 0x7F
  LCD_data_write(0x00);       //VCOM L 0000000
  //LCD_data_write(B1010011);
  
  LCD_command_write(0x36);    //Memory Access Control | DataSheet Page 127
  ///LCD_data_write(0x48);       //Adjust this value to get right color and starting point of x and y
  LCD_data_write(B0000100);     //Example

  LCD_command_write(0x3A);    //COLMOD: Pixel Format Set | DataSheet Page 134
  LCD_data_write(0x55);       //16 Bit RGB and MCU

  LCD_command_write(0x11);    //Sleep Out | DataSheet Page 245
  delay(10);                  //Necessary to wait 5msec before sending next command

  LCD_command_write(0x29);    //Display on.

  LCD_command_write(0x2c);    //Memory Write | DataSheet Page 245
}

void Address_set(int16_t y1, int16_t y2, int16_t x1, int16_t x2) {
  LCD_command_write(0x2a);  //Column Address Set | DataSheet Page 110
  LCD_data_write(y1 >> 8);  //8 Bit Shift Right of y1
  LCD_data_write(y1);       //Value of y1
  LCD_data_write(y2 >> 8);  //8 Bit Shift Right of y2
  LCD_data_write(y2);       //Value of y2

  LCD_command_write(0x2b);  //Page Address Set | DataSheet Page 110
  LCD_data_write(x1 >> 8);  //8 Bit Shift Right of x1
  LCD_data_write(x1);       //Value of x1
  LCD_data_write(x2 >> 8);  //8 Bit Shift Right of x2
  LCD_data_write(x2);       //Value of x2

  LCD_command_write(0x2c); // REG 2Ch = Memory Write
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
  digitalWrite(LCD_CS, LOW);// Chip Select active
  Address_set(y, y + 1, x, x + 1);
  //LCD_command_write(0x2C);
  LCD_data_write(color >> 8);
  LCD_data_write(color);
}

void setup() {
  // Setting Pin 2-7 as Output, DDR is PinMode Command, Pin0,1 Untouched
  DDRD = DDRD | B11111100;
  // Setting Pin 8-9 as Output
  DDRB = DDRB | B00000011;
  //Setting Analog Pins A4-A0 as Output
  DDRC = DDRC | B00011111;
  //Setting Analog Pins A4-A0 as HIGH
  //PORTC = PORTC | B00011111;
  Lcd_Init();
  Serial.begin(9600);
}

void loop() {
      for (int i = 50; i < 300; i++) {
        drawPixel(i, 60, WHITE);
        drawPixel(i, 70, WHITE);
        drawPixel(i, 80, WHITE);
        drawPixel(i, 90, WHITE);
        drawPixel(i, 100, WHITE);
        drawPixel(i, 110, WHITE);
        drawPixel(i, 120, WHITE);
        drawPixel(i, 130, WHITE);
        drawPixel(i, 140, WHITE);
        drawPixel(i, 150, WHITE);
      }
}