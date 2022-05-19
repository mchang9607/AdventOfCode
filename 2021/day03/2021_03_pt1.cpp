#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

int main() {
    std::ifstream infile("./input");
    std::vector<int> counter;
    std::string line;
    int threshold = 0;
    int bitstring_length = 0;
    int value = 0;

    // initialize counter vector
    getline(infile, line);
    for (int i = 0; i < line.size(); i++) {
        if (line.at(i) == '0') {
            counter.push_back(0);
        } else if (line.at(i) == '1') {
            counter.push_back(1);
        }
        bitstring_length += 1;
    }

    // count
    while (getline(infile, line)) {
        for (int i = 0; i < line.size(); i++) {
            if (line.at(i) == '1') {
                counter.at(i) += 1;
            }
        }
        threshold += 1;
    }
    threshold /= 2;

    // find gamma rate
    for (int i = 0; i < counter.size(); i++) {
        if (counter.at(i) > threshold) {
            value += (int)std::pow(2, counter.size() - 1 - i);
        }
    }
    
    // gamma and epsilon sum up to 2^(bitstring_length) - 1
    value *= ((int)std::pow(2, bitstring_length) - 1 - value);

    std::cout << value << std::endl;

    return 0;
}