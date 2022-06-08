#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_map<std::string, std::unordered_set<std::string>> graph_map;

graph_map read_input(const std::string& input_path) {
    graph_map graph;
	
    std::ifstream infile(input_path);
    std::string line;
    while (getline(infile, line)) {
        int pos = line.find("-");
        std::string cave_1 = line.substr(0, pos);
        line.erase(0, pos + 1);
        std::string cave_2 = line;

        graph[cave_1].insert(cave_2);
        graph[cave_2].insert(cave_1);
    }

    return graph;
}

bool is_small_cave(const std::string& cave) {
    return islower(cave[0]);
}

int find_paths_recurively(const std::string& curr_location, graph_map& graph,
std::unordered_set<std::string> visited, bool duplicate, bool is_pt2) {
    // mark as visited
    if (is_small_cave(curr_location)) {
        visited.insert(curr_location);
    }
    // found a path
    if (curr_location == "end") {
        return 1;
    }

    int paths = 0;

    for (auto dest : graph[curr_location]) {
        if (!visited.count(dest)) {
            paths += find_paths_recurively(dest, graph, visited, duplicate, is_pt2);
        } else if (is_pt2 && !duplicate && dest != "start") {
            // if is part 2, also have to take account for paths that allow up to one "non-start" duplicate
            paths += find_paths_recurively(dest, graph, visited, true, is_pt2);
        }
    }

    return paths;
}

int main() {
    graph_map graph = read_input("./input");

    std::unordered_set<std::string> visited;
    int ans_pt1 = find_paths_recurively("start", graph, visited, false, false);
    int ans_pt2 = find_paths_recurively("start", graph, visited, false, true);
    
    std::cout << "part1: " << ans_pt1 << std::endl;
    std::cout << "part2: " << ans_pt2 << std::endl;
    
    return 0;
}
