
#include "./midi.hpp"

void Midi::operator=(Midi &m) {

}

void Midi::split_path_name(std::string& full_path, std::string& name, std::string& path) {
	for(int i = full_path.length(); i >=0; i--) {
		if(full_path[i] == '/') {
			name = full_path.substr(i+1, full_path.length());
			path = full_path.substr(0, i+1);
			return;
		}
	}
	name = full_path;
	path = "./";
}


void Midi::open(std::string f_name) {
	split_path_name(f_name, file_name, file_path);
	//start the random seed
	srand(time(NULL));


	try {
		File_bytes file(f_name);
		header_chunk = MThd(file);
		while(!file.is_end()) {
			track_chunks.push_back(std::make_shared<MTrk>(MTrk(file)));
		}
	}
	catch(std::exception& e) {
		std::cerr << "An error occured while reading the midi file: " << e.what() << std::endl;
		track_chunks.clear();
		header_chunk = MThd();
		file_path = "/";
		file_name = "midi_file.mid";
	}
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
	std::cout << "File name: " << file_name << std::endl;
	std::cout << "File path: " << file_path << std::endl;
	header_chunk.print_info();

	if(track_chunks.empty()) {
		std::cout << "└── (No track chunks)" << std::endl;
	}
	else {

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

