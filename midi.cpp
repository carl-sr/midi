#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <time.h>

#include "./mtrk.hpp"
#include "./midi.hpp"
#include "./note_help.hpp"

void Midi::operator=(Midi &m) {

}

void Midi::open(std::string f_name) {
	title = f_name;
	//start the random seed
	srand(time(NULL));
	// read midi file f_name into memory, populate objects

	std::ifstream f;
	f.open(f_name, std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		std::cerr << "Error: Unable to open file " << f_name << std::endl;
		return;
	}


	// get size of file
	int f_size = f.tellg();
	f.seekg(std::fstream::beg);

	// read into file_b and close file
	file_bytes = new u_int8_t[f_size];
	for (int i = 0; i < f_size; i++) {
		file_bytes[i] = f.get();
	}
	f.close();

	u_int8_t* file_b_pop = file_bytes;	// save the old data pointer for deleting array
	header_chunk = MThd(file_b_pop);

	while(file_b_pop - file_bytes < f_size) {
		track_chunks.push_back(std::make_shared<MTrk>(MTrk(file_b_pop)));
	}


	// delete file byte array after objects are populated
	delete[] file_bytes;
	file_bytes = NULL;
}

void Midi::midi_panic() {
	if(file_bytes) {
		delete[] file_bytes;
	}
	std::cerr << "An error has occured" << std::endl;
}

void Midi::write() {
	write("midi_" + std::to_string(rand()%10000));
}

void Midi::write(std::string s) {
	std::fstream f;

	// return;

	f.open("output.mid", std::ios::binary | std::ios::trunc | std::ios::out);
	// f.open(s, std::ios::binary | std::ios::trunc | std::ios::out);

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

void Midi::tree() {
	std::cout << title << std::endl;
	header_chunk.tree();

	int chunk_count {0};
	for(auto i = track_chunks.begin(); i != track_chunks.end(); i++) {
		(i+1 == track_chunks.end()) ? std::cout << "└── " : std::cout << "├── ";

		if(chunk_count < 10) {
			std::cout << "00" << chunk_count;
		}
		else if(chunk_count < 100) {
			std::cout << "0" << chunk_count;
		}
		else {
			std::cout << chunk_count;
		}
		std::cout << " (Track Chunk)" << std::endl;

		i->get()->tree(i+1 == track_chunks.end(), true);
		++chunk_count;
	}
}

void Midi::tree(int index) {
	if(index > track_chunks.size()-1) {
		std::cerr << "Invalid track chunk index" << std::endl;
	}
	if(index < 10) {
		std::cout << "00" << index;
	}
	else if(index < 100) {
		std::cout << "0" << index;
	}
	else {
		std::cout << index;
	}
	std::cout << " (Track Chunk)" << std::endl;
	track_chunks[index].get()->tree(true, false);
}

