Make:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out
	./main.out ./simple/1.mid

compile:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out

debug:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out
	gdb ./main.out ./simple/1.mid
