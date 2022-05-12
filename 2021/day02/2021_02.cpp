#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

int main() {
    std::ifstream infile("./input");
    std::string line;
    std::string command;
    int value {0};
    int aim {0};
    int horizontal_pos {0};
    int depth {0};
    
    while (getline(infile, line)) {
        std::istringstream iss(line);
        iss >> command >> value;
        if (command == "forward") {
            horizontal_pos += value;
            depth += aim * value;
        } else if (command == "up") {
            aim -= value;
        } else if (command =="down") {
            aim += value;
        }
    }

    std::cout << "part 1 output: " << aim * horizontal_pos << std::endl;
    std::cout << "part 2 output: " << depth * horizontal_pos << std::endl;

    return 0;
}