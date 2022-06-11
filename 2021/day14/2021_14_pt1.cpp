#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, char> lookup_table;

void read_input(const std::string input, 
    std::string& polymer, lookup_table& insertion_table) {
    std::ifstream infile(input);
    getline(infile, polymer);

    std::string line;
    while (getline(infile, line)) {
        if (line.find("->") != std::string::npos) {
            insertion_table[line.substr(0, 2)] = line[6];
        }
    }
}

void insertion(std::string& polymer, lookup_table& insertion_table) {
    std::vector<int> replace_indeces;
    
    for (int i = 0; i < polymer.size() - 1; i++) {
        if (insertion_table.count(polymer.substr(i, 2))) {
            replace_indeces.push_back(i + 1);
        }
    }

    for (int i = 0; i < replace_indeces.size(); i++) {
        char to_insert = insertion_table[polymer.substr(replace_indeces[i] + i - 1, 2)];
        polymer.insert(replace_indeces[i] + i, 1, to_insert);
    }

}

int calculate_pt1(std::string& polymer) {
    std::unordered_map<char, int> counter;
    int max = -1;
    int min = -1;
    for (auto c : polymer) {
        counter[c]++;
    }
    for (auto& pair : counter) {
        if (pair.second < min || min == -1) {
            min = pair.second;
        }
        if (pair.second > max) {
            max = pair.second;
        }
    }

    return max - min;
}

int main() {
    std::string polymer;
    lookup_table insertion_table;    
    read_input("./input", polymer, insertion_table);

    for (int i = 0; i < 10; i++) {
        insertion(polymer, insertion_table);
    }

    std::cout << calculate_pt1(polymer) << std::endl;

    return 0;
}