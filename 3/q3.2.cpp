#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

struct Pair {
    int i;
    int j;

    bool operator==(const Pair& rhs) {
        return i == rhs.i && j == rhs.j;
    } 
};

int GetNumStart(std::string line, int index) {
    assert(line[index] >= '0' && line[index] <= '9');

    while (line[index] >= '0' && line[index] <= '9') {
        if (index == 0) {
            return index;
        }
        --index;
    }

    return index + 1;
}

int GetNum(std::string line, int index) {
    assert(line[index] >= '0' && line[index] <= '9');
    
    int val = 0;
    while(line[index] >= '0' && line[index] <= '9') {
        val = val * 10 + line[index] - '0';
        ++index;
    }

    return val;
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    unsigned int sum = 0;

    std::vector<std::string> map;
    std::string line;
    while (std::getline(inputSS, line)) {
        map.emplace_back(line);
    }

    for (int i = 0; i < map.size(); ++i) {
        std::cout << "Line " << i << std::endl;

        std::string line = map[i];
        for (int j = 0; j < line.size(); ++j) {
            // std::cout << "Col " << j << std::endl;

            if (line[j] != '*') {
                continue;
            }

            std::vector<Pair> numStarts;
            for (int ii = i - 1; ii < i + 2; ++ii) {
                for (int jj = j - 1; jj < j + 2; ++jj) {
                    if (ii < 0 || ii >= map.size()) {
                        continue;
                    }

                    std::string ll = map[ii];
                    if (jj < 0 || jj >= ll.size()) {
                        continue;
                    }

                    if (ll[jj] < '0' || ll[jj] > '9') {
                        continue;
                    }

                    Pair start = {ii, GetNumStart(ll, jj)};
                    if(std::find(numStarts.begin(), numStarts.end(), start) == numStarts.end()) {
                        numStarts.emplace_back(start);
                    }
                }
            }

            // for (const auto& start : numStarts) {
            //     std::cout << "Num start " << start.i << " " << start.j << std::endl;
            // }

            if (numStarts.size() != 2) {
                continue;
            }

            std::cout << map[numStarts[0].i] << std::endl;
            std::cout << GetNum(map[numStarts[0].i], numStarts[0].j) << std::endl;
            std::cout << map[numStarts[1].i] << std::endl;
            std::cout << GetNum(map[numStarts[1].i], numStarts[1].j) << std::endl;
            std::cout << GetNum(map[numStarts[0].i], numStarts[0].j) * GetNum(map[numStarts[1].i], numStarts[1].j) << std::endl;

            sum += GetNum(map[numStarts[0].i], numStarts[0].j) * GetNum(map[numStarts[1].i], numStarts[1].j);
        }

        // if (i == 1) {
        // break;
        // }
    }

    std::cout << "sum: " << sum << std::endl;
}