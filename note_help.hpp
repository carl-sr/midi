#pragma once
#include <string>
#include <memory>

namespace Note_Help {
	std::string int_to_note(u_int8_t);
	std::string midi_event_function(u_int8_t);
	std::string midi_event_fb(u_int8_t, u_int8_t);
	std::string midi_event_sb(u_int8_t, u_int8_t);
}