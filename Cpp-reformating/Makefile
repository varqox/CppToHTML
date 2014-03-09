export
# Compile commands
ifeq ($(shell clang > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
CC = clang
else
CC = gcc
endif
ifeq ($(shell clang++ > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
CXX = clang++
LINK = clang++
else
CXX = g++
LINK = g++
endif

CFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -O3 -c
CXXFLAGS := $(CFLAGS)
LFLAGS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result -s -O3

# Shell commands
MV = mv -f
CP = cp -f -r
UPDATE = $(CP) -u
RM = rm -f
MKDIR = mkdir -p

.PHONY: all debug clean clean-all

all: cr

debug:
	make all CXX='$(CXX) -DDEBUG' CC='$(CC) -DDEBUG'

cr: main.o aho.o
	$(LINK) $^ $(LFLAGS) -o $@

main.o: main.cpp aho.hpp
	$(CXX) $< $(CXXFLAGS) -o $@

aho.o: aho.cpp aho.hpp
	$(CXX) $< $(CXXFLAGS) -o $@

clean:
	$(RM) *.o

mrproper: clean
	$(RM) cr
