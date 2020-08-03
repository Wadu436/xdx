CXX=clang++
CXXFLAGS=-Wall -O3 -std=c++17

main: main.cpp fraction.h constants.h
	${CXX} main.cpp -o main $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f main
