#include "./mtrk.hpp"


void print_hex(u_int8_t a) {
	(a > 0xf) ? printf("0x%x", a) : printf("0x0%x", a);
}


Bit_pattern::Bit_pattern(long d=0) {
	data = d;
}

bool Bit_pattern::operator[](int p) {
		long offset {0b1};
		offset = offset << p;
		return data & offset;
}

bool Bit_pattern::get() {
	bool res = operator[](pos);
	pos++;
	return res;
}

int Bit_pattern::bit_length() {
	for(int i = sizeof(data)*8-1; i >= 0; i--) {
		if(operator[](i)) {
			return i+1;
		}
	}
	return 0;
}

int Bit_pattern::position() {
	return pos;
}


unsigned int vlen_to_int(File_bytes& v) {
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

void int_to_vlen(long v, std::fstream& f) {
	if(v == 0) {
		f << (u_int8_t)0;
		return;
	}

	Bit_pattern bits(v);
	std::vector<u_int8_t> data;


	while(bits.position() < bits.bit_length()) {
		u_int8_t x {0};
		for(int i = 0; i < 7; i++) {
			if(bits.get()) {
				x = x | (0b1 << i);
			}
		}
		data.push_back(x);
	}

	while(!data.empty()) {
		u_int8_t d = data.back();
		data.pop_back();

		if(!data.empty()) {
			d = d | 0b10000000; 
		}
		f << d;
	}
	
}


// MThd===================================================================================

MThd::MThd() {
	chunk_type = 0x4d546864;
	length = 0x00000006;
	format = 0001;
	ntrks = 0001;
	division = 0001;	//change this

}

MThd::MThd(File_bytes& f) {
	u_int8_t* s = reinterpret_cast<u_int8_t*>(this);
	for(int i = 0; i < 14; i++) {
		s[i] = f[i];
	}

	// convert endianness
	chunk_type = be32toh(chunk_type);
	length = be32toh(length);
	format = be16toh(format);
	ntrks = be16toh(ntrks);
	division = be16toh(division);

	if(chunk_type != 0x4d546864) {
		throw std::runtime_error("Unrecognized file type");
	}

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

void MThd::tree() {
	std::cout << "Length: " << length << std::endl;
	std::cout << "Format: " << format << std::endl;
	std::cout << "Track Count: " << ntrks << std::endl;
	std::cout << "Division: " << division << std::endl;
}

void MThd::write(std::fstream& f) {
	u_int32_t end_chunk_type = be32toh(chunk_type);
	f.write(reinterpret_cast<const char*>(&end_chunk_type), 4);

	u_int32_t end_length = be32toh(length);
	f.write(reinterpret_cast<const char*>(&end_length), 4);

	u_int16_t end_format = be16toh(format);
	f.write(reinterpret_cast<const char*>(&end_format), 2);

	u_int16_t end_ntrks = be16toh(ntrks);
	f.write(reinterpret_cast<const char*>(&end_ntrks), 2);

	u_int16_t end_division = be16toh(division);
	f.write(reinterpret_cast<const char*>(&end_division), 2);

}

// MTrk===================================================================================

MTrk::MTrk(File_bytes& f) {
	u_int8_t* s = reinterpret_cast<u_int8_t*>(this);
	for(int i = 0; i < 8; i++) {
		s[i] = f[i];
	}

	// convert endianness
	chunk_type = be32toh(chunk_type);
	length = be32toh(length);

	f += 8;

	u_int8_t running_status {0x90};
	while(!f.is_end()) {
		// find the variable delta time first
		int v = vlen_to_int(f);

		if(*f == 0xff) {
			// meta event
			f += 1;
			track_events.push_back(std::make_shared<Meta_Event>(Meta_Event(f, v)));
		}
		else if (*f == 0xf0) {
			// sys ex event
			f += 1;
			track_events.push_back(std::make_shared<Sys_Ex_Event>(Sys_Ex_Event(f, v)));
		}
		else {
			// midi event - detect running status events
			if(f[0] >= 0x80 && f[0] <= 0xff) {
				running_status = f[0];
			}
			track_events.push_back(std::make_shared<Midi_Event>(Midi_Event(f, v, running_status)));
		}
	}
}

void MTrk::print_info() {
	for(auto i = track_events.begin(); i != track_events.end(); i++) {
		i->get()->print_info();
		printf("\n");
	}
}

void MTrk::tree(bool last, bool nested) {
	int chunk_count {0};
	if(track_events.empty()) {
		std::cout << "└── (No track events)" << std::endl;
	}
	else {
		for(auto i = track_events.begin(); i != track_events.end(); i++) {
			if(nested) {
				if(last) {
					(i+1 == track_events.end()) ? std::cout << "    └── " : std::cout << "    ├── ";
				}
				else {
					(i+1 == track_events.end()) ? std::cout << "│   └── " : std::cout << "│   ├── ";
				}
			}
			else {
				(i+1 == track_events.end()) ? std::cout << "└── " : std::cout << "├── ";
			}
			if(chunk_count < 10) {
				std::cout << "000" << chunk_count;
			}
			else if(chunk_count < 100) {
				std::cout << "00" << chunk_count;
			}
			else if(chunk_count < 1000) {
				std::cout << "0" << chunk_count;
			}
			else {
				std::cout << chunk_count;
			}
			std::cout << " ";
			i->get()->tree();
			std::cout << std::endl;
			++chunk_count;
		}
	}
}

void MTrk::write(std::fstream& f) {
	u_int32_t end_chunk_type = be32toh(chunk_type);
	f.write(reinterpret_cast<const char*>(&end_chunk_type), 4);

	// this needs to be calculated at some point
	u_int32_t end_length = be32toh(length);
	f.write(reinterpret_cast<const char*>(&end_length), 4);

	for(auto i = track_events.begin(); i != track_events.end(); i++) {
		i->get()->write(f);
	}

}

// Midi_Event ===================================================================================

Midi_Event::Midi_Event(File_bytes& f, int v, u_int8_t running_status) {
	delta_time = v;
	if(f[0] >= 0x80 && f[0] <= 0xff) {
		// this is a valid midi event
		function = f[0];
		fb = f[1];
		sb = f[2];
	}
	else {
		// this should use running status
		function = running_status;
		fb = f[0];
		sb = f[1];
		f--;
	}

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
}

void Midi_Event::print_info() {
	std::cout << event_type() << std::endl;
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

void Midi_Event::tree() {
	std::cout << "(" << event_type() << ") ";
	printf("delta time: ");
	print_hex(delta_time);
	printf(", ");

	printf("[");
	print_hex(function);
	printf(", ");
	print_hex(fb);
	printf(", ");
	print_hex(sb);
	printf("] ");

	std::cout << Note_Help::midi_event_function(function) << " ";
	std::cout << Note_Help::midi_event_fb(function, fb) << " ";
	std::cout << Note_Help::midi_event_sb(function, sb) << " ";
}

void Midi_Event::write(std::fstream& f) {
	int_to_vlen(delta_time, f);
	f << function;
	if(function == 0xf1) {
		return;
	}
	if(function >= 0xf4 && function <= 0xff) {
		return;
	}
	if(function >= 0xc0 && function <= 0xdf) {
		f << fb;
	}
	else if(function == 0xf3) {
		f << fb;
	}
	else {
		f << fb;
		f << sb;
	}
}

void Midi_Event::set_function(u_int8_t f) {
	if(f != 0xf7) {
		function = f;
	}
}

void Midi_Event::set_first_byte(u_int8_t b) {
	fb = b;
}

void Midi_Event::set_second_byte(u_int8_t b) {
	sb = b;
}

// Meta_Event ===================================================================================

Meta_Event::Meta_Event(File_bytes& f, int v) {
	delta_time = v;
	type = *f;
	f += 1;
	int data_length = vlen_to_int(f);
	while(data_length--) {
		data.push_back(*f);
		f++;
	}
}

void Meta_Event::print_info() {
	std::cout << event_type() << std::endl;
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

void Meta_Event::tree() {
	std::cout << "(" << event_type() << ") ";
	printf("delta time: ");
	print_hex(delta_time);
	printf(", ");

	printf("type: ");
	print_hex(type);
	printf(", ");

	printf("data: ");
	if(!data.empty()) {
		printf("0x");
		for(auto i = data.begin(); i != data.end(); i++) {
			(*i > 0xf) ? printf("%x", *i) : printf("0%x", *i);
		}
		printf(" %%c:(");
		for(auto i = data.begin(); i != data.end(); i++) {
			if(*i >= 32 && *i <= 126) {
				printf("%c", *i);
			}
		}
		printf(")");
	}
	else {
		printf("(empty)");
	}
}

void Meta_Event::write(std::fstream& f) {
	int_to_vlen(delta_time, f);
	f << static_cast<u_int8_t>(0xff);
	f << type;
	int_to_vlen(data.size(), f);
	for(auto i = data.begin(); i != data.end(); i++) {
		f << *i;
	}
}

void Meta_Event::set_type(u_int8_t t) {
	type = t;
}

void Meta_Event::set_data(std::vector<u_int8_t>& v) {
	clear_data();
	add_data(v);
}

void Meta_Event::add_data(std::vector<u_int8_t>& v) {
	for(auto i = v.begin(); i != v.end(); i++) {
		data.push_back(*i);
	}
}


// Sys_Ex_Event ===================================================================================

Sys_Ex_Event::Sys_Ex_Event(File_bytes& f, int v) {
	delta_time = v;
	int data_length = vlen_to_int(f);

	while(data_length--) {
		data.push_back(*f);
		f++;
	}

}

void Sys_Ex_Event::print_info() {
	std::cout << event_type() << std::endl;
	printf("-------------------------\n");
	printf("| d_time     | %8x |\n", delta_time);
	printf("-------------------------\n");
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

void Sys_Ex_Event::tree() {
	std::cout << "(" << event_type() << ") ";
	printf("delta time: ");
	print_hex(delta_time);
	printf(", ");

	printf("data: ");
	if(!data.empty()) {
		printf("0x");
		for(auto i = data.begin(); i != data.end(); i++) {
			(*i > 0xf) ? printf("%x", *i) : printf("0%x", *i);
		}
		printf(" %%c:(");
		for(auto i = data.begin(); i != data.end(); i++) {
			if(*i >= 32 && *i <= 126) {
				printf("%c", *i);
			}
		}
		printf(")");
	}
	else {
		printf("(empty)");
	}
}

void Sys_Ex_Event::write(std::fstream& f) {
	int_to_vlen(delta_time, f);
	f << static_cast<u_int8_t>(0xf0);
	int_to_vlen(data.size(), f);
	for(auto i = data.begin(); i != data.end(); i++) {
		f << *i;
	}
}

void Sys_Ex_Event::set_data(std::vector<u_int8_t>& v) {
	clear_data();
	add_data(v);
}

void Sys_Ex_Event::add_data(std::vector<u_int8_t>& v) {
	for(auto i = v.begin(); i != v.end(); i++) {
		data.push_back(*i);
	}
}