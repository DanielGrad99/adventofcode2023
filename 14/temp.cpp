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

std::string to_string(const std::vector<std::vector<char>>& map) {
    std::string s;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            s += map[i][j];
        }
    }

    return s;
}

void from_string(std::vector<std::vector<char>>& map, std::string key) {
    int stringIndex = 0;

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            assert(stringIndex < key.size());
            map[i][j] = key[stringIndex++];
        }
    }

    if (stringIndex != key.size()) {
        std::cout << key << std::endl;
    }

    assert(stringIndex == key.size());
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

            assert(map[ii][j] == '.');

            map[ii][j] = 'O';
            map[i][j] = '.';
        }
    }
}

void RollToWest(std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] != 'O') {
                continue;
            }

            int jj = j - 1;
            while (jj >= 0 && map[i][jj] == '.') {
                --jj;
            }
            ++jj;

            if (jj == j) {
                continue;
            }

            assert(map[i][jj] == '.');

            map[i][jj] = 'O';
            map[i][j] = '.';
        }
    }
}

void RollToSouth(std::vector<std::vector<char>>& map) {
    for (int i = map.size() - 1; i >= 0; --i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] != 'O') {
                continue;
            }

            int ii = i + 1;
            while (ii < map.size() && map[ii][j] == '.') {
                ++ii;
            }
            --ii;

            if (ii == i) {
                continue;
            }

            assert(map[ii][j] == '.');

            map[ii][j] = 'O';
            map[i][j] = '.';
        }
    }
}

void RollToEast(std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = map[i].size() - 1; j >= 0; --j) {
            if (map[i][j] != 'O') {
                continue;
            }

            int jj = j + 1;
            while (jj < map[0].size() && map[i][jj] == '.') {
                ++jj;
            }
            --jj;

            if (jj == j) {
                continue;
            }

            assert(map[i][jj] == '.');

            map[i][jj] = 'O';
            map[i][j] = '.';
        }
    }
}

void Cycle(std::vector<std::vector<char>>& map) {
    RollToNorth(map);
    RollToWest(map);
    RollToSouth(map);
    RollToEast(map);
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

        row.shrink_to_fit();
        map.emplace_back(row);
    }
    map.shrink_to_fit();

    std::vector<std::vector<char>> map1 = map;
    int index1 = 0;
    std::vector<std::vector<char>> map2 = map;
    int index2 = 0;

    do {
        index1 += 1;
        Cycle(map1);

        index2 += 2;
        Cycle(map2);
        Cycle(map2);
    } while (to_string(map1) != to_string(map2));

    std::cout << "Meeting index1: " << index1 << std::endl;
    std::cout << "Meeting index2: " << index2 << std::endl;

    map1 = map;
    index1 = 0;
    while(to_string(map1) != to_string(map2)) {
        ++index1;
        Cycle(map1);
        Cycle(map2);
    }

    std::cout << "Starting index: " << index1 << std::endl;
    const int starting_index = index1;

    index2 = 0;
    do {
        ++index2;
        Cycle(map2);
    } while(to_string(map1) != to_string(map2));

    std::cout << "Length of loop is " << index2 << std::endl;
    const int cycle_length = index2;

    int i = starting_index + cycle_length * ((1000000000 - starting_index) / cycle_length);
    std::cout << "i: " << i << std::endl;
    for (; i < 1000000000; ++i) {
        Cycle(map1);
    }

    std::cout << "Load: " << GetLoad(map1) << std::endl;
}