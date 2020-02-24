#include <iostream>
#include <ncurses.h>
#include <assert.h>

#include "./midi.hpp"
#include "./controller.hpp"




int main(int argc, char* argv[]) {
	// std::fstream f("file.hex", std::fstream::out | std::fstream::trunc);
	// int_to_vlen(0, f);
	// return 0;

	Midi m;
	m.open(argv[1]);

	m.info();
	m.write();
	return 0;
}
