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
		return "(channel " + std::to_string(n & 0xf) + ") " + "note off";
	}
	if(n >= 0x90 && n <= 0x9f) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "note on";
	}
	if(n >= 0xa0 && n <= 0x9f) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "polyphonic aftertouch";
	}
	if(n >= 0xb0 && n <= 0xbf) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "control mode change";
	}
	if(n >= 0xc0 && n <= 0xcf) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "program change";
	}
	if(n >= 0xd0 && n <= 0xdf) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "channel aftertouch";
	}
	if(n >= 0xe0 && n <= 0xef) {
		return "(channel " + std::to_string(n & 0xf) + ") " + "pitch wheel range";
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

	if(function >= 0xb0 && function <= 0xbf) {
		switch(fb) {
			case(0x00):
				return "continuous controller #0";
			case(0x01):
				return "modulation wheel";
			case(0x02):
				return "breath control";
			case(0x03):
				return "continuous controller #3";
			case(0x04):
				return "foot controller";
			case(0x05):
				return "portamento time";
			case(0x06):
				return "data entry";
			case(0x07):
				return "main volume";
			case(0x08):
				return "continuous controller #8";
			case(0x09):
				return "continuous controller #9";
			case(0x0A):
				return "continuous controller #10";
			case(0x0B):
				return "continuous controller #11";
			case(0x0C):
				return "continuous controller #12";
			case(0x0D):
				return "continuous controller #13";
			case(0x0E):
				return "continuous controller #14";
			case(0x0F):
				return "continuous controller #15";
			case(0x10):
				return "continuous controller #16";
			case(0x11):
				return "continuous controller #17";
			case(0x12):
				return "continuous controller #18";
			case(0x13):
				return "continuous controller #19";
			case(0x14):
				return "continuous controller #20";
			case(0x15):
				return "continuous controller #21";
			case(0x16):
				return "continuous controller #22";
			case(0x17):
				return "continuous controller #23";
			case(0x18):
				return "continuous controller #24";
			case(0x19):
				return "continuous controller #25";
			case(0x1A):
				return "continuous controller #26";
			case(0x1B):
				return "continuous controller #27";
			case(0x1C):
				return "continuous controller #28";
			case(0x1D):
				return "continuous controller #29";
			case(0x1E):
				return "continuous controller #30";
			case(0x1F):
				return "continuous controller #31";
			case(0x20):
				return "continuous controller #0";
			case(0x21):
				return "modulation wheel";
			case(0x22):
				return "breath control";
			case(0x23):
				return "continuous controller #3";
			case(0x24):
				return "foot controller";
			case(0x25):
				return "portamento time";
			case(0x26):
				return "data entry";
			case(0x27):
				return "main volume";
			case(0x28):
				return "continuous controller #8";
			case(0x29):
				return "continuous controller #9";
			case(0x2A):
				return "continuous controller #10";
			case(0x2B):
				return "continuous controller #11";
			case(0x2C):
				return "continuous controller #12";
			case(0x2D):
				return "continuous controller #13";
			case(0x2E):
				return "continuous controller #14";
			case(0x2F):
				return "continuous controller #15";
			case(0x30):
				return "continuous controller #16";
			case(0x31):
				return "continuous controller #17";
			case(0x32):
				return "continuous controller #18";
			case(0x33):
				return "continuous controller #19";
			case(0x34):
				return "continuous controller #20";
			case(0x35):
				return "continuous controller #21";
			case(0x36):
				return "continuous controller #22";
			case(0x37):
				return "continuous controller #23";
			case(0x38):
				return "continuous controller #24";
			case(0x39):
				return "continuous controller #25";
			case(0x3A):
				return "continuous controller #26";
			case(0x3B):
				return "continuous controller #27";
			case(0x3C):
				return "continuous controller #28";
			case(0x3D):
				return "continuous controller #29";
			case(0x3E):
				return "continuous controller #30";
			case(0x3F):
				return "continuous controller #31";
			case(0x40):
				return "damper pedal on/off (sustain)";
			case(0x41):
				return "portamento on/off";
			case(0x42):
				return "sustenuto on/off";
			case(0x43):
				return "soft pedal on/off";
			case(0x44):
				return "undefined on/off";
			case(0x45):
				return "undefined on/off";
			case(0x46):
				return "undefined on/off";
			case(0x47):
				return "undefined on/off";
			case(0x48):
				return "undefined on/off";
			case(0x49):
				return "undefined on/off";
			case(0x4A):
				return "undefined on/off";
			case(0x4B):
				return "undefined on/off";
			case(0x4C):
				return "undefined on/off";
			case(0x4D):
				return "undefined on/off";
			case(0x4E):
				return "undefined on/off";
			case(0x4F):
				return "undefined on/off";
			case(0x50):
				return "undefined on/off";
			case(0x51):
				return "undefined on/off";
			case(0x52):
				return "undefined on/off";
			case(0x53):
				return "undefined on/off";
			case(0x54):
				return "undefined on/off";
			case(0x55):
				return "undefined on/off";
			case(0x56):
				return "undefined on/off";
			case(0x57):
				return "undefined on/off";
			case(0x58):
				return "undefined on/off";
			case(0x59):
				return "undefined on/off";
			case(0x5A):
				return "undefined on/off";
			case(0x5B):
				return "undefined on/off";
			case(0x5C):
				return "undefined on/off";
			case(0x5D):
				return "undefined on/off";
			case(0x5E):
				return "undefined on/off";
			case(0x5F):
				return "undefined on/off";
			case(0x60):
				return "data entry +1";
			case(0x61):
				return "data entry -1";
			case(0x62):
				return "undefined";
			case(0x63):
				return "undefined";
			case(0x64):
				return "undefined";
			case(0x65):
				return "undefined";
			case(0x66):
				return "undefined";
			case(0x67):
				return "undefined";
			case(0x68):
				return "undefined";
			case(0x69):
				return "undefined";
			case(0x6A):
				return "undefined";
			case(0x6B):
				return "undefined";
			case(0x6C):
				return "undefined";
			case(0x6D):
				return "undefined";
			case(0x6E):
				return "undefined";
			case(0x6F):
				return "undefined";
			case(0x70):
				return "undefined";
			case(0x71):
				return "undefined";
			case(0x72):
				return "undefined";
			case(0x73):
				return "undefined";
			case(0x74):
				return "undefined";
			case(0x75):
				return "undefined";
			case(0x76):
				return "undefined";
			case(0x77):
				return "undefined";
			case(0x78):
				return "undefined";
			case(0x79):
				return "undefined";
			case(0x7A):
				return "local control on/off";
			case(0x7B):
				return "all notes off";
			case(0x7C):
				return "omni mode off (includes all notes off)";
			case(0x7D):
				return "omni mode on (includes all notes off)";
			case(0x7E):
				return "poly mode on/off(includes all notes off)";
			case(0x7F):
				return "poly mode on(incl mono=off&all notes off)";

		}
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
	if(function >= 0xb0 && function <= 0xbf) {
		switch(sb) {
			case(0x00):
				return std::to_string(sb);
			case(0x01):
				return std::to_string(sb);
			case(0x02):
				return std::to_string(sb);
			case(0x03):
				return std::to_string(sb);
			case(0x04):
				return std::to_string(sb);
			case(0x05):
				return std::to_string(sb);
			case(0x06):
				return std::to_string(sb);
			case(0x07):
				return std::to_string(sb);
			case(0x08):
				return std::to_string(sb);
			case(0x09):
				return std::to_string(sb);
			case(0x0A):
				return std::to_string(sb);
			case(0x0B):
				return std::to_string(sb);
			case(0x0C):
				return std::to_string(sb);
			case(0x0D):
				return std::to_string(sb);
			case(0x0E):
				return std::to_string(sb);
			case(0x0F):
				return std::to_string(sb);
			case(0x10):
				return std::to_string(sb);
			case(0x11):
				return std::to_string(sb);
			case(0x12):
				return std::to_string(sb);
			case(0x13):
				return std::to_string(sb);
			case(0x14):
				return std::to_string(sb);
			case(0x15):
				return std::to_string(sb);
			case(0x16):
				return std::to_string(sb);
			case(0x17):
				return std::to_string(sb);
			case(0x18):
				return std::to_string(sb);
			case(0x19):
				return std::to_string(sb);
			case(0x1A):
				return std::to_string(sb);
			case(0x1B):
				return std::to_string(sb);
			case(0x1C):
				return std::to_string(sb);
			case(0x1D):
				return std::to_string(sb);
			case(0x1E):
				return std::to_string(sb);
			case(0x1F):
				return std::to_string(sb);
			case(0x20):
				return std::to_string(sb);
			case(0x21):
				return std::to_string(sb);
			case(0x22):
				return std::to_string(sb);
			case(0x23):
				return std::to_string(sb);
			case(0x24):
				return std::to_string(sb);
			case(0x25):
				return std::to_string(sb);
			case(0x26):
				return std::to_string(sb);
			case(0x27):
				return std::to_string(sb);
			case(0x28):
				return std::to_string(sb);
			case(0x29):
				return std::to_string(sb);
			case(0x2A):
				return std::to_string(sb);
			case(0x2B):
				return std::to_string(sb);
			case(0x2C):
				return std::to_string(sb);
			case(0x2D):
				return std::to_string(sb);
			case(0x2E):
				return std::to_string(sb);
			case(0x2F):
				return std::to_string(sb);
			case(0x30):
				return std::to_string(sb);
			case(0x31):
				return std::to_string(sb);
			case(0x32):
				return std::to_string(sb);
			case(0x33):
				return std::to_string(sb);
			case(0x34):
				return std::to_string(sb);
			case(0x35):
				return std::to_string(sb);
			case(0x36):
				return std::to_string(sb);
			case(0x37):
				return std::to_string(sb);
			case(0x38):
				return std::to_string(sb);
			case(0x39):
				return std::to_string(sb);
			case(0x3A):
				return std::to_string(sb);
			case(0x3B):
				return std::to_string(sb);
			case(0x3C):
				return std::to_string(sb);
			case(0x3D):
				return std::to_string(sb);
			case(0x3E):
				return std::to_string(sb);
			case(0x3F):
				return std::to_string(sb);
			case(0x40):
				if(sb) {return "on";} else {return "off";}
			case(0x41):
				if(sb) {return "on";} else {return "off";}
			case(0x42):
				if(sb) {return "on";} else {return "off";}
			case(0x43):
				if(sb) {return "on";} else {return "off";}
			case(0x44):
				if(sb) {return "on";} else {return "off";}
			case(0x45):
				if(sb) {return "on";} else {return "off";}
			case(0x46):
				if(sb) {return "on";} else {return "off";}
			case(0x47):
				if(sb) {return "on";} else {return "off";}
			case(0x48):
				if(sb) {return "on";} else {return "off";}
			case(0x49):
				if(sb) {return "on";} else {return "off";}
			case(0x4A):
				if(sb) {return "on";} else {return "off";}
			case(0x4B):
				if(sb) {return "on";} else {return "off";}
			case(0x4C):
				if(sb) {return "on";} else {return "off";}
			case(0x4D):
				if(sb) {return "on";} else {return "off";}
			case(0x4E):
				if(sb) {return "on";} else {return "off";}
			case(0x4F):
				if(sb) {return "on";} else {return "off";}
			case(0x50):
				if(sb) {return "on";} else {return "off";}
			case(0x51):
				if(sb) {return "on";} else {return "off";}
			case(0x52):
				if(sb) {return "on";} else {return "off";}
			case(0x53):
				if(sb) {return "on";} else {return "off";}
			case(0x54):
				if(sb) {return "on";} else {return "off";}
			case(0x55):
				if(sb) {return "on";} else {return "off";}
			case(0x56):
				if(sb) {return "on";} else {return "off";}
			case(0x57):
				if(sb) {return "on";} else {return "off";}
			case(0x58):
				if(sb) {return "on";} else {return "off";}
			case(0x59):
				if(sb) {return "on";} else {return "off";}
			case(0x5A):
				if(sb) {return "on";} else {return "off";}
			case(0x5B):
				if(sb) {return "on";} else {return "off";}
			case(0x5C):
				if(sb) {return "on";} else {return "off";}
			case(0x5D):
				if(sb) {return "on";} else {return "off";}
			case(0x5E):
				if(sb) {return "on";} else {return "off";}
			case(0x5F):
				if(sb) {return "on";} else {return "off";}
			case(0x60):
				return "127";
			case(0x61):
				return "127";
			case(0x7A):
				if(sb) {return "on";} else {return "off";}
			case(0x7B):
				return "0";
			case(0x7C):
				return "0";
			case(0x7D):
				return "0";
			case(0x7F):
				return "0";
		}
	}
	
	return "";
}
