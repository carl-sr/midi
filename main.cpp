#include <iostream>
#include <ncurses.h>

#include "./midi.hpp"
#include "./controller.hpp"


int main(int argc, char* argv[]) {
	Midi m;
	m.open("mario_theme.mid");

	return 0;
}
