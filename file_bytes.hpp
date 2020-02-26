#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class File_bytes {
	private:
		u_int8_t* head;
		u_int8_t* pos;
		int file_size;

	public:
		File_bytes(std::string);
		~File_bytes();

		void check_range();

		u_int8_t operator[](int);
		File_bytes& operator+=(int);
		File_bytes& operator-=(int);
		File_bytes& operator++(int);
		File_bytes& operator--(int);
		u_int8_t operator*();

		u_int8_t* cur_pos() {
			return pos;
		}
		u_int8_t* cur_head() {
			return head;
		}

		bool is_end() {
			return pos >= (head + file_size-1);
		}

		int size() {
			return file_size;
		}
};

