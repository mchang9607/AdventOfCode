#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

int calculate_pt1_score(std::vector<char>& corrupts) {
    std::unordered_map<char, int> corruption_score_table = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    int score = 0;
    for (auto corrupt : corrupts) {
        score += corruption_score_table[corrupt];
    }
    return score;
}

long calculate_pt2_score(std::vector<std::string>& fixs) {
    std::unordered_map<char, int> completion_score_table = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
    std::vector<long> scores;

    for (auto& fix : fixs) {
        long score = 0;
        for (auto c : fix) {
            score *= 5;
            score += completion_score_table[c];
        }
        scores.push_back(score);
    }
    std::sort(scores.begin(), scores.end());

    return scores.at(scores.size() / 2);
}

int main() {
    const std::unordered_set<char> open_chars = {'(', '<', '[', '{'};
    const std::unordered_set<char> close_chars = {')', '>', ']', '}'};
    std::unordered_map<char, char> lookup_close = {{'(', ')'}, {'<', '>'}, {'[', ']'}, {'{', '}'}};

    std::vector<char> corrupts;
    std::vector<std::string> fixs;

    std::ifstream infile("./input");
    std::string line;
    while (getline(infile, line)) {
        std::list<char> stack;
        for (auto c : line) {
            // invalid char count
            if (!open_chars.count(c) && !close_chars.count(c)) {
                continue;
            }

            if (open_chars.count(c)) {
                stack.push_back(c);
            } else {
                // open char was closed correctly
                if (c == lookup_close[stack.back()]) {
                    stack.pop_back();
                } else {
                    // corrupted case
                    corrupts.push_back(c);
                    stack.clear();
                    break;
                }
            }
            
        }
        // incomplete case
        if (!stack.empty()) {
            std::string repair = "";
            for (auto c : stack) {
                repair = lookup_close[c] + repair;
            }
            fixs.push_back(repair);
        }
    }

    std::cout << "part1: " << calculate_pt1_score(corrupts) << std::endl;
    std::cout << "part2: " << calculate_pt2_score(fixs) << std::endl;

    return 0;
}
