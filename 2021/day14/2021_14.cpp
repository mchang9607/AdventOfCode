#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, long> occurrence_counter;
typedef std::unordered_map<char, long> char_counter;
typedef std::unordered_map<std::string, char> lookup_table;

void read_input(const std::string& input, char_counter& polymer,
    occurrence_counter& polymer_map, lookup_table& insertion_lookup) {
    
    std::ifstream infile(input);
    std::string polymer_str;
    getline(infile, polymer_str);

    // count frequency of each character
    for (auto c : polymer_str) {
        polymer[c]++;
    } 
    // count substring of 2 occurences
    for (int i = 0; i < polymer_str.size() - 1; i++) {
        polymer_map[polymer_str.substr(i, 2)]++;
    }

    // construct insertion lookup map
    std::string line;
    while(getline(infile, line)) {
        if (line.find("->")) {
            insertion_lookup[line.substr(0, 2)] = line[6];
        }
    }
}

occurrence_counter step_insertion(char_counter& polymer,
    occurrence_counter& old_polymer_map, lookup_table& insertion_lookup) {
    // used to record new count of substrings of 2 after iteration
    occurrence_counter new_polymer_map;
    // use original stats to calculate new stats of polymer
    for (auto& pair : old_polymer_map) {
        if (insertion_lookup.count(pair.first)) {
            // count newly inserted characters
            polymer[insertion_lookup[pair.first]] += pair.second;

            // convert to string for string concatination
            std::string to_insert = std::string() + insertion_lookup[pair.first];
            new_polymer_map[pair.first[0] + to_insert] += pair.second;
            new_polymer_map[to_insert + pair.first[1]] += pair.second;
        }
    }
    return new_polymer_map;
}

long calculate_output(char_counter& polymer) {
    long min = -1;
    long max = -1;
    for (auto& pair : polymer) {
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
    char_counter polymer;
    occurrence_counter polymer_map;
    lookup_table insertion_lookup;
    read_input("./input", polymer, polymer_map, insertion_lookup);

    for (int i = 0; i < 10; i++) {
        polymer_map = step_insertion(polymer, polymer_map, insertion_lookup);
    }

    std::cout << "part1: " << calculate_output(polymer) << std::endl;

    for (int i = 0; i < 30; i++) {
        polymer_map = step_insertion(polymer, polymer_map, insertion_lookup);
    }

    std::cout << "part2: " << calculate_output(polymer) << std::endl;
}