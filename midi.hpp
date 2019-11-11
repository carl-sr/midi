#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include "./mtrk.hpp"

class Midi {
	private:
		std::string filename;
		
		//header
		uint16_t format {0};
		uint16_t tracks {0};
		uint16_t division {0};	//this is supposed to be signed

		//mtrk
		std::vector<Mtrk> mtrk;

		//random functions
		void switch_endianness(uint16_t& f) {
			f = ((f << 8) | (f >> 8));
		}
		void switch_endianness(int16_t& f) {
			f = ((f << 8) | (f >> 8));
		}
	public:
		void open(std::string fname) {
			// read midi file fname into memory, populate objects

			std::ifstream f;
			f.open(fname, std::ios::binary | std::ios::ate);
			if(!f.is_open()) {
				std::cerr << "Error: Unable to open file " << fname << std::endl;
				return;
			}

			int f_size = f.tellg();
			f.seekg(std::fstream::beg);

			//save the beginning of the bytes with const
			const uint8_t* file_m = new uint8_t[f_size];
			uint8_t* file = const_cast<uint8_t*>(file_m);

			for(int i = 0; i < f_size; i++) {
				file[i] = f.get();
			}
			f.close();

			format = reinterpret_cast<uint16_t*>(file)[4];
			tracks = reinterpret_cast<uint16_t*>(file)[5];
			division = reinterpret_cast<uint16_t*>(file)[6];
			switch_endianness(format);
			switch_endianness(tracks);
			switch_endianness(division);

			//skip past the header information
			file += 14;

			for(int i = 0; i < tracks; i++) {
				mtrk.push_back(Mtrk(file));
			}
			std::cout << *file << std::endl;

			delete[] file_m;
		}

};

