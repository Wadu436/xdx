# xdx
Calculates (xdx + (x+1)d(x+1)) / ((x+2)d(x+2)) for any x\
\
Default configuration uses an `unsigned long long`, this allows for up to x = 7 before overflow happens. If you want to go over x = 7, you have to use a floating point type. Change `LARGEINT` in constants.h to some floating point type like `long double`.

# Build
Change the default compiler to g++ or clang++ in Makefile\
Run `make`

# Run
`./main <x>`
