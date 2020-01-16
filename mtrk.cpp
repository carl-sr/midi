#include "./mtrk.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>

unsigned int vlen_to_int(u_int8_t*& v) {
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


// MThd===================================================================================

MThd::MThd() {
	chunk_type = 0x4d546864;
	length = 0x00000006;
	format = 0001;
	ntrks = 0001;
	division = 0001;	//change this

}

MThd::MThd(u_int8_t *&f) {
	std::memcpy(this, f, 14);

	// convert endianness
	chunk_type = be32toh(chunk_type);
	length = be32toh(length);
	format = be16toh(format);
	ntrks = be16toh(ntrks);
	division = be16toh(division);

	f += 14;
}

void MThd::print_info() {
	printf("-------------------------\n");
	printf("| chunk_type | %8x |\n", chunk_type);
	printf("-------------------------\n");
	printf("| length     | %8x |\n", length);
	printf("-------------------------\n");
	printf("| format     | %8x |\n", format);
	printf("-------------------------\n");
	printf("| ntrks      | %8x |\n", ntrks);
	printf("-------------------------\n");
	printf("| division   | %8x |\n", division);
	printf("-------------------------\n");
}

// MTrk===================================================================================

MTrk::MTrk(u_int8_t *&f) {
	std::memcpy(this, f, 8);

	// convert endianness
	chunk_type = be32toh(chunk_type);
	length = be32toh(length);

	f += 8;
	u_int8_t* base = f;
	while(f - base < length) {
		// find the variable delta time first
		int v = vlen_to_int(f);

		if(*f == 0xff) {
			// meta event
			f += 1;
			// printf("pushing meta event...\n");
			track_events.push_back(std::make_shared<Meta_Event>(Meta_Event(f, v)));
		}
		else if (*f == 0xf0) {
			// sys ex event
			f += 1;
			// printf("pushing sys ex event...\n");
			track_events.push_back(std::make_shared<Sys_Ex_Event>(Sys_Ex_Event(f, v)));
		}
		else {
			// midi event
			// printf("pushing midi event...\n");
			track_events.push_back(std::make_shared<Midi_Event>(Midi_Event(f, v)));
		}
	}
}

void MTrk::print_info() {
	for(auto i = track_events.begin(); i != track_events.end(); i++) {
		i->get()->print_info();
		printf("\n");
	}
}

// Midi_Event ===================================================================================

Midi_Event::Midi_Event(u_int8_t*& f, int v) {
	delta_time = v;
	std::memcpy(this, f, 3);

	if(function >= 0xc0 && function <= 0xdf) {
		// two bytes
		f += 2;
	}
	else if(function == 0xf3) {
		// two bytes
		f += 2;
	}
	else if(function >= 0xf4 && function <= 0xff) {
		// one byte
		f += 1;
	}
	else if(function == 0xf1) {
		// one byte
		f += 1;
	}
	else {
		f += 3;
	}

	// print_info();
}

void Midi_Event::print_info() {
	printf("Midi Event:\n");
	printf("-------------------------\n");
	printf("| d_time     | %8x |\n", delta_time);
	printf("-------------------------\n");
	printf("| function   | %8x |\n", function);
	printf("-------------------------\n");
	printf("| 1st byte   | %8x |\n", fb);
	printf("-------------------------\n");
	printf("| 2nd byte   | %8x |\n", sb);
	printf("-------------------------\n");
}


// Meta_Event ===================================================================================

Meta_Event::Meta_Event(u_int8_t*& f, int v) {
	delta_time = v;
	type = *f;
	f += 1;
	int data_length = vlen_to_int(f);
	while(data_length--) {
		data.push_back(*f++);
	}
}

void Meta_Event::print_info() {
	printf("Meta Event:\n");
	printf("-------------------------\n");
	printf("| d_time     | %8x |\n", delta_time);
	printf("-------------------------\n");
	printf("| type       | %8x |\n", type);
	if(data.size()) {
		printf("-------------------------\n");
		printf("| data:                 |\n");
		for(auto i = data.begin(); i != data.end(); i++) {
			if(*i >= 32 && *i <= 126) {
				printf("|    %8x (%c)       |\n", *i, *i);
			}
			else {
				printf("|    %8x           |\n", *i);
			}
		}
	}
	printf("-------------------------\n");

}

// Sys_Ex_Event ===================================================================================

Sys_Ex_Event::Sys_Ex_Event(u_int8_t*& f, int v) {
	delta_time = v;
	int data_length = vlen_to_int(f);
	
	while(data_length--) {
		data.push_back(*f++);
	}

}

void Sys_Ex_Event::print_info() {
	printf("Sys Ex Event:\n");
	printf("-------------------------\n");
	printf("| d_time     | %8x |\n", delta_time);
	printf("-------------------------\n");

}

