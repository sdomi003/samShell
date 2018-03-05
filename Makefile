
FLAG = -g -Wall -Werror -ansi -pedantic
COMPILE = g++
FILE1 = main.cpp
FILE2 = command.cpp
DIRECTORY = src/

EXECUTIBLE = rshell
SOURCEDIR = src
BUILDDIR = bin


all:
	mkdir -p bin
	$(COMPILE) $(FLAG) $(DIRECTORY)$(FILE1) -o bin/rshell

rshell:
	mkdir -p bin
	$(COMPILE) $(FLAG)  $(DIRECTORY)$(FILE1) -o bin/rshell

clean:
	rm bin/rshell
	rm -rf bin	