// Licensed CC-BY-SA 3.0
// https://creativecommons.org/licenses/by-sa/3.0/

// Created by Warre Dujardin

#include "fraction.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

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
        stream << *it << "\t --> \t" << map.at(*it) << "%\n";
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

std::map<unsigned long, unsigned long> ndx(unsigned long n, unsigned long x);
std::map<unsigned long, unsigned long> xdx(unsigned long x);

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << "ERROR! Forgot to give X\n";
        return -1;
    }

    char* endp;
    long int x = strtol(argv[1], &endp, 10);

    if (x <= 0) {
        std::cerr << "Invalid number\n";
        return -1;
    }

    auto d1 = xdx(x);
    auto d2 = xdx(x+1);
    auto d3 = xdx(x+2);

    auto collect = std::map<fraction, unsigned long>();
    auto probability = std::map<fraction, double>();
    unsigned long total = 0;

    for(auto iter1 = d1.begin(); iter1 != d1.end(); iter1++) {
        for(auto iter2 = d2.begin(); iter2 != d2.end(); iter2++) {
            for(auto iter3 = d3.begin(); iter3 != d3.end(); iter3++) {
                fraction key = {iter1->first + iter2->first, iter3->first};
                unsigned long value = iter1->second * iter2->second * iter3 ->second;

                collect[key] += value;
                total += value;
            }
        }
    }

    for(auto it = collect.begin(); it != collect.end(); it++) {
        probability[it->first] = (100.0 * double(it->second)) / total;
    }

    std::string header = "Probabilities for (%dd%d + %dd%d)/(%dd%d)\n\n";

    std::cout << std::fixed << std::setprecision(20);
    std::cout << string_format(header, x, x, x+1, x+1, x+2, x+2);
    std::cout << probability << '\n';
    std::cout << "Total different scenarios: " << total << '\n';
}

std::map<unsigned long, unsigned long> ndx(unsigned long n, unsigned long x) {
    auto m = std::map<unsigned long, unsigned long>();

    // BASE CASES
    if(n == 0) {
        m[0] = 1;
        return m;
    }

    auto o = ndx(n-1, x);

    for(unsigned long i = 1; i <= x; i++) {
        for(auto iter = o.begin(); iter != o.end(); iter++) {
            unsigned long key = iter->first;
            unsigned long value = iter->second;

            unsigned long m_key = key + i;

            m[m_key] += value;
        }
    }

    return m;
}

std::map<unsigned long, unsigned long> xdx(unsigned long x) {
    return ndx(x, x);
}