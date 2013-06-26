CXX=g++
flags=-s -O3

all: kmp spr gen aho CppToHTML

kmp: kmp.cpp
	$(CXX) kmp.cpp $(flags) -o kmp

spr: spr.cpp
	$(CXX) spr.cpp $(flags) -o spr

gen: gen.cpp
	$(CXX) gen.cpp $(flags) -o gen

aho: aho.cpp
	$(CXX) aho.cpp $(flags) -o aho

CppToHTML: main.cpp
	$(CXX) main.cpp $(flags) -o CppToHTML