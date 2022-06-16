#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

typedef std::vector<std::vector<int>> grid;

struct priority_pair {
    int priority_value;
    std::pair<int, int> coordinate;

    priority_pair(int priority_value, std::pair<int, int> coordinate) {
        this->priority_value = priority_value;
        this->coordinate = coordinate;
    }

    friend bool operator>(const priority_pair& lhs, const priority_pair& rhs) {
        return lhs.priority_value > rhs.priority_value;
    }
    
};

grid read_input(const std::string& input) {
    grid board;
    std::ifstream infile(input);
    std::string line;
    while (getline(infile, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c - 48);
        }
        board.push_back(row);
    }
    return board;
}

// initialize a 2D array to have same shape as board
grid construct_risk_table(const grid& board) {
    grid risk_table;
    for (auto& row : board) {
        std::vector<int> risk_row(row.size(), 0);
        risk_table.push_back(risk_row);
    }
    return risk_table;
}

// bigger is better, we need the big power
grid make_bigger_board(const grid& smaller_board, int times) {
    grid bigger_board;
    // problematic looping through board while appending rows to it
    grid temp_board;
    
    // expand each existing row by times
    for (auto& row : smaller_board) {
        std::vector<int> bigger_row;
        for (int time = 0; time < times; time++) {
            for (int i = 0; i < row.size(); i++) {
                int value = (row[i] + time) % 9;
                // if value is a multiple of 9, then it's actually 9
                value = value == 0 ? 9 : value;
                bigger_row.push_back(value);
            }
        }
        temp_board.push_back(bigger_row);
    }

    // expand the total amount of rows
    for (int time = 0; time < times; time++) {
        for (auto& temp_row : temp_board) {
            std::vector<int> bigger_row;
            for (int i = 0; i < temp_row.size(); i++) {
                int value = (temp_row[i] + time) %9;
                value = value == 0 ? 9 : value;
                bigger_row.push_back(value);
            }
            bigger_board.push_back(bigger_row);
        }
    }
    
    return bigger_board;
}

bool is_in_bounds(const grid& board, const std::pair<int, int>& coordinate) {
    return coordinate.first >= 0 && coordinate.second >= 0 &&
        coordinate.first < board.size() && coordinate.second < board[coordinate.first].size();
}

int dijkstra(const grid& board, grid& risk_levels) {
    // offsets for searching neighbors of a coordination on the grid
    std::array<std::pair<int, int>, 4> neighbor_delta = 
        {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1)};

    // frontier priority queue for the dijkstra's search
    std::priority_queue<priority_pair, std::vector<priority_pair>, std::greater<priority_pair>> frontier;

    // visited coordinates
    std::unordered_set<std::pair<int, int>,
        boost::hash<std::pair<int, int>>> visited;
    
    // initialize search: start from topper left. Since never entered, risk value is 0.
    frontier.push(priority_pair(0, {0, 0}));

    while (!frontier.empty()) {
        // unpack priority_pair
        int curr_risk_level = frontier.top().priority_value;
        std::pair<int, int> curr = frontier.top().coordinate;
        
        frontier.pop();
        // found lower risk level before, skip
        if (visited.count(curr)) {
            continue;
        } else {
            // mark as visited
            visited.insert(curr);
        }

        risk_levels[curr.first][curr.second] = curr_risk_level;

        // for up, left, down, right
        for (auto& dxy : neighbor_delta) {
            // compute adjacent node
            std::pair<int, int> neighbor = 
                std::make_pair(curr.first + dxy.first, curr.second + dxy.second);
            // if coordinate is on grid, add coordinate to frontier
            if (is_in_bounds(board, neighbor)) {                
                int risk_value = 
                    curr_risk_level + board[neighbor.first][neighbor.second];
                frontier.push(priority_pair(risk_value, neighbor));
            }
        }
    }
    
    // return risk value of bottom right
    return risk_levels[board.size() - 1][board[board.size() - 1].size() - 1];
}

int main() {
    grid board = read_input("./input");
    grid risk_levels = construct_risk_table(board);
    grid bigger_board = make_bigger_board(board, 5);
    grid bigger_risk_levels = construct_risk_table(bigger_board);
    
    std::cout << "part1: " << dijkstra(board, risk_levels) << std::endl;
    std::cout << "part2: " << dijkstra(bigger_board, bigger_risk_levels) << std::endl; 
 
    return 0;
}