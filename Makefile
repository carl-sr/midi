Make:
	g++ main.cpp -g -o main.out
	./main.out

compile:
	g++ main.cpp -g -o main.out

debug:
	g++ main.cpp -g -o main.out
	gdb ./main.out