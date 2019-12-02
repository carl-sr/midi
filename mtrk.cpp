#include "mtrk.hpp"
#include <cstdint>
#include <iostream>

Mtrk::Mtrk(uint8_t *&f) {
	// construct MTrk based on raw byte data
	uint8_t* base = f;
	if (f[0] != 'M' || f[1] != 'T' || f[2] != 'r' || f[3] != 'k')
	{
		std::cout << "not an mtrk" << std::endl;
		return;
	}
	f += 4; //jump past 'MTrk'

	length = (int)(f[0] << 24 | f[1] << 16 | f[2] << 8 | f[3]);
	f += 4; //jump past length

	while (f < base + length)
	{
		int type = (int)(f[0] << 8 | f[1]);
		f += 4; //jump past type
		if (type == 0x00ff)
		{
			Meta_Event m = Meta_Event(f);
		}
	}
}

Midi_Event::Midi_Event(uint8_t*& f) {

}

Meta_Event::Meta_Event(uint8_t*& f) {

}

Sys_Ex_Event::Sys_Ex_Event(uint8_t*& f) {

}
















unsigned int vlen_to_int(uint8_t*& v) {
	// convert a variable length string of bytes into an integer.
	// last byte in variable length byte string has a msb != 0
	// all other bytes have msb != 1
	// msb is ignored in building final value
	// return 0 on error - no lengths of 0 should occur under normal circumstances

	unsigned long r {0};
	int set {1};

	// v_length should not be greater than 4 bytes - maybe can
	for(; (*v & 0b10000000); set++) {
		v++;
		if(set > 4) {
			return 0;
		}
	}

	for(int j {0}; j < set; j++) {
		int add {*v};
		v--;
		add = ((add & 0b01111111) << (j*7));
		r += add;
	}
	// set pointer to next element after vlen
	v += set;
	return r;
}

