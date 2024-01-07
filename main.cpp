#include "stdio.h"
#include "stdlib.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" 
#include "hardware/gpio.h"
#include <time.h>

#include "pico_tone.hpp"
#include "driver.hpp"
#include "controller.hpp"

struct button
{
	/* data */
	controller::button id;
	color c;
	int x;
	int y;
	int note;

	void play(Tone player){
		ili9341::fill_rect( this -> x, this -> y,60,60, this -> c );
		player.tone( this -> note );
		sleep_ms(100);
		player.stop();
		ili9341::fill_rect( this -> x, this -> y,60,60,0 );
	}

};

button buttons[] = {
	[0] = button{controller::button::up   , color::violet  , 130 , 15  , NOTE_C4},
	[1] = button{controller::button::down , color::green   , 130 , 165 , NOTE_D4},
	[2] = button{controller::button::right, color::gold    , 205 , 90  , NOTE_E4},
	[3] = button{controller::button::left , color::fuchsia , 55  , 90  , NOTE_F4},
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
		buttons[y].play(myPlayer);
		sleep_ms(10);
	}
}

void play_game_over(Tone myPlayer) {
	for ( int i; i<2;i++){
		myPlayer.tone(NOTE_D1);
		sleep_ms(100);
		myPlayer.stop();
	}
}

int main() {

	ili9341::initialize();
	controller::initialize();
	
	Tone myPlayer(28);
	myPlayer.init(TONE_NON_BLOCKING) ;

	vector<int> notes_simon{};
	ili9341::clear(color::black);
		
	
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

		for (int y=0; y < 4 ; y++){
			if (controller::is_pressed(buttons[y].id) && !clicked){
				sel = y;
				clicked=true;
			}
		}

		if (clicked){
		
			buttons[sel].play(myPlayer);

			sleep_ms(100);

			if ( sel == notes_simon.data[j] ){
				j++;
			}
			else {
				play_game_over(myPlayer);
				j=0;
				cond=true;
			}

			sleep_ms(100);

		}

	}
	return 0;
}