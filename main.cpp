#include <iostream>
#include <assert.h>

#include "./midi.hpp"
#include "./controller.hpp"




int main(int argc, char* argv[]) {
	Midi m;
	m.open(argv[1]);

	// m.info();
	m.tree(1);
	m.write();
	return 0;
}