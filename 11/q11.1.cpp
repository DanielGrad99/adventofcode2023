#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Cordinate {
    unsigned int Row;
    unsigned int Col;

    bool operator==(const Cordinate& c) const {
        return Row == c.Row && Col == c.Col;
    }

    struct HashFunction {
        size_t operator()(const Cordinate& c) const {
            size_t xHash = std::hash<int>()(c.Row);
            size_t yHash = std::hash<int>()(c.Col) << 1;
            return xHash ^ yHash;
        }
    };
};

void PrintMap(const std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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

    std::vector<int> colsToExpand;
    std::vector<int> rowsToExpand;

    for (int i = 0; i < map.size(); ++i) {
        bool hasGalaxies = false;
        for (int j = 0; j < map[0].size(); ++j) {
            if (map[i][j] == '#') {
                hasGalaxies = true;
                break;
            }
        }

        if (!hasGalaxies) {
            rowsToExpand.push_back(i);
        }
    }

    for (int j = 0; j < map[0].size(); ++j) {
        bool hasGalaxies = false;
        for (int i = 0; i < map.size(); ++i) {
            if (map[i][j] == '#') {
                hasGalaxies = true;
                break;
            }
        }

        if (!hasGalaxies) {
            colsToExpand.push_back(j);
        }
    }

    PrintMap(map);

    for (int i = rowsToExpand.size() - 1; i >= 0; --i) {
        std::cout << rowsToExpand[i] << " ";
        for (int ii = 0; ii < 1000000; ++ii) {
        map.insert(map.begin() + rowsToExpand[i],
                   std::vector<char>(map[0].size(), '.'));
        }
    }
    std::cout << std::endl;

    for (int j = colsToExpand.size() - 1; j >= 0; --j) {
        std::cout << colsToExpand[j] << " ";
        for (int i = 0; i < map.size(); ++i) {
            for (int ii = 0; ii < 1000000; ++ii) {
                map[i].insert(map[i].begin() + colsToExpand[j], '.');                
            }
        }
    }
    std::cout << std::endl;

    PrintMap(map);

    std::vector<Cordinate> galaxies;

    for (unsigned int i = 0; i < map.size(); ++i) {
        for (unsigned int j = 0; j < map[0].size(); ++j) {
            if (map[i][j] == '#') galaxies.push_back({i, j});
        }
    }

    unsigned int sum = 0;
    unsigned int count = 0;

    for (int i = 0; i < galaxies.size(); ++i) {
        for (int j = 0; j < galaxies.size(); ++j) {
            if (i >= j) {
                continue;
            }

            ++count;

            unsigned int distance = std::max(galaxies[i].Row, galaxies[j].Row) -
                                    std::min(galaxies[i].Row, galaxies[j].Row) +
                                    std::max(galaxies[i].Col, galaxies[j].Col) -
                                    std::min(galaxies[i].Col, galaxies[j].Col);

            std::cout << i << " " << j << ": " << distance << std::endl;
            sum += distance;
        }
    }

    std::cout << "Count: " << count << std::endl;
    std::cout << "Galaxies: " << galaxies.size() << std::endl;

    std::cout << "Sum: " << sum << std::endl;
}