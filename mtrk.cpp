#include "./mtrk.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <memory>

unsigned int vlen_to_int(uint8_t*& v) {
	// convert a variable length string of bytes into an integer.
	// last byte in variable length byte string has a msb 1
	// all other bytes have msb 0
	unsigned int add {0};
	int set {1};
	while(*v & 0x80) {
		set += 1;
		v += 1;
	}
	for(int i = set; i > 0; i--) {
		int a = ((*v & 0x7f) << 7*(set-i));
		add += a;
		v -= 1;
	}

	//set the pointer to the byte after the vlen
	v += set + 1;

	return add;
}



// Mtrk===================================================================================
Mtrk::Mtrk(uint8_t *&f) {
	// printf("Entering MTrk constructor value of f: %x\n", *f);

	std::cout << "STARTING AN MTRK SEGMENT: " << f[0] << f[1] << f[2] << f[3] << std::endl;
	// construct MTrk based on raw byte data
	uint8_t* base = f;
	if (f[0] != 'M' || f[1] != 'T' || f[2] != 'r' || f[3] != 'k') {
		std::cerr << "\t\tnot an mtrk" << std::endl;
		return;
	}
	f += 4; //jump past 'MTrk'

	length = (int)(f[0] << 24 | f[1] << 16 | f[2] << 8 | f[3]);
	f += 4; //jump past length
	base = f;

	std::cout << "Length: " << length << std::endl;

	// for(int i = 0; i < length; i++) {
	// 	(f[i] > 0x0f) ? printf("%x ", f[i]) : printf("0%x ", f[i]);
	// 	if((i+1)%10 == 0) {
	// 		printf("\n");
	// 	}
	// }
	std::cout << std::endl;

	while (f < (base + length)) {
		// int type = (int)(f[0] << 8 | f[1]);
		// printf("Starting f value: %p(%d)\n", f, *f);
		// printf("++++++++%x,%x,%x,%x\n", f[0], f[1], f[2], f[3]);
		// std::cin.get();
		// printf("\tCreating MTrk at value of f: %x\n", *f);
		printf("%x ", *f);

		int type = static_cast<int>(*f);
		if(type == 0x00) {
			f++;
			continue;
		}
		if (type >= 0x80 && type <= 0xef) {
			// Midi events (status bytes 0x8n - 0xEn)
			events.push_back(std::make_shared<Midi_Event>(Midi_Event(f)));
		}
		else if (type == 0xf0 || type == 0xf7) {
			// SysEx events (status bytes 0xF0 and 0xF7)
			events.push_back(std::make_shared<Sys_Ex_Event>(Sys_Ex_Event(f)));
		}
		else if (type == 0xff) {
			// Meta events (status byte 0xFF)
			events.push_back(std::make_shared<Meta_Event>(Meta_Event(f)));
		}
		else {
			printf("--------Unkown byte: %x\n", *f);
			f++;
		}
		// printf("\tEnding creation of MTrk at value of f: %x\n", *f);

	}
	// printf("Ending MTrk constructor value of f: %x\n", *f);
}

void Mtrk::print_info() {
	std::cout << "\tChunk Type: " << chunk_type << std::endl;
	std::cout << "\tLength: " << length << std::endl;
	for(auto i = events.begin(); i != events.end(); i++) {
		std::cout << (*i)->event_type() << std::endl;
	}
}



// Midi_Event ===================================================================================

Midi_Event::Midi_Event(uint8_t*& f) {
	channel =  *f & 0b00001111;
	event_function =*f >> 4;
	f += 1;
	fb = *f;
	f += 1;
	sb += *f;
	gap = vlen_to_int(f);
	f += 1;
	


}


std::string Midi_Event::event_type() {
	std::string s = "Midi Event: Gap: ";
	s += std::to_string(gap);
	s += " channel: ";
	s += std::to_string(channel);
	s += " function: ";
	s += std::to_string(event_function);
	s += " fb: ";
	s += std::to_string(fb);
	s += " sb: ";
	s += std::to_string(sb);
	return s;
}


// Meta_Event ===================================================================================

Meta_Event::Meta_Event(uint8_t*& f) {
	f += 1;
	type = *f;
	f += 1;
	length = vlen_to_int(f);
	for(int i = 0; i < length; i++) {
		data.push_back(*f++);
	}
	if(length > 0) {
		f += 1;
	}
}


std::string Meta_Event::event_type() {
	std::string s = "Meta Event";
	s += " type: ";
	s += std::to_string(type);
	s += " length: ",
	s += std::to_string(length);
	s += " Data: ";
	for(auto i = data.begin(); i != data.end(); i++) {
		s += *i;
	}
	return s;
}

// Sys_Ex_Event ===================================================================================

Sys_Ex_Event::Sys_Ex_Event(uint8_t*& f) {
	length = vlen_to_int(++f);
	for(int i = 0; i < length; i++) {
		data.push_back(*f++);
	}
}

std::string Sys_Ex_Event::event_type() {
	std::string s = "Sys Ex Event Length: ";
	s += length;
	s += " Data: ";
	for(auto i = data.begin(); i != data.end(); i++) {
		s += *i;
	}
	return s;
}


















