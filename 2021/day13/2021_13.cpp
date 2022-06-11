#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<bool>> paper_matrix;
typedef std::vector<std::pair<int, int>> coordinates;
typedef std::vector<std::pair<char, int>> commands;

void read_input(const std::string& input, coordinates& dots, commands& cmds) {
    std::ifstream infile(input);
    std::string line;
    while (getline(infile, line)) {
        int comma_index = line.find(",");
        if (comma_index != std::string::npos) {
            int col_val = stoi(line.substr(0, comma_index));
            line.erase(0, comma_index + 1);
            int row_val = stoi(line);
            dots.push_back(std::make_pair(col_val, row_val));
        } else if (line.find("=") != std::string::npos) {
            // get rid of "fold along "
            line.erase(0, 11);
            cmds.push_back(std::make_pair(line[0], stoi(line.substr(2))));
        }
    }
}

paper_matrix construct_paper_matrix(const coordinates& dots, const commands& cmds) {

    // get dimensions of original paper matrix
    int x_dim = -1;
    int y_dim = -1;
    for (auto& cmd : cmds) {
        // only breaks if both x_dim and y_dim have been both initialized
        if (x_dim != -1 && y_dim != -1) {
            break;
        }
        if (cmd.first == 'x' && x_dim == -1) {
            x_dim = cmd.second * 2 + 1;
        } 
        if (cmd.first == 'y' && y_dim == -1) {
            y_dim = cmd.second * 2 + 1;
        }
    }

    std::vector<std::vector<bool>> paper_grid(
        y_dim,
        std::vector<bool>(x_dim, false)
    );

    // plot dots onto paper grid
    for (auto& dot : dots) {
        paper_grid[dot.second][dot.first] = true;
    }
    return paper_grid;
}

void fold_up(paper_matrix& paper_grid, int folding_line) {
    int original_size = paper_grid.size();
    for (int i = 0; i < folding_line; i++) {
        for (int j = 0; j < paper_grid[i].size(); j++) {
            paper_grid[i][j] = paper_grid[i][j] || paper_grid[original_size - 1 - i][j];
        }
    }
    paper_grid.resize(folding_line);
}

void fold_left(paper_matrix& paper_grid, int folding_line) {
    for (auto& row : paper_grid) {
        for (int i = 0; i < folding_line; i++) {
            row[i] = row[i] || row[row.size() - 1 - i];
        }
        row.resize(folding_line);
    }
}

// folds up or left with respect to the command
void fold(paper_matrix& paper_grid, std::pair<char, int>& cmd) {
    if (cmd.first == 'y') {
        fold_up(paper_grid, cmd.second);
    } else if (cmd.first == 'x') {
        fold_left(paper_grid, cmd.second);
    }
}

// counts total amount of dots (trues) on the paper grid
int count_dots(const paper_matrix& paper_grid) {
    int count = 0;
    for (auto& row : paper_grid) {
        for (bool dot_exists : row) {
            if (dot_exists) {
                count++;
            }
        }
    }
    return count;
}

// prints board
void print_paper_grid(const paper_matrix& paper_grid) {
    for (auto& row : paper_grid) {
        for (bool is_dot : row) {
            if (is_dot) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    coordinates dots;
    commands cmds;
    read_input("./input", dots, cmds);

    paper_matrix paper_grid = construct_paper_matrix(dots, cmds);
    
    int ans_pt1 = 0;
    for (int i = 0; i < cmds.size(); i++) {
        fold(paper_grid, cmds[i]);
        if (i == 0) {
            ans_pt1 = count_dots(paper_grid);
        }
    }
        
    std::cout << "part1: " << ans_pt1 << std::endl;
    std::cout << "part2: " << std::endl;
    print_paper_grid(paper_grid);

    return 0;
}