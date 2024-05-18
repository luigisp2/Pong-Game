all: compile link

compile:
	g++ -I src/include -c Pong.cpp

link:
	g++ Pong.o -o Pong -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system