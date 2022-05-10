app: msg dist game main

msg: 
	@echo Ola, estamos preparando as coisas para voce...

dist: 
	mkdir dist

game:
	g++ -c src/Game.cpp -o dist/Game.o
	g++ -c src/main.cpp -o dist/main.o

main: 
	g++ -o dist/main dist/main.o dist/Game.o
	./dist/main

reload: app	