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

void print(std::vector<std::vector<int>> lines) {
    for (const auto& v : lines) {
        for (const auto& i : v) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<int> getNextLine(const std::vector<int>& line) {
    std::vector<int> nextLine;

    for (int i = 0; i < line.size() - 1; ++i) {
        nextLine.emplace_back(line[i + 1] - line[i]);
    }

    return nextLine;
}

bool allZeroes(const std::vector<int>& line) {
    assert(!line.empty());

    for (const auto& n : line) {
        if (n != 0) {
            return false;
        }
    }
    return true;
}

int getNextNumber(std::vector<int> line) {
    std::vector<std::vector<int>> pyramid;
    pyramid.emplace_back(line);

    while (!allZeroes(pyramid.back())) {
        pyramid.emplace_back(getNextLine(pyramid.back()));
    }

    pyramid.back().emplace_back(0);

    for (int i = pyramid.size() - 2; i >= 0; --i) {
        pyramid[i].emplace_back(pyramid[i + 1].back() + pyramid[i].back());
    }

    // print(pyramid);

    return pyramid[0].back();
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<std::vector<int>> lines;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::stringstream ss;
        ss << line;

        std::vector<int> nums;

        int num;
        while (ss >> num) {
            nums.emplace_back(num);
            // std::cout << num << " ";
        }

        // std::cout << std::endl;

        lines.emplace_back(nums);
    }

    print(lines);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


    int sum = 0;
    for (int i = 0; i < lines.size(); ++i) {
        sum += getNextNumber(lines[i]);
    }

    std::cout << "Sum: " << sum << std::endl;
}