#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <time.h>

#include "./mtrk.hpp"
#include "./note_help.hpp"
#include "./file_bytes.hpp"

class Midi {
	private:
		MThd header_chunk;
		std::vector<std::shared_ptr<MTrk>> track_chunks;
		std::string file_name;
		std::string file_path;
		void split_path_name(std::string&, std::string&, std::string&);

	public:
		void operator=(Midi &);
		void open(std::string);

		void write();
		void write(std::string);
		void info();
		void tree();
		void tree(int);
};