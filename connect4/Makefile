all: clean compile link start

compile:
	g++ -c connect4.cpp -I"../src/include" -DSFML_STATIC

link:
	g++ connect4.o -o game -L"../src/lib" -lsfml-graphics -lsfml-window -lsfml-system

start:
	./game.exe

clean:
	rm -f game *.o