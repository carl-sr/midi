#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cstring>
#include <fstream>

#include <endian.h>

#include "./note_help.hpp"

class MTrk;
class MTrk_Event;
class Midi_Event;
class Meta_Event;
class Sys_Ex_Event;

void print_hex(u_int8_t);

class Bit_pattern {
	private:
		long data;
		int pos {0};
	public:
		Bit_pattern(long);
		bool operator[](int);
		bool get();
		int bit_length();
		int position();
};

unsigned int vlen_to_int(u_int8_t*&);
void int_to_vlen(long, std::fstream&);

#pragma pack(2)

class MThd {
	private:
		u_int32_t chunk_type;
		u_int32_t length;
		u_int16_t format;
		u_int16_t ntrks;
		u_int16_t division;
	public:
		MThd();
		MThd(u_int8_t *&f);
		void write(std::fstream&);

		void print_info();
		void tree();
};

class MTrk {
	private:
		u_int32_t chunk_type;
		u_int32_t length;

		std::vector<std::shared_ptr<MTrk_Event>> track_events;
	public:
		MTrk(u_int8_t*& f);
		void write(std::fstream&);

		void print_info();
		void tree(bool, bool);
};

class MTrk_Event {
	protected:
		int delta_time {0};
	public:
		virtual ~MTrk_Event() {};
		virtual std::string event_type()=0;
		virtual void print_info()=0;
		virtual void tree()=0;
		virtual void write(std::fstream&)=0;
};


class Midi_Event : public MTrk_Event {
	private:
		u_int8_t function;
		u_int8_t fb;
		u_int8_t sb;
	public:
		Midi_Event(u_int8_t a=0x90, u_int8_t b=0x80, u_int8_t c=0x80): function(a), fb(b), sb(c) {};
		Midi_Event(u_int8_t*& f, int);
		~Midi_Event() {};
		void write(std::fstream&);

		std::string event_type() {return "Midi Event";};
		void print_info();
		void tree();

		u_int8_t func() {return function;};
		u_int8_t f_byte() {return fb;};
		u_int8_t s_byte() {return sb;};

		u_int8_t pitch() {return fb;};
		u_int8_t velocity() {return sb;};
		u_int8_t channel() {return function >> 4;};

		// general
		void set_function(u_int8_t);
		void set_first_byte(u_int8_t);
		void set_second_byte(u_int8_t);


};

class Meta_Event : public MTrk_Event {
	private:
		u_int8_t type;
		std::vector<u_int8_t> data;
	public:
		Meta_Event(u_int8_t a=0x01): type(a) {};
		Meta_Event(u_int8_t*& f, int);
		~Meta_Event() {};

		void write(std::fstream&);

		// get
		std::string event_type() {return "Meta Event";};
		void print_info();
		void tree();

		// set
		void set_type(u_int8_t);
		void set_data(std::vector<u_int8_t>&);
		void add_data(std::vector<u_int8_t>&);
		void clear_data() {data.clear();};
		

};


class Sys_Ex_Event : public MTrk_Event {
	private:
		std::vector<u_int8_t> data;
	public:
		Sys_Ex_Event(u_int8_t*& f, int);
		~Sys_Ex_Event() {};

		void write(std::fstream&);

		// get
		std::string event_type() {return "Sys Ex Event";};
		void print_info();
		void tree();

		// set
		void set_data(std::vector<u_int8_t>&);
		void add_data(std::vector<u_int8_t>&);
		void clear_data() {data.clear();};
};

#pragma pack()