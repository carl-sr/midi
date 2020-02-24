#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <time.h>

#include "./mtrk.hpp"

class Midi {
	private:
		MThd header_chunk;
		std::vector<std::shared_ptr<MTrk>> track_chunks;
		std::string title;

	public:
		void operator=(Midi &);
		void open(std::string);
		void write();
		void write(std::string);
		void info();
};


void Midi::operator=(Midi &m) {

}

void Midi::open(std::string f_name) {
	//start the random seed
	srand(time(NULL));
	// read midi file f_name into memory, populate objects

	std::ifstream f;
	f.open(f_name, std::ios::binary | std::ios::ate);
	if (!f.is_open())
	{
		std::cerr << "Error: Unable to open file " << f_name << std::endl;
		return;
	}

	// get size of file
	int f_size = f.tellg();
	f.seekg(std::fstream::beg);

	// read into file_b and close file
	u_int8_t *file_b = new u_int8_t[f_size];
	for (int i = 0; i < f_size; i++) {
		file_b[i] = f.get();
	}
	f.close();

	u_int8_t* file_b_pop = file_b;	// save the old data pointer for deleting array
	header_chunk = MThd(file_b_pop);

	while(file_b_pop - file_b < f_size) {
		track_chunks.push_back(std::make_shared<MTrk>(MTrk(file_b_pop)));
	}


	// delete file byte array after objects are populated
	delete[] file_b;
}

void Midi::write() {
	write("midi_" + std::to_string(rand()%10000));
}

void Midi::write(std::string s) {
	std::cout << s << std::endl;
	std::fstream f;

	// f.open(s + ".mid", std::ios::binary | std::ios::trunc | std::ios::out);
	f.open("output.mid", std::ios::binary | std::ios::trunc | std::ios::out);

	header_chunk.write(f);

	for(auto i = track_chunks.begin(); i != track_chunks.end(); i++) {
		i->get()->write(f);
	}
}

void Midi::info() {
	printf("Midi file info:\n");
	printf("\nMThd:\n");
	printf("=================================\n");
	header_chunk.print_info();
	
	if(track_chunks.size()) {
		printf("\nMTrk chunks:\n");
		printf("=================================\n");
		for(auto i = track_chunks.begin(); i != track_chunks.end(); i++) {
			i->get()->print_info();
		}
	}

}

