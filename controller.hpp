#pragma once

#include <string>

#include "./midi.hpp"

class Controller {
	private:
		Midi* mid {NULL};
		bool q {false};
	public:

		void input(char c) {
			switch(c) {
				default:
					break;
			}
		}
		void command(std::string s) {
			if(s == "quit") {
				q = true;
				return;
			}
		}
		bool quit() {
			return !q;
		}


};