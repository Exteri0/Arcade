all: clean compile link start

compile:
	g++ -c main.cpp -I"src/include"

link:
	g++ main.o -o game -L"src/lib" -lsfml-graphics -lsfml-window -lsfml-system

start:
	./game.exe

clean:
	rm -f game *.o