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

int hash(std::string s) {
    int val = 0;
    for (char c : s) {
        val += (int) c;
        val *= 17;
        val = val % 256;
    }

    return val;
}

int main() {
    assert(hash("HASH") == 52);

    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::string line;
    std::getline(inputSS, line);

    int sum = 0;

    std::string curSeq;
    for (char c : line) {
        if (c == ',') {
            std::cout << curSeq << ": " << hash(curSeq) << std::endl; 
            sum += hash(curSeq);
            curSeq.clear();
        } else {
            curSeq += c;
        }
    }
    sum += hash(curSeq);
    curSeq.clear();

    std::cout << "sum: " << sum << std::endl;

    // std::vector<std::vector<char>> map;

    // 
    // while () {
    //     std::stringstream ss;
    //     ss << line;

    //     std::vector<char> row;

    //     char c;
    //     while (ss >> c) {
    //         row.emplace_back(c);
    //     }

    //     row.shrink_to_fit();
    //     map.emplace_back(row);
    // }
    // map.shrink_to_fit();
}