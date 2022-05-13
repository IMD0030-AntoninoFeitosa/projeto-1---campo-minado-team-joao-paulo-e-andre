	mkdir dist

	g++ -c src/utils/string.cpp -o dist/string.o
	g++ -c src/utils/time.cpp -o dist/time.o
	g++ -c src/Game.cpp -o dist/Game.o
	g++ -c src/main.cpp -o dist/main.o

	g++ -o dist/main dist/main.o dist/Game.o
	./dist/main