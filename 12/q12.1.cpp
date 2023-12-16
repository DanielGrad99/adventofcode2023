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
#include <utility>
#include <vector>

struct Row {
    std::vector<char> states;
    std::vector<int> continous;

    Row() {}
    Row(std::vector<char> states, std::vector<int> continous)
        : states(states), continous(continous) {}
    Row(const Row& row) : states(row.states), continous(row.continous) {}
    Row(Row&& row) noexcept
        : states(std::move(row.states)), continous(std::move(row.continous)) {}

    int GetTotalBroken() const {
        int sum = 0;
        for (const auto& i : continous) {
            sum += i;
        }

        return sum;
    }

    int GetBrokenFromState() const {
        int count = 0;
        for (const auto& s : states) {
            if (s == '#') {
                ++count;
            }
        }

        return count;
    }

    bool CheckIfValid() const {
        int continousIndex = 0;

        for (int i = 0; i < states.size(); ++i) {
            if (states[i] == '.') {
                continue;
            } else if (states[i] == '#') {
                int currentRun = 0;
                while (states[i] == '#' && i < states.size()) {
                    ++currentRun;
                    ++i;
                }

                if (currentRun == continous[continousIndex]) {
                    ++continousIndex;
                } else {
                    return false;
                }

            } else {
                assert(false);
            }
        }

        return continousIndex == continous.size();
    }

    bool IsInvalid() {
        int continousIndex = 0;

        for (int i = 0; i < states.size(); ++i) {
            if (states[i] == '.') {
                continue;
            } else if (states[i] == '#') {

                bool addedExtra = false;

                int currentRun = 0;
                while (i < states.size() &&
                       (states[i] == '#' || states[i] == '?') &&
                       currentRun < continous[continousIndex]
                    ) {
                    if (states[i] == '?') {
                        addedExtra = true;
                        states[i] = '#';
                    }
                    ++currentRun;
                    ++i;
                }

                if (addedExtra && GetBrokenFromState() > GetTotalBroken()) {
                    return true;
                }

                if (currentRun == continous[continousIndex]) {
                    if (i < states.size()) {
                        if (states[i] == '#') {
                            return true;
                        } else if (states[i] = '?') {
                            states[i] = '.';
                        }
                    } 
                    ++continousIndex;
                } else if (states[i] == '?') {
                    return false;
                } else {
                    return true;
                }

            } else {
                int possiblyBad = 0;
                int definetlyBad = 0;
                for (; i < states.size(); ++i) {
                    if (states[i] == '#') {
                        ++definetlyBad;
                    } else if (states[i] == '?') {
                        ++possiblyBad;
                    }
                }

                int badLeft = 0;
                for (; continousIndex < continous.size(); ++continousIndex) {
                    badLeft += continous[continousIndex];
                }

                if (badLeft < definetlyBad) {
                    return true;
                }

                if (definetlyBad + possiblyBad < badLeft) {
                    return true;
                }

                return false;
            }
        }

        return continousIndex != continous.size();
    }

    void print() const {
        for (int i = 0; i < states.size(); ++i) {
            std::cout << states[i] << "";
        }
        std::cout << " ";  // std::endl;

        for (int i = 0; i < continous.size(); ++i) {
            std::cout << continous[i] << ", ";
        }
        std::cout << std::endl;
    }
};

long GetNumCombinations(Row& row) {
    // row.print();

    int numberOfUnknowns = 0;
    int numberOfBroken = 0;

    for (const auto& c : row.states) {
        if (c == '?') {
            ++numberOfUnknowns;
        } else if (c == '#') {
            ++numberOfBroken;
        }
    }

    // std::cout << "numberOfUnknowns: " << numberOfUnknowns << std::endl;
    // std::cout << "numberOfBroken: " << numberOfBroken << std::endl;
    // std::cout << "GetTotalBroken: " << row.GetTotalBroken() << std::endl;

    assert(numberOfBroken <= row.GetTotalBroken());

    if (numberOfUnknowns == 0) {
        if (row.CheckIfValid()) {
            // row.print();
            return 1;
        } else {
            // assert(false);
            return 0;
        }
    }

    if (numberOfBroken == row.GetTotalBroken()) {
        for (int i = 0; i < row.states.size(); ++i) {
            if (row.states[i] == '?') {
                row.states[i] = '.';
            }
        }

        if (row.CheckIfValid()) {
            // r.print();
            return 1;
        } else {
            return 0;
        }
    }

    // recursion

    long sum = 0;
    for (int i = 0; i < row.states.size(); ++i) {
        if (row.states[i] != '?') {
            continue;
        }

        Row rr = Row(row);

        rr.states[i] = '#';
        if (!rr.IsInvalid()) {
            sum += GetNumCombinations(rr);
        }

        row.states[i] = '.';
        if (!row.IsInvalid()) {
            sum += GetNumCombinations(row);
        }

        return sum;
    }

    assert(false);
}

int main(int argc, char *argv[]) {
    std::ifstream inputSS;
    if (argc == 1) {
        inputSS.open("input.txt");
    } else {
        std::cout << "Opening " << argv[1] << std::endl;
        inputSS.open(argv[1]);
    }

    std::vector<Row> map;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::vector<char> states;
        std::vector<int> continous;

        int index = 0;
        while (line[index] != ' ') {
            states.emplace_back(line[index]);
            ++index;
        }

        line = line.substr(index);

        std::stringstream ss;
        ss << line;

        int num;
        char c;
        while (ss >> num) {
            continous.emplace_back(num);

            if (!(ss >> c && c == ',')) {
                break;
            }
        }

        std::vector<char> final_states;
        std::vector<int> final_continous;

        for (int i = 0; i < 5; ++i) {
            if (!final_states.empty()) {
                final_states.emplace_back('?');
            }

            for (const auto& s : states) {
                final_states.emplace_back(s);
            }

            for (const auto& c : continous) {
                final_continous.emplace_back(c);
            }
        }

        map.emplace_back(final_states, final_continous);
        assert(!map.back().IsInvalid());
    }

    std::thread threads[map.size()];
    long sums[map.size()] = {0};

    for (int i = 0; i < map.size(); ++i) {
        // threads[i] = std::thread([&sums, &map, i]() {
            sums[i] = GetNumCombinations(map[i]);
        // });
    }

    for (int i = 0; i < map.size(); ++i) {
        if (threads[i].joinable()) threads[i].join();
    }

    long sum = 0;
    for (int i = 0; i < map.size(); ++i) {
        std::cout << i + 1 << ") " << sums[i] << std::endl;
        sum += sums[i];
    }

    std::cout << "Sum: " << sum << std::endl;
}
