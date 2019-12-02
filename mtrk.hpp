#pragma once
#include <cstdint>
#include <vector>

class Mtrk;
class Mtrk_Event;
class Midi_Event;
class Meta_Event;
class Sys_Ex_Event;

class Mtrk {
	private:
		//check data types
		int chunk_type;
		int length;
		std::vector<Mtrk_Event> events;
	public:
		// Mtrk();
		Mtrk(uint8_t*& f);
	
};

class Mtrk_Event {
	private:

	public:

};


class Midi_Event : public Mtrk_Event {
	private:

	public:
		Midi_Event(uint8_t*& f);

};

class Meta_Event : public Mtrk_Event {
	private:
		
	public:
		Meta_Event(uint8_t*& f);
};


class Sys_Ex_Event : public Mtrk_Event {
	private:

	public:
		Sys_Ex_Event(uint8_t*& f);
};