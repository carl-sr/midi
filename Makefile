Make:
	g++ main.cpp mtrk.cpp midi.cpp note_help.cpp -g -o main.out
	./main.out ./simple/1.mid
	# ./main.out ./mario_theme.mid

compile:
	g++ main.cpp mtrk.cpp midi.cpp note_help.cpp -g -o main.out

debug:
	g++ main.cpp mtrk.cpp midi.cpp note_help.cpp -g -o main.out
	gdb ./main.out ./simple/1.mid
