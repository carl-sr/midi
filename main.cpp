#include <iostream>
#include <ncurses.h>
#include <assert.h>

#include "./midi.hpp"
#include "./controller.hpp"




int main(int argc, char* argv[]) {
	Midi m;
	m.open(argv[1]);
 
	m.info();

	// uint8_t* t = new uint8_t[4];
	// uint8_t* f = t;
	// t[0] = 0x00;
	// t[1] = 'a';
	// t[2] = 'b';
	// t[3] = 'c';
	
	// std::cout << xvlen_to_int(t) << std::endl;

	// std::cout << "t-f: " << t-f << " *t: " << *t << std::endl;

	return 0;
}
