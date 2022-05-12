#include <iostream>
#include <fstream>
#include <list>

int main() {
    std::ifstream infile("./input");
    std::string line;
    std::list<int> window;
    int counter {0};

    for (int i = 0; i < 3; i++) {
        getline(infile, line);
        window.push_back(stoi(line));
    }

    while (getline(infile, line)) {
        window.push_back(stoi(line));
        if (window.back() > window.front()) {
            counter++;
        }
        window.pop_front();
    }
    
    std::cout << "output: " << counter << std::endl;

    return 0;    
}