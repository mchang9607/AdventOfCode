#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

typedef std::vector<std::vector<int>> i_matrix;
typedef std::vector<std::pair<int, int>> t_vector;
typedef std::list<std::pair<int, int>> t_list;
typedef std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> t_set;

// reads input into a 2d int vector
i_matrix read_input() {
    i_matrix landscape;

    std::ifstream infile("./input");
    std::string line;
    while (getline(infile, line)) {
        std::vector<int> row;
        for (auto c : line) {
            // ascii 0-9 starts from 48
            row.push_back(c - 48);
        }
        landscape.push_back(row);
    }
    return landscape;
}

t_vector find_lows(i_matrix& landscape) {
    t_vector lows;
    for (int i = 0; i < landscape.size(); i++) {
        for (int j = 0; j < landscape[i].size(); j++) {
            bool is_low = true;
            int local = landscape[i][j];
            // if not top, check above
            if (i != 0) {
                if (local >= landscape[i - 1][j]) {
                    is_low = false;
                }
            }
            // if not bottom, check under
            if (i != landscape.size() - 1) {
                if (local >= landscape[i + 1][j]) {
                    is_low = false;
                }
            }
            // if not left, check left
            if (j != 0) {
                if (local >= landscape[i][j - 1]) {
                    is_low = false;
                }
            }
            // if not right, check right
            if (j != landscape[i].size() - 1) {
                if (local >= landscape[i][j + 1]) {
                    is_low = false;
                }
            }
            if (is_low) {
                std::pair<int, int> location(i, j);
                lows.push_back(location);
            }
        }
    }
    return lows;
}

/*
 * The prompt specified that every non 9 location will belong to strictly one basin.
 * This implies that we don't have to care about ordering as long as it is surrounded by
 * a "9" wall.
 */
bool valid_new_neighbor(std::pair<int, int> neighbor, t_set& explored, i_matrix& landscape) {
    if (explored.count(neighbor)) {
        return false;
    }
    if (landscape[neighbor.first][neighbor.second] == 9) {
        return false;
    }
    return true;
}

std::vector<int> find_basin_sizes(i_matrix& landscape, t_vector& lows) {
    std::vector<int> basins;
    for (auto& low : lows) {
        t_list frontier {low};
        t_set explored;
        while (!frontier.empty()) {
            std::pair<int, int> curr = frontier.front();
            frontier.pop_front();            
            if (curr.first != 0) {
                std::pair<int, int> neighbor(curr.first - 1, curr.second);
                if (valid_new_neighbor(neighbor, explored, landscape)) {
                    frontier.push_back(neighbor);    
                }
            }
            if (curr.first != landscape.size() - 1) {
                std::pair<int, int> neighbor(curr.first + 1, curr.second);
                if (valid_new_neighbor(neighbor, explored, landscape)) {
                    frontier.push_back(neighbor);    
                }
            }
            if (curr.second != 0) {
                std::pair<int, int> neighbor(curr.first, curr.second - 1);
                if (valid_new_neighbor(neighbor, explored, landscape)) {
                    frontier.push_back(neighbor);
                }
            }
            if (curr.second != landscape[curr.first].size() - 1) {
                std::pair<int, int> neighbor(curr.first, curr.second + 1);
                if (valid_new_neighbor(neighbor, explored, landscape)) {
                    frontier.push_back(neighbor);    
                }
            }
            explored.insert(curr);
        }
        basins.push_back(explored.size());
    }
    return basins;
}

int calculate_risk_level_sum(i_matrix& landscape, t_vector& lows) {
    int value = 0;
    for (auto low : lows) {
        value += landscape[low.first][low.second];
    }
    value += lows.size();
    return value;
}

int calculate_top3_basin_product(std::vector<int>& basins) {
    int value = 1;

    std::sort(basins.begin(), basins.end());
    for (int i = 0; i < 3; i++) {
        value *= basins[basins.size() - 1 - i];
    }
    
    return value;
}

int main() {
    i_matrix landscape = read_input();
    t_vector lows = find_lows(landscape);
    std::vector<int> basins = find_basin_sizes(landscape, lows);
    
    int ans_pt1 = calculate_risk_level_sum(landscape, lows);
    int ans_pt2 = calculate_top3_basin_product(basins);

    std::cout << "part1: " << ans_pt1 << std::endl;
    std::cout << "part2: " << ans_pt2 << std::endl;

    return 0;
}