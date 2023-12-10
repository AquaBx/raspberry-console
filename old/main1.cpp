#include "stdio.h"
#include "stdlib.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h" 

#include "hardware/gpio.h"
#include "pico_tone.hpp"

#define BIT(d, off) (d & (off<<0b1))
#define HIGH 1
#define LOW  0

enum division {
  ronde = 1,
  blanche = 2,
  noire = 4,
  croche = 8,
  double_croche = 16,
  tripple_croche = 32,
};

enum note {
  silence
};

typedef enum {
  /* screen */ /* gpio */ /* raspberry pin */
  DT2         = 8 ,       // pin 11
  DT3         = 9 ,       // pin 12
  DT4         = 10,       // pin 14
  DT5         = 11,       // pin 15
  DT6         = 12,       // pin 16
  DT7         = 13,       // pin 17
  DT0         = 14,       // pin 19
  DT1         = 15,       // pin 20

  RD          = 16,       // pin 21
  WR          = 17,       // pin 22
  RS          = 18,       // pin 24
  CS          = 19,       // pin 25
  RST         = 20,       // pin 26
} PIN;

void LCD_write(uint8_t d) {

  // LCD Pins |7|6|5|4|3|2|1|0|
  // Uno Pins |7|6|5|4|3|2|9|8|

  //Arduino Port D : Pin 0 - 7, but Pin 0,1 not used in the LCD Shield
  //Arduino Port B : Pin 8 - 13,but Pin, 10,11,12,13 not used in the LCD Shield
  //Arduino Port C : Analog Pins

  gpio_put(PIN::DT0, BIT(d, 0) /* (d>>0)&0b1 */ );
  gpio_put(PIN::DT1, BIT(d, 1) /* (d>>1)&0b1 */ );
  gpio_put(PIN::DT2, BIT(d, 2) /* (d>>2)&0b1 */ );
  gpio_put(PIN::DT3, BIT(d, 3) /* (d>>3)&0b1 */ );
  gpio_put(PIN::DT4, BIT(d, 4) /* (d>>4)&0b1 */ );
  gpio_put(PIN::DT5, BIT(d, 5) /* (d>>5)&0b1 */ );
  gpio_put(PIN::DT6, BIT(d, 6) /* (d>>6)&0b1 */ );
  gpio_put(PIN::DT7, BIT(d, 7) /* (d>>7)&0b1 */ );

  gpio_put(PIN::WR, LOW);
  gpio_put(PIN::WR, HIGH);
  
}

void Lcd_Write_Com(unsigned data) {
	gpio_put(PIN::RS, LOW);

	LCD_write(data);
}

void Lcd_Write_Data(unsigned data) {
	gpio_put(PIN::RS, HIGH);

	LCD_write(data);
}

void Lcd_Write_Data_16(unsigned data) {
	gpio_put(PIN::RS, HIGH);

  Lcd_Write_Data(data>>8);
  Lcd_Write_Data(data);
}

void Lcd_Write_Com_Data(unsigned com , unsigned data) {
  Lcd_Write_Com(com);
  Lcd_Write_Data(data);
}

