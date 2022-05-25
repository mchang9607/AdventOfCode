#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
    
// reading file input be wack
std::vector<int> read_input(const std::string& path) {
    std::ifstream infile(path);
    std::string raw;
    infile >> raw;
    std::replace(raw.begin(), raw.end(), ',', ' ');
    std::istringstream iss(raw);
    std::vector<int> crabs;
    for (int crab; iss >> crab;) {
        crabs.push_back(crab);
    }
    std::sort(crabs.begin(), crabs.end());
    return crabs;
}

int calculate_difference_pt1(const std::vector<int>& crabs) {
    /*
     * If center is between two points, connecting 
     * pt1, center and pt2 is the same as connecting pt1 and pt2.
     */
    int center = crabs[crabs.size() / 2];
    int diff = 0;

    for (auto crab : crabs) {        
        diff += std::abs(crab - center);
    }
    
    return diff;
}

int pt2_step_cost(int crab, int center) {
    int steps = std::abs(crab - center);
    return ((steps * (steps + 1)) / 2);
}

int calculate_difference_pt2(const std::vector<int>& crabs) {
    /*
     * Average is the sweet spot where each crab has to walk the least "in average".
     * This is ideal since it is more beneficial for each crab to walk about the same,
     * as when unbalanced the crab that has to walk more will have a significantly increasing cost.
     */

    double avg = 0;
    for (auto crab : crabs) {
        avg += crab;
    }
    avg /= crabs.size();
    
    // round ... which didn't work. Why?
    // if (((int)(avg * 10)) % 10 >= 5) {
    //     avg = avg + 1;
    // }
    // int center = (int)avg;
    
    // due to rounding not working, we compare values of different centers (floor vs ceiling)
    // calculate fuel consumption
    int center_floor = (int)avg;
    int center_ceiling = center_floor + 1;
    int fuel_floor = 0;
    int fuel_ceiling = 0;
    for (auto crab : crabs) {
        fuel_floor += pt2_step_cost(crab, center_floor);
        fuel_ceiling += pt2_step_cost(crab, center_ceiling);
    }
    return std::min(fuel_floor, fuel_ceiling);
}

int main() {
    std::vector<int> crabs = read_input("./input");
    
    int diff1 = calculate_difference_pt1(crabs);
    int diff2 = calculate_difference_pt2(crabs);

    std::cout << "part1: " << diff1 << std::endl;
    std::cout << "part2: " << diff2 << std::endl;

    return 0;
}