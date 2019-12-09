#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <memory>

class Mtrk;
class Mtrk_Event;
class Midi_Event;
class Meta_Event;
class Sys_Ex_Event;

unsigned int vlen_to_int(uint8_t*&);


class Mtrk {
	private:
		//check data types
		int chunk_type;
		int length;
		std::vector<std::shared_ptr<Mtrk_Event>> events;
		// std::vector<int> events;
	public:
		// Mtrk();
		Mtrk(uint8_t*& f);
		void print_info();
	
};

class Mtrk_Event {
	private:

	public:
		virtual ~Mtrk_Event() {};
		virtual std::string event_type()=0;
};


class Midi_Event : public Mtrk_Event {
	private:
		int channel;
		int event_function;
		uint8_t fb {0};
		uint8_t sb {0};

		int gap;

	public:
		Midi_Event(uint8_t*& f);
		~Midi_Event() {};
		std::string event_type();

};

class Meta_Event : public Mtrk_Event {
	private:
		uint8_t type;
		unsigned int length;
		std::vector<uint8_t> data;
	public:
		Meta_Event(uint8_t*& f);
		~Meta_Event() {};
		std::string event_type();
};


class Sys_Ex_Event : public Mtrk_Event {
	private:
		unsigned int length;
		std::vector<uint8_t> data;

	public:
		Sys_Ex_Event(uint8_t*& f);
		~Sys_Ex_Event() {};
		std::string event_type();
};




