#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// WARNING: This program assumes square boards and positive values for all boards.
constexpr int BOARD_LENGTH = 5;
constexpr int BOARD_SIZE = BOARD_LENGTH * BOARD_LENGTH;
constexpr int DRAWN = -1;

// read announced draws
std::vector<int> get_draws(std::ifstream& infile) {
    std::vector<int> draws;
    std::string line;
    infile >> line;
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream iss(line);
    for (int draw; iss >> draw;) {
        draws.push_back(draw);
    }
    return draws;
}

// get all boards
std::vector<std::vector<int>> get_boards(std::ifstream& infile) {
    std::vector<std::vector<int>> boards;
    // keep reading input into BOARD_SIZE vectors until EOF
    while (true) {
        std::vector<int> board(BOARD_SIZE);
        for (int i = 0; i < BOARD_SIZE; i++) {
            int number;
            infile >> number;
            if (infile.eof()) {
                return boards;
            }
            board.at(i) = number;
        }
        boards.push_back(board);
    }    
}

void update_board(std::vector<int>& board, int drew) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        // mark announced draw on given board
        if (board.at(i) == drew) {
            board.at(i) = DRAWN;
        }
    }
}

/*
 * Checks whether a board has a bingo or not.
 * @param board: A 1D array which is actually a 2D array.
 *  Uses the row and column indices to perform arithmatic when checking for bingo.                  
 */
bool bingo(const std::vector<int>& board) {    
    // check for row bingos
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            // once a streak is broken, proceed to next row
            if (board.at((i * BOARD_LENGTH) + j) != DRAWN) {
                break;
            }
            if (j == BOARD_LENGTH - 1) {
                return true;
            }
        }
    }

    // check for column bingos
    for (int j = 0; j < BOARD_LENGTH; j++) {
        for (int i = 0; i < BOARD_LENGTH; i++) {
            if (board.at((i * BOARD_LENGTH) + j) != DRAWN) {
                break;
            }
            if (i == BOARD_LENGTH - 1) {
                return true;
            }            
        }
    }
    // if no bingos, return false
    return false;
}

int calc_score(std::vector<int>& board, int multiplier) {
    int score = 0;
    for (auto value : board) {
        if (value != DRAWN) {
            score += value;
        } 
    }
    score *= multiplier;
    return score;
}

int main() {
    // read input: get announced draws and a list of boards
    std::ifstream infile ("./input");
    std::vector<int> draws = get_draws(infile);
    std::vector<std::vector<int>> boards = get_boards(infile);

    // fields for keeping track
    int first_winner_score = -1;
    int last_winner_score = -1;
    int total_boards = boards.size();
    int winners = 0;

    // start game
    for (int i = 0; i < draws.size(); i++) {
        for (auto it = boards.begin(); it != boards.end();) {
            // for each draw, update each board
            update_board((*it), draws.at(i));
            // check bingo
            if (bingo(*it)) {
                // first bingo case
                if (winners == 0) {
                    first_winner_score = calc_score((*it), draws.at(i));
                    std::cout << "part1: " << first_winner_score << std::endl;
                }
                // last bingo case
                if (total_boards - winners == 1) {
                    last_winner_score = calc_score((*it), draws.at(i));
                    std::cout << "part2: " << last_winner_score << std::endl;
                    return 0;
                }
                // remove bingo board from list and increment winners
                it = boards.erase(it);
                winners++;
            } else {
                it++;
            }
        }
    }
    
    // this should never happen...
    std::cerr << "No bingo found." << std::endl;
    return 1;
}