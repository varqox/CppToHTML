# Compile commands
ifeq ($(shell clang > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CC = clang
else
export CC = gcc
endif
ifeq ($(shell clang++ > /dev/null 2> /dev/null; echo $$?), $(shell echo 1))
export CXX = clang++
export LINK = clang++
else
export CXX = g++
export LINK = g++
endif

export CFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -O3 -c
export CXXFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -O3 -c
export LFLAGS = -Wall -W -Wabi -Weffc++ -Wformat -Wshadow -Wsequence-point -Wuninitialized -Wfloat-equal -s -O3

# Shell commands
export MV = mv -f
export CP = cp -f -r
export UPDATE = $(CP) -u
export RM = rm -f
export MKDIR = mkdir -p

.PHONY: all clean
all: cth

cth: main.o aho.o parser.o synax_highlight.o
	$(LINK) $(LFLAGS) $^ -o $@

main.o: main.cpp main.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

aho.o: aho.cpp main.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

parser.o: parser.cpp main.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

synax_highlight.o: synax_highlight.cpp main.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o