CXX=g++
CXXFLAGS=-s -O3
RM= rm -f

PHONY: all clean
all: cth

cth: main.o aho.o parser.o synax_highlight.o
	$(CXX) $^ -o $@

main.o: main.cpp main.hpp
	$(CXX) -c $< -o $@

aho.o: aho.cpp main.hpp
	$(CXX) -c $< -o $@

parser.o: parser.cpp main.hpp
	$(CXX) -c $< -o $@

synax_highlight.o: synax_highlight.cpp main.hpp
	$(CXX) -c $< -o $@

clean:
	$(RM) *.o