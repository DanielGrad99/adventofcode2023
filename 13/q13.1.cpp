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

typedef std::vector<std::string> Map;

bool IsReflectHorizontal(const Map& map, const int row) {
    assert(row >= 0);
    assert(row < map.size() - 1);

    for (int diff = 0; row - diff >= 0 && row + diff + 1 < map.size(); ++diff) {
        for (int c = 0; c < map[0].size(); ++c) {
            if (map[row - diff][c] != map[row + diff + 1][c]) {
                return false;
            }
        }
    }

    return true;
}

bool IsReflectVertical(const Map& map, const int col) {
    assert(col >= 0);
    assert(col < map[0].size() - 1);

    for (int diff = 0; col - diff >= 0 && col + diff + 1 < map[0].size();
         ++diff) {
        for (int r = 0; r < map.size(); ++r) {
            if (map[r][col - diff] != map[r][col + diff + 1]) {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    std::ifstream inputSS;
    if (argc == 1) {
        inputSS.open("input.txt");
    } else {
        std::cout << "Opening " << argv[1] << std::endl;
        inputSS.open(argv[1]);
    }

    std::vector<Map> maps;
    Map curMap;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::cout << line << std::endl;
        if (line.empty()) {
            maps.emplace_back(curMap);
            curMap.clear();
            continue;
        }

        curMap.emplace_back(line);
    }
    maps.emplace_back(curMap);

    long sum = 0;
    for (const auto& map : maps) {
        for (int i = 0; i < map.size() - 1; ++i) {
            if (IsReflectHorizontal(map, i)) {
                sum += 100 * (i + 1);
            }
        }

        for (int i = 0; i < map[0].size() - 1; ++i) {
            if (IsReflectVertical(map, i)) {
                sum += i + 1;
            }
        }
    }

    std::cout << "Sum: " << sum << std::endl;
}