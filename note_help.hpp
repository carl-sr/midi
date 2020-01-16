#pragma once
#include <string>
#include <memory>


std::string int_to_note(u_int8_t n) {
	std::string r;

	switch(n%12) {
		case 0:
			r = "C";
			break;
		case 1:
			r = "C#";
			break;
		case 2:
			r = "D";
			break;
		case 3:
			r = "D#";
			break;
		case 4:
			r = "E";
			break;
		case 5:
			r = "F";
			break;
		case 6:
			r = "F#";
			break;
		case 7:
			r = "G";
			break;
		case 8:
			r = "G#";
			break;
		case 9:
			r = "A";
			break;
		case 10:
			r = "A#";
			break;
		case 11:
			r = "B";
			break;
		default:
			r = "ERROR";
			break;
	}

	r += std::to_string((n/12)-1);
	return r;
}