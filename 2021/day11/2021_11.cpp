#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

typedef std::vector<std::vector<int>> i_matrix;

i_matrix read_board(const std::string& file_path) {
    i_matrix board;

    std::ifstream infile(file_path);
    std::string line;
    while (getline(infile, line)) {
        std::vector<int> row;
        for (auto c : line) {
            row.push_back((int)c - 48);
        }
        board.push_back(row);
    }
    return board;
}

/*
 * Given an index and a direction (offset), checks whether:
 * 1. The neighbor in that direction exists (is on the board boundaries)
 * 2. The neighbor hasn't shined yet
 */
bool is_valid_neighbor(i_matrix& board, int row, int column) {
    return (row >= 0 && row < board.size() && column >= 0 && column < board[row].size() && board[row][column] != 0);
}

int simulate(i_matrix& board) {
    const std::array<std::pair<int, int>, 8> dxy 
    {std::make_pair(-1, -1), std::make_pair(-1, 0), std::make_pair(-1, 1), 
    std::make_pair(0, -1), std::make_pair(0, 1),
    std::make_pair(1, -1), std::make_pair(1, 0), std::make_pair(1, 1)};
    constexpr int FLASH = 10;

    for (auto& row : board) {
        for (auto& value : row) {
            value++;
        }
    }

    int count = 0;
    
    bool continue_shine = true;
    while (continue_shine) {
        continue_shine = false;
        // loop through board
        for (int row = 0; row < board.size(); row++) {
            for (int column = 0; column < board[row].size(); column++) {
                // if an octopus shines
                if (board[row][column] >= FLASH) {
                    count++;
                    // might cause other octopus to shine, check in the same simulation
                    continue_shine = true;
                    // increase neighbors that haven't shined this simulation
                    for (auto& offset : dxy) {
                        int neighbor_row = row + offset.first;
                        int neighbor_column = column + offset.second;
                        if (is_valid_neighbor(board, neighbor_row, neighbor_column)) {
                            board[neighbor_row][neighbor_column]++;
                        }
                    }
                    // current octupus shined, reset
                    board[row][column] = 0;
                }
            }
        }
    }
    
    return count;
}

bool is_all_flash(i_matrix& board) {
    constexpr int FLASHED = 0;
    for (auto& row : board) {
        for (auto value : row) {
            if (value != FLASHED) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    i_matrix board = read_board("./input");
    int ans_pt1 = 0;
    int ans_pt2 = 0;
    
    while (true) {
        int flashed = simulate(board);
        if (ans_pt2 < 100) {
            ans_pt1 += flashed;
        }
        
        ans_pt2++;
        if (is_all_flash(board)) {
            break;
        }
    }
    
    std::cout << "part1: " << ans_pt1 << std::endl;
    std::cout << "part2: " << ans_pt2 << std::endl;

    return 0;
}