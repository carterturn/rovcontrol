CC = gcc
CPP = g++
MAIN_SRC = main.cpp
EXE = rovcontrol
CFLAGS = -I/usr/include/freetype2/ -I/usr/include/ctsocket -lGL -lglfw -lftgl -lfuturegl -lctsocket -ltomcrypt

build: $(SRC) $(MAIN_SRC)
	$(CPP) -o $(EXE) $(SRC) $(INC) $(MAIN_SRC) $(CFLAGS)
	
clean:
	rm -f *.o *.h.gch $(EXE)
	
rebuild: clean build

