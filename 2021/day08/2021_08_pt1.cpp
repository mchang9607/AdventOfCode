#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

int count_chars(std::string& word) {
    int count = 0;
    std::unordered_set<char> chars;
    for (auto c : word) {
        chars.insert(c);        
    }
    return chars.size();
}

int main() {
    constexpr int IRRELAVENT_INFO = 11;
    constexpr int OUTPUT_DIGITS = 4;
    
    std::ifstream infile("./input");
    std::string line;

    std::unordered_set<int> simple_digits {2, 3, 4, 7};
    int output = 0;
    
    while(getline(infile, line)) {
        std::istringstream iss(line);
        std::string word;
        for (int i = 0; i < IRRELAVENT_INFO; i++) {
            iss >> word;
        }
        for (int i = 0; i < OUTPUT_DIGITS; i++) {
            iss >> word;
            if(simple_digits.count(count_chars(word))) {
                output++;
            }
        }
    }
    std::cout << output << std::endl;
}