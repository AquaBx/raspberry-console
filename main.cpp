#include "stdio.h"
#include "stdlib.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" 
#include "hardware/gpio.h"
#include <time.h>

#include "pico_tone.hpp"
#include "driver.hpp"

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

int btns[] = {
  0, // haut    violet
  1, // bas     vert
  2, // droite  orange
  3, // gauche  rose
  4, // b
  5, // a
};

int btns_color[] = {
  0xc59a, // haut    violet
  0xb7f0, // bas     vert
  0xfd44, // droite  orange
  0xed3c, // gauche  rose
};

int possibilites[] = {
  NOTE_C4,
  NOTE_D4,
  NOTE_E4,
  NOTE_F4
};


template<typename T>
struct vector {

  T* data = nullptr;
  unsigned int capacity = 0, size = 0;

  vector()
    : data(new T[20]), capacity(20), size(0)
  {}

  ~vector() {
    if(this->data)
      delete this->data;

    this->data = nullptr;
    this->size = 0;
    this->capacity = 0;
  }

  void append(T element) {
    if(this->size >= this->capacity) {
      T* old = this->data;
      this->capacity += 10;
      this->data = new T[this->capacity];
      for(unsigned int i = 0; i < this->size; i++) {
        this->data[i] = old[i];
      }
      delete old;
    }
    this->data[this->size] = element;
    this->size++;
  }

  void reset() {
    this->size = 0;
  }

};


void playliste(Tone myPlayer, vector<int> * notes_simon){
  for (int i=0;i<(*notes_simon).size;i++){
    int y = (*notes_simon).data[i];

    myPlayer.tone( possibilites[y] );

    ili9341::clear( btns_color[y] );
    ili9341::draw_buffer();

    sleep_ms(100);
    myPlayer.stop();

    renderer::set_clear_color(uint16_t c);


    ili9341::clear();
    ili9341::draw_buffer();

    sleep_ms(10);
  }
}



int main() {

  ili9341::initialize();

	Tone myPlayer(28);
	myPlayer.init(TONE_NON_BLOCKING) ;

  vector<int> notes_simon{};
    
  for ( int i = 0 ; i < 4 ; i++ ){
    int btn = btns[i];
    
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_pull_up(btn);
  }

  srand( time( NULL ) );  


  bool cond = true;
  int j = 0;

	while (true) {

    if (cond) {
      notes_simon.reset();
      cond = false;
    }

    if ( j == notes_simon.size ) {
      notes_simon.append( rand() % 4 );
      playliste(myPlayer, &notes_simon);
      j=0;
    }

    int sel;
    bool clicked = false;

    for (int y=0;  y < 4 ; y++){
      if (!gpio_get(btns[y]) && !clicked){
        sel = y;
        clicked=true;
      }
    }

    if (clicked){
      
      myPlayer.tone( possibilites[sel] );
      sleep_ms(100);
      myPlayer.stop();
      sleep_ms(100);

      if ( sel == notes_simon.data[j] ){
        sleep_ms(1000);
        j++;
      }
      else {

        myPlayer.tone(NOTE_D1);
        sleep_ms(100);
        myPlayer.stop();

        myPlayer.tone(NOTE_D1);
        sleep_ms(100);
        myPlayer.stop();

        sleep_ms(1000);
        
        j=0;
        cond=true;
      }
    }

	}
  return 0;
}