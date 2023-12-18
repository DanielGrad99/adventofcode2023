#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define NUM_THREADS 5

struct DP {
    std::vector<std::vector<int>> heatloss =
        std::vector<std::vector<int>>(4, std::vector<int>(4, 2147483647));

    std::vector<std::vector<int>> from =
        std::vector<std::vector<int>>(4, std::vector<int>(4, -1));
};

int get_index_dp(int dir_row, int dir_col) {
    assert((dir_col == 1 ^ dir_col == -1) ^ (dir_row == 1 ^ dir_row == -1));
    if (dir_row == 1) {
        return 0;
    } else if (dir_col == 1) {
        return 1;
    } else if (dir_row == -1) {
        return 2;
    } else if (dir_col == -1) {
        return 3;
    }
    assert(false);
}

void set_dir(int index, int& dir_row, int& dir_col) {
    assert(index != -1);
    assert(index >= 0 && index <= 3);
    if (index == 0) {
        dir_row = 1;
        dir_col = 0;
    } else if (index == 1) {
        dir_row = 0;
        dir_col = 1;
    } else if (index == 2) {
        dir_row = -1;
        dir_col = 0;
    } else if (index == 3) {
        dir_row = 0;
        dir_col = -1;
    } else {
        assert(false);
    }
}

int encode_pos(int row, int col) {
    int code = row * 10000 + col;
    assert(col == code % 10000);
    assert(row == code / 10000);
    return code;
}

void decode_pos(int code, int& row, int& col) {
    col = code % 10000;
    row = code / 10000;
}

