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

struct Ray {
    int row;
    int col;

    int dir_row;
    int dir_col;
};

struct Covered {
    bool covered;

    std::vector<int> dir_row;
    std::vector<int> dir_col;
};

int GetCoverage(std::vector<std::vector<char>> map,
                std::vector<std::vector<Covered>> coverage, Ray startingRay) {
    std::cout << "Starting ray: " << startingRay.row << ", " << startingRay.col << " " << startingRay.dir_row << ", " << startingRay.dir_col << std::endl;
    std::vector<Ray> rays;
    rays.push_back(startingRay);

    while (!rays.empty()) {
        Ray r = rays.back();
        rays.pop_back();

        // std::cout << "New ray: " << r.row << ", " << r.col << ", " <<
        // r.dir_row
        //           << ", " << r.dir_col << ", " << std::endl;

        assert((r.dir_col == 1 ^ r.dir_col == -1) ^
               (r.dir_row == 1 ^ r.dir_row == -1));
        assert(r.dir_row * r.dir_col == 0);

        while (true) {
            // std::cout << "Existing ray: " << r.row << ", " << r.col << ", "
            // << r.dir_row
            //       << ", " << r.dir_col << ", " << std::endl;

            if (r.row < 0 || r.row >= map.size()) {
                break;
            }

            if (r.col < 0 || r.col >= map[r.row].size()) {
                break;
            }

            Covered& c = coverage[r.row][r.col];
            assert(c.dir_row.size() == c.dir_col.size());

            if (c.covered) {
                bool found = false;
                for (int i = 0; i < c.dir_row.size(); ++i) {
                    if (c.dir_row[i] == r.dir_row &&
                        c.dir_col[i] == r.dir_col) {
                        found = true;
                        break;
                    }
                }

                if (found) {
                    break;
                }
            }

            c.covered = true;
            c.dir_row.emplace_back(r.dir_row);
            c.dir_col.emplace_back(r.dir_col);

            if (map[r.row][r.col] == '.') {
            } else if (map[r.row][r.col] == '/') {
                int temp = r.dir_col;
                r.dir_col = r.dir_row * -1;
                r.dir_row = temp * -1;
            } else if (map[r.row][r.col] == '\\') {
                int temp = r.dir_col;
                r.dir_col = r.dir_row;
                r.dir_row = temp;
            } else if (map[r.row][r.col] == '-') {
                if (r.dir_row != 0) {
                    r.dir_row = 0;
                    r.dir_col = 1;

                    rays.emplace_back(r.row, r.col, 0, -1);
                }

            } else if (map[r.row][r.col] == '|') {
                if (r.dir_col != 0) {
                    r.dir_row = 1;
                    r.dir_col = 0;

                    rays.emplace_back(r.row, r.col, -1, 0);
                }
            } else {
                assert(false);
            }

            r.row += r.dir_row;
            r.col += r.dir_col;
        }
    }

    int sum = 0;
    for (const auto& v : coverage) {
        for (const auto& b : v) {
            if (b.covered) {
                std::cout << "#";
                ++sum;
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
        ;
    }
    std::cout << "Sum: " << sum << std::endl;
    std::cout << std::endl;

    return sum;
}

int main(int argc, char* argv[]) {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<std::vector<char>> map;
    std::vector<std::vector<Covered>> coverage;

    std::string line;
    while (std::getline(inputSS, line)) {
        map.emplace_back(std::vector<char>());
        coverage.emplace_back(std::vector<Covered>());
        for (const auto& c : line) {
            map.back().emplace_back(c);
            coverage.back().emplace_back(false, std::vector<int>(),
                                         std::vector<int>());
        }
        map.back().shrink_to_fit();
        coverage.back().shrink_to_fit();
    }

    map.shrink_to_fit();
    coverage.shrink_to_fit();

    int sum = 0;
    for (int i = 0; i < map.size(); ++i) {
        sum = std::max(sum, GetCoverage(map, coverage, Ray(i, 0, 0, 1)));
        sum = std::max(sum, GetCoverage(map, coverage, Ray(i, map[i].size() - 1, 0, -1)));
    }
    for (int i = 0; i < map[0].size(); ++i) {
        sum = std::max(sum, GetCoverage(map, coverage, Ray(0, i, 1, 0)));
        sum = std::max(sum, GetCoverage(map, coverage, Ray(map.size() - 1, i, -1, 0)));
    }
    std::cout << "Sum: " << sum << std::endl;
}
