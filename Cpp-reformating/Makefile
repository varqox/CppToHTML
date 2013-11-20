ARCHITECTURE = 64
CC = clang
CXX = clang++
LINK = clang++
CFLAGS = -Wall -O3 -m$(ARCHITECTURE)
CXXFLAGS = -Wall -O3 -m$(ARCHITECTURE)
LFLAGS = -Wall -s -O3 -m$(ARCHITECTURE)
RM = rm -f
CP = cp -f

.PHONY: all clean

all: cr

cr: main.o
	$(LINK) $(LFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# clean
clean:
	$(RM) *.o