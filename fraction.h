#pragma once

#include <iostream>

unsigned long gcd(unsigned long n1, unsigned long n2) {
    while(n1 != n2) {
        n1 > n2 ? n1 -= n2 : n2 -= n1;
    }
    return n1;
}

class fraction {
    private:
    unsigned long num, denom;

    public:
    fraction() {}
    fraction(unsigned long numerator, unsigned long denominator) {
        unsigned long g = gcd(numerator, denominator);
        num = numerator / g;
        denom = denominator / g;
    }

    bool operator<(const fraction &other) const {
        return (num * other.denom) < (denom * other.num);
    }

    friend std::ostream& operator<<(std::ostream& stream, fraction f) {
        if(f.denom == 1) {
            return stream << f.num;
        }
        return stream << f.num << " / " << f.denom;
    }
};