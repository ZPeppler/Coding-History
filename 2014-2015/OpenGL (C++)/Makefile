COMPILING = test.cpp

EXECUTABLE = test

CC = gcc

CFLAGS = -g3 -Wall -Wno-non-template-friend -fpermissive

LIBRARIES = -L"/usr/X11/lib" -lglut -lGL -lGLU -lm -lstdc++

all:
	$(CC) $(CFLAGS) $(LIBRARIES) $(COMPILING) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
