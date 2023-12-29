all: clean compile link start

compile:
	g++ -c main.cpp -I"src/include" -DSFML_STATIC

link:
	g++ main.o -o game -L"src/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lsfml-main

start:
	./game.exe

clean:
	rm -f game *.o