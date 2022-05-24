#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

constexpr int COUNTDOWN_SIZE = 9;

std::array<long long, COUNTDOWN_SIZE> initialize_lanternfish_vector(const std::string& input) {
    // countdown array
    std::array<long long, COUNTDOWN_SIZE> lanternfish;
    // initialize array
    for (int i = 0; i < lanternfish.size(); i++) {
        lanternfish[i] = 0;
    }

    // parse input
    std::ifstream infile("./input");
    std::string raw;
    infile >> raw;
    std::replace(raw.begin(), raw.end(), ',', ' ');
    std::istringstream iss(raw);

    // count
    for (int countdown; iss >> countdown;) {
        lanternfish[countdown]++;
    }

    return lanternfish;
}

void simulate(std::array<long long, COUNTDOWN_SIZE>& lanternfish, int days) {
    constexpr int RESET = 6;
    constexpr int NEW_BIRTH = 8;
    for (int i = 0; i < days; i++) {
        long long gave_birth = lanternfish[0];
        for (int j = 0; j < lanternfish.size() - 1; j++) {
            // shift countdown
            lanternfish[j] = lanternfish[j + 1];
        }
        // new born lanternfish
        lanternfish[NEW_BIRTH] = gave_birth;
        // reset lanternfish that gave
        lanternfish[RESET] += gave_birth;        
    }
}

long long count_lanternfish(const std::array<long long, COUNTDOWN_SIZE>& lanternfish) {
    long long total = 0;
    for (auto countdown : lanternfish) {
        total += countdown;
    }
    return total;
}

int main() {
    std::array<long long, COUNTDOWN_SIZE> lanternfish = initialize_lanternfish_vector("./input");
    simulate(lanternfish, 256);
    std::cout << count_lanternfish(lanternfish) << std::endl;
}