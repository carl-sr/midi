#include <string>
#include <memory>

#include "./note_help.hpp"

std::string Note_Help::int_to_note(u_int8_t n) {
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


std::string Note_Help::midi_event_function(u_int8_t n) {
	if(n >= 0x80 && n <= 0x8f) {
		return "note off";
	}
	if(n >= 0x90 && n <= 0x9f) {
		return "note on";
	}
	if(n >= 0xa0 && n <= 0x9f) {
		return "polyphonic aftertouch";
	}
	if(n >= 0xb0 && n <= 0xbf) {
		return "control mode change";
	}
	if(n >= 0xc0 && n <= 0xcf) {
		return "program change";
	}
	if(n >= 0xd0 && n <= 0xdf) {
		return "channel aftertouch";
	}
	if(n >= 0xe0 && n <= 0xef) {
		return "pitch wheel range";
	}
	switch(n) {
		case (0xf0):
			return "system Exclusive";
		case (0xf1):
			return "system common - undefined";
		case (0xf2):
			return "system common - song position pointer";
		case (0xf3):
			return "system common - song select (song number)";
		case (0xf4):
			return "system common - undefined";
		case (0xf5):
			return "system common - undefined";
		case (0xf6):
			return "system common - tune request";
		case (0xf7):
			return "system common - end of SysEx (EOX)";
		case (0xf8):
			return "system real time timing clock";
		case (0xf9):
			return "system real time undefined";
		case (0xfa):
			return "system real time start";
		case (0xfb):
			return "system real time continue";
		case (0xfc):
			return "system real time stop";
		case (0xfd):
			return "system real time undefined";
		case (0xfe):
			return "system real time active sensing";
		case (0xff):
			return "system real time system reset";
	}
	return "undefined";
}

std::string Note_Help::midi_event_fb(u_int8_t function, u_int8_t fb) {
	if(function >= 0x80 && function <= 0xaf) {
		return Note_Help::int_to_note(fb);
	}
	if(function >= 0xc0 && function <= 0xcf) {
		return "program " + std::to_string(fb);
	}
	if(function >= 0xd0 && function <= 0xdf) {
		return "aftertouch pressure " + std::to_string(fb);
	}
	if(function >= 0xe0 && function <= 0xef) {
		return "pitch wheel LSB " + std::to_string(fb);
	}
	if(function == 0xf2) {
		return "MSB " + std::to_string(fb);
	}
	if(function == 0xf3) {
		return std::to_string(fb);
	}

	return "";
}


std::string Note_Help::midi_event_sb(u_int8_t function, u_int8_t sb) {
	if(function >= 0x80 && function <= 0x9e) {
		return "velocity " + std::to_string(sb);
	}
	if(function >= 0xa0 && function <= 0xaf) {
		return "aftertouch pressure " + std::to_string(sb);
	}
	if(function >= 0xe0 && function <= 0xef) {
		return "pitch wheel MSB " + std::to_string(sb);
	}
	if(function == 0xf2) {
		return "LSB " + std::to_string(sb);
	}

	return "";
}
