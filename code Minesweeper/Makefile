all: clean compile link start



compile:
	g++ -c minesweeper.cpp -I"../src/include" -DSFML_STATIC

link:
	g++ minesweeper.o -o game -L"../src/lib" -lsfml-graphics -lsfml-window -lsfml-system

start:
	./game.exe

clean:
	rm -f game *.o