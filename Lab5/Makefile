# this makefile is intended for gcc on Linux

CC = gcc
# CFLAGS = -c -Wall -Wpedantic
CFLAGS = -c -O3
LDFLAGS =
SOURCES = main.c 
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main

all: $(EXECUTABLE) clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -g -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS)

