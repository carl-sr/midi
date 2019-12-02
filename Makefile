Make:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out
	./main.out

compile:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out

debug:
	g++ main.cpp mtrk.cpp -g -lncurses -o main.out
	gdb ./main.out