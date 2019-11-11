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
		Mtrk(uint8_t*& f) {
			// construct MTrk based on raw byte data
		}
	
};

class Mtrk_Event {
	private:
		int delta_time {0};	//read up

	public:
		int time() {return delta_time;}
		void set_time(int x)  {delta_time = x;}
		virtual void c() {};
};


class Midi_Event : public Mtrk_Event {
	private:
		uint8_t function {0};
		uint8_t a {0};
		uint8_t b {0};
	public:
		int type() {return 1;}

};

class Meta_Event : public Mtrk_Event {
	private:
		
	public:
		int type() {return 2;}
};

class Sys_Ex_Event : public Mtrk_Event {
	private:

	public:
		int type() {return 3;}
};


unsigned int vlen_to_int(uint8_t*& v) {
	// convert a variable length string of bytes into an integer.
	// last byte in variable length byte string has a msb == 0
	// all other bytes have msb == 1
	// msb is ignored in building final value
	// return 0 on error - no lengths of 0 should occur under normal circumstances

	unsigned long r {0};
	int set {1};

	// v_length should not be greater than 4 bytes
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

