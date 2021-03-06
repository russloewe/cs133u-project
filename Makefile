CC=gcc
CFLAGS= -g --std=c99 -O3 
LIBS=   `sdl2-config --libs`  


OBJ=  input.o graphics.o gameobjects.o checker.o maps.o ai.o draw.o
TESTS= test_graphics.o test_gameobjects.o test_checker.o test_ai.o test_input.o test_maps.o test.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ) main.o
	gcc -o GnomeSlayer $^ -g $(LIBS)
	
test: $(OBJ) $(TESTS)
	gcc -o test $^ -g $(LIBS)
	
install:
	mkdir -p /usr/share/GnomeSlayer
	cp -r ./img /usr/share/GnomeSlayer/
	cp -r ./rooms /usr/share/GnomeSlayer/
	cp GnomeSlayer /usr/games/

clean:
	rm -rf *o
	rm -rf GnomeSlayer
	rm -rf test
