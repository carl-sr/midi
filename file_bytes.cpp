#include "./file_bytes.hpp"


File_bytes::File_bytes(std::string f_name) {
	std::ifstream f;
	f.open(f_name, std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		throw std::runtime_error("Unable to open file");
	}


	// get size of file
	file_size = f.tellg();
	f.seekg(std::fstream::beg);

	head = new u_int8_t[file_size];
	for (int i = 0; i < file_size; i++) {
		head[i] = f.get();
	}
	f.close();
	pos = head;
}

File_bytes::~File_bytes() {
	if(head) {
		delete[] head;
	}
}

void File_bytes::check_range() {
	if((head + file_size - 1) < pos) {
		throw std::runtime_error("File_bytes: memory access past EOF");
	}
	if(head > pos) {
		throw std::runtime_error("File_bytes: memory access before start of file");
	}
}

u_int8_t File_bytes::operator[](int i) {
	if((i + pos) > (head + file_size - 1)) {
		throw std::runtime_error("File_bytes: memory access past EOF");
	}
	if(head > (i + pos)) {
		throw std::runtime_error("File_bytes: memory access before start of file");
	}
	return pos[i];
}

File_bytes& File_bytes::operator++(int) {
	pos++;
	return *this;
}

File_bytes& File_bytes::operator+=(int x) {
	pos += x;
	return *this;
}

File_bytes& File_bytes::operator-=(int x) {
	pos -= x;
	return *this;
}

u_int8_t File_bytes::operator*() {
	return *pos;
}