void Print(const std::vector<std::vector<int>>& map) {
    for (const auto& v : map) {
        for (const auto& n : v) {
            std::cout << n;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Print(const std::vector<std::vector<DP>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            int minValue = map[i][j].heatloss[0][0];  // 2147483647;
            for (const auto& v : map[i][j].heatloss) {
                for (const auto& num : v) {
                    minValue = std::min(minValue, num);
                }
            }
            std::cout << minValue << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void PrintPathTo(const std::vector<std::vector<int>>& map,
                 const std::vector<std::vector<DP>>& dp, int row, int col) {
    std::cout << "PrintPathTo: " << row << ", " << col << std::endl;
    std::vector<int> rows = {row};
    std::vector<int> cols = {col};
    std::vector<int> encoded_pos = {encode_pos(row, col)};

    int lastDirection = -1;

    while (true) {
        int row = rows.back();
        int col = cols.back();

        if (row == 0 && col == 0) {
            std::cout << row << "," << col << std::endl;
            break;
        }

        auto& _dp = dp[row][col];

        int min_1 = (lastDirection + 1) % 4;
        int min_2 = (lastDirection + 1) % 4;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == lastDirection) {
                    continue;
                }

                if (_dp.heatloss[i][j] < _dp.heatloss[min_1][min_2]) {
                    min_1 = i;
                    min_2 = j;
                }
            }
        }

        lastDirection = min_1;

        int dir_row;
        int dir_col;
        set_dir(min_1, dir_row, dir_col);

        std::cout << row << "," << col << " ||| " << dir_row << ", " << dir_col
                  << " ||| " << min_2 << std::endl;

        assert(min_2 != 0);
        for (int i = 0; i < min_2; ++i) {
            row -= dir_row;
            col -= dir_col;

            std::cout << "  " << row << "," << col << std::endl;

            rows.emplace_back(row);
            cols.emplace_back(col);
            encoded_pos.emplace_back(encode_pos(rows.back(), cols.back()));
        }
    }

    int weight = 0;

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (std::find(encoded_pos.begin(), encoded_pos.end(),
                          encode_pos(i, j)) != encoded_pos.end()) {
                std::cout << ".";
                if (i || j) weight += map[i][j];
            } else {
                std::cout << map[i][j];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Weight: " << weight << std::endl << std::endl;
}

struct Path {
    int heatLoss;

    int row;
    int col;

    int dir_row;
    int dir_col;

    int stepsInSameDirection;

    Path() {}
    Path(int heatLoss, int row, int col, int dir_row, int dir_col,
         int stepsInSameDirection)
        : heatLoss(heatLoss),
          row(row),
          col(col),
          dir_row(dir_row),
          dir_col(dir_col),
          stepsInSameDirection(stepsInSameDirection) {}
};

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<std::vector<int>> map;
    std::vector<std::vector<DP>> dp;

    std::string line;
    while (std::getline(inputSS, line)) {
        map.emplace_back(std::vector<int>());
        dp.emplace_back(std::vector<DP>());

        for (char c : line) {
            map.back().emplace_back(c - '0');
            dp.back().push_back(DP());
        }

        map.back().shrink_to_fit();
        dp.back().shrink_to_fit();
    }

    map.shrink_to_fit();
    dp.shrink_to_fit();

    std::stack<Path> paths;
    paths.push(Path(0, 0, 0, 1, 0, 0));
    paths.push(Path(0, 0, 0, 0, 1, 0));
    for (int i = 0; i < 4; ++i) {
        dp[0][0].heatloss[i][0] = 0;
        dp[0][0].heatloss[i][1] = 0;
        dp[0][0].heatloss[i][2] = 0;
        dp[0][0].heatloss[i][3] = 0;
    }

    int bestSolutionSoFar = 2147483647;

    std::mutex dplock_arr[map.size() * map[0].size()];
    std::mutex std_lock;
    std::mutex paths_lock;

    std::mutex* dplock = dplock_arr;

    std::thread threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = std::thread([&map, &dp, &paths, &dplock, &std_lock,
                                  &paths_lock, &bestSolutionSoFar]() {
            const int dest_row = map.size() - 1;
            const int dest_col = map[dest_row].size() - 1;

            while (true) {
                paths_lock.lock();
                if (paths.empty()) {
                    paths_lock.unlock();
                    return;
                }
                Path p = paths.top();
                paths.pop();
                paths_lock.unlock();

                while (true) {
                    if (p.stepsInSameDirection == 3) {
                        break;
                    }

                    int init_pos_encoded = encode_pos(p.row, p.col);

                    p.row += p.dir_row;
                    p.col += p.dir_col;

                    if (p.row < 0 || p.row >= map.size()) {
                        break;
                    }

                    if (p.col < 0 || p.col >= map[p.row].size()) {
                        break;
                    }

                    p.heatLoss += map[p.row][p.col];
                    ++p.stepsInSameDirection;

                    auto& _dplock = dplock[p.row * map.size() + p.col];
                    _dplock.lock();

                    auto& _dp = dp[p.row][p.col];
                    const int dir_index = get_index_dp(p.dir_row, p.dir_col);
                    int movesmade = p.stepsInSameDirection;

                    if (_dp.heatloss[dir_index][movesmade] < p.heatLoss) {
                        _dplock.unlock();
                        break;
                    } else {
                        _dp.heatloss[dir_index][movesmade] = p.heatLoss;
                        _dp.from[dir_index][movesmade] = init_pos_encoded;
                    }

                    bool existsBetter = false;
                    while (movesmade - 1 >= 0) {
                        --movesmade;
                        if (_dp.heatloss[dir_index][movesmade] < p.heatLoss) {
                            existsBetter = true;
                            break;
                        }
                    }

                    _dplock.unlock();

                    if (existsBetter) {
                        break;
                    }

                    std_lock.lock();
                    if (p.row == dest_row && p.col == dest_col) {
                        bestSolutionSoFar =
                            std::min(bestSolutionSoFar, p.heatLoss);
                        std_lock.unlock();
                        break;
                    } else if (bestSolutionSoFar < p.heatLoss) {
                        std_lock.unlock();
                        break;
                    }
                    std_lock.unlock();

                    paths_lock.lock();
                    paths.emplace(p.heatLoss, p.row, p.col, p.dir_col,
                                  p.dir_row, 0);
                    paths.emplace(p.heatLoss, p.row, p.col, p.dir_col * -1,
                                  p.dir_row * -1, 0);
                    paths_lock.unlock();
                }
            }
        });
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        if (threads[i].joinable()) threads[i].join();
    }

    Print(dp);

    PrintPathTo(map, dp, map.size() - 1, map[0].size() - 1);

    for (int i = 0; i < dp.back().back().heatloss.size(); ++i) {
        const auto& _dp = dp.back().back();

        std::cout << _dp.heatloss[0][i] << ", " << _dp.heatloss[1][i] << ", "
                  << _dp.heatloss[2][i] << ", " << _dp.heatloss[3][i]
                  << std::endl;

        // int min = _dp.heatloss[0][i];
        // min = std::min(min, _dp.heatloss[1][i]);
        // min = std::min(min, _dp.heatloss[2][i]);
        // min = std::min(min, _dp.heatloss[3][i]);
        // std::cout << min << std::endl;
    }

    std::cout << "Best Solution: " << bestSolutionSoFar << std::endl;
}