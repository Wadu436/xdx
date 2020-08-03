// Licensed CC-BY-SA 3.0
// https://creativecommons.org/licenses/by-sa/3.0/

// Created by Warre Dujardin

#include "constants.h"
#include "fraction.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

std::numeric_limits<LARGEINT> lim = {};

// source https://stackoverflow.com/a/18604441
// added sorted printing
template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &stream, const std::map<T1, T2>& map) {
    auto key_list = std::vector<T1>();

    for(auto iter1 = map.begin(); iter1 != map.end(); iter1++) {
        key_list.push_back(iter1->first);
    }

    std::sort(key_list.begin(), key_list.end());

    for (auto it = key_list.begin(); it != key_list.end(); ++it) {
        stream << std::setprecision(0) << std::left << std::setfill(' ') << std::setw(16) << *it << " --> \t" << std::setprecision(60) << map.at(*it) << "%\n";
    }
    return stream;
}

// source https://stackoverflow.com/a/26221725
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

bool overflow_addition(LARGEINT x, LARGEINT y);
bool overflow_multiplication(LARGEINT x, LARGEINT y);

std::map<LARGEINT, LARGEINT> ndx(LARGEINT n, LARGEINT x, bool& overflow);
std::map<LARGEINT, LARGEINT> xdx(LARGEINT x, bool& overflow);

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << "ERROR! Forgot to give X\n";
        return -1;
    }

    char* endp;
    long int x = strtol(argv[1], &endp, 10);

    bool overflow = false;

    if (x <= 0) {
        std::cerr << "Invalid number\n";
        return -1;
    }

    std::cout << "Using datatype which can contain numbers in the range:\n[" << lim.min() << ", " << lim.max() << "]\n\n";

    auto d1 = xdx(x, overflow);
    auto d2 = xdx(x+1, overflow);
    auto d3 = xdx(x+2, overflow);

    auto collect = std::map<fraction, LARGEINT>();
    auto probability = std::map<fraction, double>();
    LARGEINT total = 0;

    for(auto iter1 = d1.begin(); iter1 != d1.end(); iter1++) {
        for(auto iter2 = d2.begin(); iter2 != d2.end(); iter2++) {
            for(auto iter3 = d3.begin(); iter3 != d3.end(); iter3++) {
                overflow |= overflow_addition(iter1->first, iter2->first);
                fraction key = {iter1->first + iter2->first, iter3->first};

                LARGEINT value = iter1->second;
                overflow |= overflow_multiplication(value, iter2->second);
                value *= iter2->second;
                overflow |= overflow_multiplication(value, iter3->second);
                value *= iter3->second;

                collect[key] += value;
                total += value;
            }
        }
    }

    for(auto it = collect.begin(); it != collect.end(); it++) {
        probability[it->first] = (100.0 * double(it->second)) / total;
    }
    
    if(!overflow) {
        std::string header = "Probabilities for (%dd%d + %dd%d)/(%dd%d)\n\n";

        std::cout << std::fixed << std::setprecision(20);
        std::cout << string_format(header, x, x, x+1, x+1, x+2, x+2);
        std::cout << probability << '\n';
        std::cout << std::setprecision(0) << "Total different scenarios: " << total << '\n';
    } else {
        std::cerr << "Overflow was detected, results aren't valid\n";
    }
}

std::map<LARGEINT, LARGEINT> ndx(LARGEINT n, LARGEINT x, bool& overflow) {
    auto m = std::map<LARGEINT, LARGEINT>();

    // BASE CASE
    if(n == 0) {
        m[0] = 1;
        return m;
    }

    auto o = ndx(n-1, x, overflow);

    for(LARGEINT i = 1; i <= x; i++) {
        for(auto iter = o.begin(); iter != o.end(); iter++) {
            LARGEINT key = iter->first;
            LARGEINT value = iter->second;

            overflow |= overflow_addition(key, i);
            LARGEINT m_key = key + i;

            overflow |= overflow_addition(m[m_key], value);
            m[m_key] += value;
        }
    }

    return m;
}

std::map<LARGEINT, LARGEINT> xdx(LARGEINT x, bool& overflow) {
    return ndx(x, x, overflow);
}

bool overflow_addition(LARGEINT x, LARGEINT y) {
    return (x > (lim.max() - y));
}

bool overflow_multiplication(LARGEINT x, LARGEINT y) {
    return (y !=0 && x > (lim.max() / y));
}