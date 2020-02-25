#pragma once
#include <vector>
#include <string>

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
		void tree();
};