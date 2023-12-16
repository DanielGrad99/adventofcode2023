#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

void PrintMap(const std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int GetLoad(const std::vector<std::vector<char>>& map) {
    int load = 0;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] != 'O') {
                continue;
            }

            load += map.size() - i;
        }
    }

    return load;
}

void RollToNorth(std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] != 'O') {
                continue;
            }

            int ii = i - 1;
            while (ii >= 0 && map[ii][j] == '.') {
                --ii;
            }
            ++ii;

            if (ii == i) {
                continue;
            }

            map[ii][j] = 'O';
            map[i][j] = '.';
        }
    }
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<std::vector<char>> map;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::stringstream ss;
        ss << line;

        std::vector<char> row;

        char c;
        while (ss >> c) {
            row.emplace_back(c);
        }

        map.emplace_back(row);
    }

    RollToNorth(map);
    // PrintMap(map);

    int load = GetLoad(map);
    std::cout << "Load: " << load << std::endl;
}