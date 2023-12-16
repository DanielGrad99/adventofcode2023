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
    std::cout << "Token: " << s << " " << s.length()  << " " << (s[0] - 'A') + (s[1] - 'A') * 100 + (s[2] - 'A') * 10000 << std::endl;

    assert(s.length() == 3);
    return (s[0] - 'A') + (s[1] - 'A') * 100 + (s[2] - 'A') * 10000;
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

    assert(std::getline(inputSS, line));
    while (std::getline(inputSS, line)) {
        std::string token;

        std::cout << line << std::endl;

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

        std::cout << start_s << " " << left_s << " " << right_s << std::endl;

        map[convertToInt(start_s)] = {convertToInt(left_s), convertToInt(right_s)};
    }

    
    int curPos = convertToInt("AAA");
    assert(curPos == 0);

    const int dest = convertToInt("ZZZ");
    assert(dest == 252525);

    int index = 0;
    while (curPos != dest) {
        curPos = map[curPos].getNext(directions[index % directions.size()]);
        ++index;
    }

    std::cout << "Index: " << index << std::endl;
}
