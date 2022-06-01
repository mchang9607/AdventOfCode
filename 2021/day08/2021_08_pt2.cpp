#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

constexpr int OUTPUT_DIGITS = 4;
constexpr int UNIQUE_DIGITS = 10;

// counts the amount of unique characters in a string
int count_chars(const std::string& word) {
    int count = 0;
    std::unordered_set<char> chars;
    for (auto c : word) {
        chars.insert(c);        
    }
    return chars.size();
}

// checks whether str1 is a subset of str2
bool is_subset(const std::string str1, const std::string str2) {
    std::unordered_set<char> chars;
    for (auto c: str2) {
        chars.insert(c);
    }
    int str2_chars = chars.size();
    for (auto c: str1) {
        chars.insert(c);
    }
    if (chars.size() == str2_chars) {
        return true;
    }
    return false;
}

// calculates the value of the output_array given a map of sequence values
int calculate_output_value(std::array<std::string, OUTPUT_DIGITS>& output_array,
std::unordered_map<std::string, int>& sequence_to_num) {
    int value = 0;
    for (int i = output_array.size() -1; i >= 0; i--) {
        int digit_value = sequence_to_num[output_array[output_array.size() - 1 - i]];
        value += (digit_value * std::pow(10, i));
    }
    return value;
}


int identify_digits(std::string& line) {
    constexpr int UNIDENTIFIED = -1;

    // digits that are identible without any extra effort
    std::unordered_set<int> simple_digits_length {2, 3, 4, 7};

    // unidentified sequences
    std::unordered_set<std::string> clues;
    std::unordered_set<std::string> to_identify;

    // two way map that gives information of mapping from number to sequence string
    std::unordered_map<int, std::string> num_to_sequence;
    std::unordered_map<std::string, int> sequence_to_number;

    std::istringstream iss(line);

    // if we can already identify such sequence (is a simple digit), do it
    for (int i = 0; i < UNIQUE_DIGITS; i++) {
        std::string word;
        iss >> word;
        std::sort(word.begin(), word.end());
        int word_length = count_chars(word);
        if (simple_digits_length.count(word_length)) {
            int word_value = UNIDENTIFIED;
            if (word_length == 2) {
                word_value = 1;
            } else if (word_length == 3) {
                word_value = 7;
            } else if (word_length == 4) {
                word_value = 4;
            } else if (word_length == 7) {
                word_value = 8;
            }
            num_to_sequence[word_value] = word;
            sequence_to_number[word] = word_value;
        } else {
            clues.insert(word);
        }
    }

    std::string trash;
    iss >> trash;

    // read output digits and record unidentified sequeneces
    std::array<std::string, OUTPUT_DIGITS> output_array;
    for (int i = 0; i < OUTPUT_DIGITS; i++) {
        std::string word;
        iss >> word;
        std::sort(word.begin(), word.end());
        output_array[i] = word;
        if (!sequence_to_number.count(word)) {
            to_identify.insert(word);
        }
    }
    
    // while required information is insufficient
    while (!to_identify.empty()) {
        for (auto it = clues.begin(); it != clues.end();) {
            int clue_length = count_chars(*it);
            int value = UNIDENTIFIED;
            // Try to identify the given clue. If successful, update value.
            if (clue_length == 5) {
                if (is_subset(num_to_sequence[1], *it)) {
                    value = 3;
                } else {
                    // 2 and 5 require information about 9 to be identified
                    if (num_to_sequence.count(9)) {
                        if (is_subset(*it, num_to_sequence[9])) {
                            value = 5;
                        } else {
                            value = 2;
                        }
                    }
                }
            } else if (clue_length == 6) {
                if (!is_subset(num_to_sequence[1], *it)) {
                    value = 6;
                } else if (is_subset(num_to_sequence[4], *it)) {
                    value = 9;
                } else {
                    value = 0;
                }
            }
            
            if (value != UNIDENTIFIED) {
                num_to_sequence[value] = *it;
                sequence_to_number[*it] = value;
                to_identify.erase(*it);
                it = clues.erase(it);
            } else {
                it++;
            }
        }
    }

    return calculate_output_value(output_array, sequence_to_number);
    
}


int main() {
    int output = 0;
    
    std::ifstream infile("./input");
    std::string line;
    while (getline(infile, line)) {
        output += identify_digits(line);
    }

    std::cout << output << std::endl;

    return 0;
}