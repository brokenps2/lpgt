TARGET = bin/program

SRCS = src/*.cpp

INCLUDES = -Iinclude/

LIBDIRS = -L/usr/lib

LIBS = -l:libSDL2main.a -l:libSDL2.a -lm -lGL -l:libGLEW.so

CFLAGS = -O2 -Wall -ggdb -g2

CC = g++

all:
	$(CC) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIBDIRS) $(LIBS) -o $(TARGET)