void LCD_WriteReg(unsigned char com,unsigned int dat){
  Lcd_Write_Com(com);
  Lcd_Write_Data_16(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  Lcd_Write_Com(0x50);
	Lcd_Write_Data_16(x1);
  
  Lcd_Write_Com(0x51);
	Lcd_Write_Data_16(x2);
        
  Lcd_Write_Com(0x52);
	Lcd_Write_Data_16(y1);
  
	Lcd_Write_Com(0x53);
	Lcd_Write_Data_16(y2);

   Lcd_Write_Com(0x20);
  Lcd_Write_Data_16(x1);
  
  Lcd_Write_Com(0x21);
  Lcd_Write_Data_16(y1);
	Lcd_Write_Com(0x22); 							 
}


void Lcd_Init(void) {

  gpio_put(PIN::RST,HIGH);
  sleep_ms( 2 * 50); 
  gpio_put(PIN::RST,LOW);
  sleep_ms( 2 * 150);
  gpio_put(PIN::RST,HIGH);
  sleep_ms( 2 * 150);

  gpio_put(PIN::CS,HIGH);
  gpio_put(PIN::RD,HIGH);
  gpio_put(PIN::WR,HIGH);
  gpio_put(PIN::CS,LOW);  //CS


  Lcd_Write_Com(0xC5);    //Test this Out | VCOM Control 1 : Colour Contrast Maybe
  Lcd_Write_Data(0B10101000);       //VCOM H 1111111 0x7F
  Lcd_Write_Data(0x00);       //VCOM L 0000000
  //LCD_data_write(B1010011);
  
  Lcd_Write_Com(0x36);    //Memory Access Control | DataSheet Page 127
  ///LCD_data_write(0x48);       //Adjust this value to get right color and starting point of x and y
  Lcd_Write_Data(0B0000100);     //Example

  Lcd_Write_Com(0x3A);    //COLMOD: Pixel Format Set | DataSheet Page 134
  Lcd_Write_Data(0x55);       //16 Bit RGB and MCU

  Lcd_Write_Com(0x11);    //Sleep Out | DataSheet Page 245
  sleep_ms(100);                  //Necessary to wait 5msec before sending next command

  Lcd_Write_Com(0x29);    //Display on.

  Lcd_Write_Com(0x2c);    //Memory Write | DataSheet Page 245



}



void drawPixel(unsigned int x, unsigned int y, unsigned int color) {

  gpio_put(PIN::CS, LOW);

  Address_set(x, y, x + 1 , y + 1);

  Lcd_Write_Data_16(color);

  gpio_put(PIN::CS,HIGH);   


}

void setup() {
  // Setting Pin 2-7 as Output, DDR is PinMode Command, Pin0,1 Untouched

  gpio_init(PIN::DT0);
  gpio_set_dir(PIN::DT0 , GPIO_OUT);

  gpio_init(PIN::DT1);
  gpio_set_dir(PIN::DT1 , GPIO_OUT);

  gpio_init(PIN::DT2);
  gpio_set_dir(PIN::DT2 , GPIO_OUT);

  gpio_init(PIN::DT3);
  gpio_set_dir(PIN::DT3 , GPIO_OUT);

  gpio_init(PIN::DT4);
  gpio_set_dir(PIN::DT4 , GPIO_OUT);

  gpio_init(PIN::DT5);
  gpio_set_dir(PIN::DT5 , GPIO_OUT);

  gpio_init(PIN::DT6);
  gpio_set_dir(PIN::DT6 , GPIO_OUT);

  gpio_init(PIN::DT7);
  gpio_set_dir(PIN::DT7 , GPIO_OUT);

  gpio_init(PIN::CS);
  gpio_set_dir(PIN::CS, GPIO_OUT);

  gpio_init(PIN::RD);
  gpio_set_dir(PIN::RD, GPIO_OUT);

  gpio_init(PIN::WR);
  gpio_set_dir(PIN::WR, GPIO_OUT);

  gpio_init(PIN::RST);
  gpio_set_dir(PIN::RST, GPIO_OUT);

  gpio_init(PIN::RS);
  gpio_set_dir(PIN::RS, GPIO_OUT);


  gpio_put(PIN::CS, HIGH);
  gpio_put(PIN::RST, HIGH);
  gpio_put(PIN::RS, HIGH);
  gpio_put(PIN::WR, HIGH);
  gpio_put(PIN::RD, HIGH);

  Lcd_Init();
}

void LCD_Clear(unsigned int j)                   
{	
  unsigned int i,m;
  Address_set(0,0,240-1,320-1);
  //Lcd_Write_Com(0x02c); //write_memory_start
  //digitalWrite(LCD_RS,HIGH);
  gpio_put(PIN::CS,LOW);


  for(i=0;i<320;i++){
    for(m=0;m<240;m++) {
      Lcd_Write_Data_16(j);
    }
  }
  gpio_put(PIN::CS,HIGH);
}

int melody[] = {
  NOTE_C4, division::ronde,
  NOTE_G4, division::ronde,
  NOTE_C4, division::blanche,
  NOTE_G4, division::blanche,
  NOTE_F4, division::noire,
  NOTE_E4, division::noire,
  NOTE_D4, division::noire,
  NOTE_C4, division::noire,
  NOTE_G4, division::ronde,
  NOTE_C4, division::ronde,
  NOTE_G4, division::blanche,
  NOTE_C4, division::blanche,
  NOTE_G4, division::noire,
  NOTE_A4, division::noire,
  NOTE_B4, division::noire,
  NOTE_C5, division::noire,
  NOTE_G4, division::ronde,
};

void fusion( int notes[] , int durations[] , int newtab[], int size ) {
  
  for ( unsigned int i = 0 ; i < size ; i++ ) {
    newtab[2*i]   = notes[i];
    newtab[2*i+1] = durations[i];
  }

}

int main() {

	setup();

  // int melody[sizeof(notes) / sizeof(int)*2];

  // fusion(notes,noteDurations,melody, sizeof(notes) / sizeof(int));

	Tone myPlayer(5);
	myPlayer.init(TONE_NON_BLOCKING) ;

	
	while (true) {

    myPlayer.play_melody(400,sizeof(melody)/sizeof(int)/2,melody);

    // LCD_Clear(0xffff);
    // sleep_ms( 2 * 100);
    // LCD_Clear(0x0000);
    // sleep_ms( 2 * 100);
    // LCD_Clear(0xf800);
    // sleep_ms( 2 * 100);
    // LCD_Clear(0x07E0);
    // sleep_ms( 2 * 100);
    // LCD_Clear(0x001F);
    // sleep_ms( 2 * 100);

    drawPixel(60, 60, 0x5555);
    drawPixel(60, 61, 0x5555);
    drawPixel(60, 62, 0x5555);
    drawPixel(60, 120, 0x0000);
    drawPixel(60, 121, 0x0000);
    drawPixel(60, 122, 0x0000);


		// for (int i = 50; i < 300; i++) {
		// 	// drawPixel(i, 60, 0x2FA4);
		// 	// drawPixel(i, 70, 0x2FA4);
		// 	// drawPixel(i, 80, 0x2FA4);
		// 	// drawPixel(i, 90, 0x2FA4);
		// 	// drawPixel(i, 100, 0x2FA4);
		// 	// drawPixel(i, 110, 0x2FA4);
		// 	// drawPixel(i, 120, 0x2FA4);
		// 	// drawPixel(i, 130, 0x2FA4);
		// 	// drawPixel(i, 140, 0x2FA4);
		// 	// drawPixel(i, 150, 0x2FA4);


   	// 	}
	}
  return 0;
}