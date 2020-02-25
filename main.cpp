#include <iostream>
#include <ncurses.h>
#include <assert.h>

#include "./midi.hpp"
#include "./controller.hpp"




int main(int argc, char* argv[]) {
	Midi m;
	m.open(argv[1]);

	// m.info();
	m.tree();
	m.write();
	return 0;
}