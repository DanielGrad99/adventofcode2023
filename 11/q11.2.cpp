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
    unsigned long Row;
    unsigned long Col;

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

    int counter = 0;
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {

            if (map[i][j] == '#') {
                std::cout << ++counter;
            } else {
                std::cout << map[i][j];
            }
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

        std::cout << "Rows to expand: ";
    for (int i = 0; i < map.size(); ++i) {
        bool hasGalaxies = false;
        for (int j = 0; j < map[0].size(); ++j) {
            if (map[i][j] == '#') {
                hasGalaxies = true;
                break;
            }
        }

        if (!hasGalaxies) {
            std::cout << i << ", ";
            rowsToExpand.push_back(i);
        }
    }
        std::cout << std::endl;
        std::cout << "Cols to expand: ";

    for (int j = 0; j < map[0].size(); ++j) {
        bool hasGalaxies = false;
        for (int i = 0; i < map.size(); ++i) {
            if (map[i][j] == '#') {
                hasGalaxies = true;
                break;
            }
        }

        if (!hasGalaxies) {
            std::cout << j << ", ";
            colsToExpand.push_back(j);
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

    unsigned long sum = 0;
    unsigned int count = 0;

    for (int i = 0; i < galaxies.size(); ++i) {
        for (int j = 0; j < galaxies.size(); ++j) {
            if (i >= j) {
                continue;
            }

            ++count;

            unsigned long distance = std::max(galaxies[i].Row, galaxies[j].Row) -
                                    std::min(galaxies[i].Row, galaxies[j].Row) +
                                    std::max(galaxies[i].Col, galaxies[j].Col) -
                                    std::min(galaxies[i].Col, galaxies[j].Col);

            unsigned long expansion = 1000000 - 1;
            for (const auto& col : colsToExpand) {
                if (std::max(galaxies[i].Col, galaxies[j].Col) > col &&
                    std::min(galaxies[i].Col, galaxies[j].Col) < col) {
                    distance += expansion;
                }
            }

            for (const auto& row : rowsToExpand) {
                if (std::max(galaxies[i].Row, galaxies[j].Row) > row &&
                    std::min(galaxies[i].Row, galaxies[j].Row) < row) {
                    distance += expansion;
                }
            }

            // std::cout << i + 1 << " " << j + 1 << ": " << distance
            //           << ", max Row: " << std::max(galaxies[i].Row, galaxies[j].Row)
            //           << ", min Row: " << std::min(galaxies[i].Row, galaxies[j].Row)
            //           << ", max Col: " << std::max(galaxies[i].Col, galaxies[j].Col)
            //           << ", min Col: " << std::min(galaxies[i].Col, galaxies[j].Col)
            //           << std::endl;
            sum += distance;
        }
    }

    std::cout << "Count: " << count << std::endl;
    std::cout << "Galaxies: " << galaxies.size() << std::endl;

    std::cout << "Sum: " << sum << std::endl;
}

// 9536038
// 9536038

// 1 6: 16, max Row: 6, min Row: 0, max Col: 9, min Col: 3
// 1 6: 15, max Row: 7, min Row: 0, max Col: 12, min Col: 4

// 


// Rows to expand: 3, 7,
// Cols to expand: 2, 5, 8,

// 1068041782
// 1068489518