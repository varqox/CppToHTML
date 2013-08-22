CXX=g++
CXXFLAGS=-Wall -s -O3 -m32
RM= rm -f

.PHONY: all clean
all: cth

cth: main.o aho.o parser.o synax_highlight.o
	$(CXX) $(CXXFLAGS) $^ -o $@

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