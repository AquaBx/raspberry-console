#include "stdio.h"
#include "stdlib.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" 
#include "hardware/gpio.h"
#include <time.h>

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
    myPlayer.tone( (*notes_simon).data[i] );
    sleep_ms(100);
    myPlayer.stop();
  }
}



int main() {

  // int melody[sizeof(notes) / sizeof(int)*2];

  // fusion(notes,noteDurations,melody, sizeof(notes) / sizeof(int));

	Tone myPlayer(28);
	myPlayer.init(TONE_NON_BLOCKING) ;

  vector<int> notes_simon{};
  
  int btns[] = {11,10,9,8};
  
  for ( int i = 0 ; i < 4 ; i++ ){
    int btn = btns[i];
    
    gpio_init(btn);
    gpio_set_dir(btn, GPIO_IN);
    gpio_pull_up(btn);
  }

  srand( time( NULL ) );  

  int possibilites[] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4};

  bool cond = true;
  int j = 0;

	while (true) {

    if (cond) {
      notes_simon.reset();
      cond = false;
    }

    if ( j == notes_simon.size ) {
      int rng = rand() % 4;
      notes_simon.append(possibilites[rng]);
      playliste(myPlayer, &notes_simon);
      j=0;
    }

    int sel;
    bool clicked = false;

    for (int y=0;  y < 4 ; y++){
      if (!gpio_get(btns[y]) && !clicked){
        sel = possibilites[y];
        clicked=true;
      }
    }

    if (clicked){
      
      myPlayer.tone(sel);
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