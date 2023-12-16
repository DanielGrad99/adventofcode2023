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

    long stats_reads = 0;
    long stats_successful_reads = 0;
    long stats_writes = 0;

    struct Pair {
        std::string map;
        int iterations;
    };

    std::unordered_map<std::string, Pair> cache;

    int i = 0;
    for (; i < 1000000000; ++i) {
        std::string key = to_string(map);

        auto f = cache.find(key);
        ++stats_reads;

        if (f == cache.end() || i + f->second.iterations > 1000000000) {
            Cycle(map);

            std::string newKey = to_string(map);
            auto ff = cache.find(newKey);
            ++stats_reads;

            if (ff == cache.end()) {
                cache[key] = {newKey, 1};
                ++stats_writes;
                continue;
            }

            int distance = 0;
            std::string lastMap;

            while (ff != cache.end() && distance + i + ff->second.iterations < 1000000000) {
                ++stats_successful_reads;

                distance += ff->second.iterations;
                lastMap = ff->second.map;

                ff = cache.find(lastMap);
                ++stats_reads;
            }

            cache[key] = {lastMap, distance + 1};
            ++stats_writes;

            from_string(map, lastMap);
            i += distance;

        } else {
            ++stats_successful_reads;
            from_string(map, f->second.map);
            i += f->second.iterations - 1;
        }
    }
    assert(i == 1000000000);

    // RollToNorth(map);
    // Cycle(map);
    PrintMap(map);

    int load = GetLoad(map);
    std::cout << "Load: " << load << std::endl;

    std::cout << "stats_reads: " << stats_reads << std::endl;
    std::cout << "stats_successful_reads: " << stats_successful_reads
              << std::endl;
    std::cout << "stats_writes: " << stats_writes << std::endl;
}