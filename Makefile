CXX=clang++
CXXFLAGS=-Wall -O3 -std=c++11

main: main.cpp fraction.h
	${CXX} main.cpp -o main $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f main
