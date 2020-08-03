#pragma once

#include "constants.h"

#include <iostream>

LARGEINT gcd(LARGEINT n1, LARGEINT n2) {
    while(n1 != n2) {
        n1 > n2 ? n1 -= n2 : n2 -= n1;
    }
    return n1;
}

class fraction {
    private:
    LARGEINT num, denom;

    public:
    fraction() {}
    fraction(LARGEINT numerator, LARGEINT denominator) {
        LARGEINT g = gcd(numerator, denominator);
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