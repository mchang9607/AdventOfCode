#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

// convert bitstring into decimal number (int)
int bin_to_dec(std::string bin) {
    int value = 0;
    for (int i = 0; i < bin.size(); i++) {
        if (bin.at(i) == '1') {
            value += (int) std::pow(2, bin.size() - 1 - i);
        }        
    }
    return value;
}

/*
 * apply criteria to find o2 or co2 rate.
 * @param max: determines if looking for most frequent or least frequent bit.
 */
int filter(std::vector<std::string> bitstrings, bool max) {  
    int sig_pos = 0;

    // count ones
    while (bitstrings.size() != 1) {
        int threshold = (bitstrings.size() + 1) / 2;
        int counter = 0;        
        for (auto bitstring : bitstrings) {
            if (bitstring.at(sig_pos) == '1') {
                counter++;
            }
        }

        // Find filtering character. 
        char target;
        if (max) {
            if (counter >= threshold) {
                target = '1';
            } else {
                target = '0';
            }
        } else {
            if (counter < threshold) {
                target = '1';
            } else {
                target = '0';
            }
        }

        // apply filtering criteria
        for (auto it = bitstrings.begin(); it != bitstrings.end();) {
            if ((*it).at(sig_pos) != target) {
                it = bitstrings.erase(it);
            } else {
                it++;
            }
        }
        
        sig_pos++;
    }

    return bin_to_dec(bitstrings.at(0));
}

int main() {
    // read input into vector
    std::ifstream infile("./input");
    std::vector<std::string> bitstrings;
    std::string bitstring;
    while (infile >> bitstring) {
        bitstrings.push_back(bitstring);
    }    
    
    int o2_rate = filter(bitstrings, true);
    int co2_rate = filter(bitstrings, false);

    std::cout << o2_rate * co2_rate << std::endl;
}