#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

// reads and parses cmd, then uses the extracted values to intialize start and end
void get_command(std::string cmd, std::pair<int, int>& start, std::pair<int, int>& end) {
    std::replace(cmd.begin(), cmd.end(), ',', ' ');
    std::istringstream iss(cmd);
    std::string trash;
    iss >> start.first >> start.second >> trash >> end.first >> end.second;
}

// updates board for horizontal and vertical lines ONLY.
void update_board_part1(std::vector<std::vector<int>>& board, 
    const std::pair<int, int>& start, const std::pair<int, int>& end) {

    // x position identical, vertical line
    if (start.first == end.first) {
        for (int i = std::min(start.second, end.second); i <= std::max(start.second, end.second); i++) {
            board.at(i).at(start.first) += 1;
        }
    // y position identical, horizontal line
    } else if (start.second == end.second) {
        for (int i = std::min(start.first, end.first); i <= std::max(start.first, end.first); i++) {
            board.at(start.second).at(i) += 1;
        }
    }

}

/*
 * Updates the board for diagonal lines ONLY.
 * This function assumes that the board has been updated by part one ahead of time.
 */
void update_board_part2(std::vector<std::vector<int>>& board, 
    const std::pair<int, int>& start, const std::pair<int, int>& end) {
    // single dot lines have been considered in part one, hence are not counted here
    if ((start != end) &&
        std::abs(start.first - end.first) == std::abs(start.second - end.second)) {
        int x_direction = (start.first - end.first > 0) ? -1 : 1;
        int y_direction = (start.second - end.second > 0) ? -1 : 1;
        int iteration_times = std::abs(start.first - end.first) + 1;

        for (int i = 0; i < iteration_times; i++) {
            board.at(start.second + i * y_direction).at(start.first + i * x_direction) += 1;
        }
    }

}


/*
 * Updates the board appropriatly w.r.t. which part of the challenge.
 * Calls update_board_part1 and update_board_part2.
 */
void update_board(std::vector<std::vector<int>>& board, const std::string& cmd, bool is_part_one) {
    // parse command into two pairs: start=(x1, y1) and end=(x2, y2)
    std::pair<int, int> start;
    std::pair<int, int> end;
    get_command(cmd, start, end);

    if (is_part_one) {
        update_board_part1(board, start, end);
    } else {
        update_board_part2(board, start, end);
    }

}

int count_overlap_points(std::vector<std::vector<int>>& board) {
    int count = 0;
    for (const auto& row : board) {
        for (auto value : row) {
            if (value > 1) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    constexpr int BOARD_DIMENSION = 1000;

    std::ifstream infile("./input");
    std::string line;
    std::vector<std::vector<int>> board(BOARD_DIMENSION, std::vector<int>(BOARD_DIMENSION, 0));

    // look for horizontal and vertical lines for part 1
    while(getline(infile, line)) {
        update_board(board, line, true);
    }

    int ans_pt1 = count_overlap_points(board);

    // reset file reader position
    infile.clear();
    infile.seekg(0, std::ios::beg);

    // look for diagonal lines for part 2
    while(getline(infile, line)) {
        update_board(board, line, false);
    }

    int ans_pt2 = count_overlap_points(board);

    std::cout << "part1: " << ans_pt1 << std::endl;
    std::cout << "part2: " << ans_pt2 << std::endl;
    
    return 0;
}