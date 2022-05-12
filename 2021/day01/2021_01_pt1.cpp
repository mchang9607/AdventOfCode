#include <iostream>
#include <fstream>

int main() {
    std::ifstream infile("./input");
    std::string line;
    int counter {0};
    
    getline(infile, line);
    int curr {stoi(line)};
    int prev = curr;

    while (getline(infile, line)) {
        curr = stoi(line);
        if (prev < curr) {
            counter++;
        }
        prev = curr;
    }

    std::cout << "output: " << counter << std::endl;

    return 0;
}