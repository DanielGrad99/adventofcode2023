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

struct Next {
    int left;
    int right;

    int getNext(int dir) {
        if (dir == 0) {
            return left;
        } else if (dir == 1) {
            return right;
        }

        assert(false);
    }
};

int convertToInt(std::string s) {
    // std::cout << "Token: " << s << " " << s.length()  << " " << (s[0] - 'A') + (s[1] - 'A') * 100 + (s[2] - 'A') * 10000 << std::endl;

    assert(s.length() == 3);
    return (s[0] - 'A') + (s[1] - 'A') * 100 + (s[2] - 'A') * 10000;
}

bool allNodesOnEnd(const std::vector<int>& currentNodes) {
    for (const auto& node : currentNodes) {
        if (node / 10000 != 'Z' - 'A' - 1) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<int> directions;

    std::string line;
    assert(std::getline(inputSS, line));
    std::cout << line << std::endl;

    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == 'L') {
            directions.emplace_back(0);
        } else if (line[i] == 'R') {
            directions.emplace_back(1);
        }
    }

    std::unordered_map<int, Next> map;
    std::vector<int> currentNodes;

    assert(std::getline(inputSS, line));
    while (std::getline(inputSS, line)) {
        std::string token;

        // std::cout << line << std::endl;

        std::string start_s = "AAA";
        std::string left_s = "AAA";
        std::string right_s = "AAA";

        start_s[0] = line[0 + 0];
        start_s[1] = line[0 + 1];
        start_s[2] = line[0 + 2];

        left_s[0] = line[7 + 0];
        left_s[1] = line[7 + 1];
        left_s[2] = line[7 + 2];

        right_s[0] = line[12 + 0];
        right_s[1] = line[12 + 1];
        right_s[2] = line[12 + 2];

        // std::cout << start_s << " " << left_s << " " << right_s << std::endl;

        int s = convertToInt(start_s);

        if (s / 10000 == 'Z' - 'A' - 1) {
            std::cout << start_s << std::endl;
        }

        

        if (start_s[2] == 'A') {
            std::cout << "Starting node: " << start_s << std::endl;
            currentNodes.emplace_back(convertToInt(start_s));
        }

        map[convertToInt(start_s)] = {convertToInt(left_s), convertToInt(right_s)};
    }

    long index = 0;
    while (!allNodesOnEnd(currentNodes)) {
        for (long i = 0; i < currentNodes.size(); ++i) {
            currentNodes[i] = map[currentNodes[i]].getNext(directions[index % directions.size()]);
        }
        ++index;
    }

    std::cout << "Index: " << index << std::endl;
}


/*

Starting node: AAA
    - Length of each loop
    - Number of Zs hit
Starting node: XDA
Starting node: XSA
Starting node: CFA
Starting node: HJA
Starting node: HPA

*/