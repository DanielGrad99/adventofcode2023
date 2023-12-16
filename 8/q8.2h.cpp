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
    std::string left;
    std::string right;

    std::string getNext(int dir) {
        if (dir == 0) {
            return left;
        } else if (dir == 1) {
            return right;
        }

        assert(false);
    }
};

bool allNodesOnEnd(const std::vector<std::string>& currentNodes) {
    for (const auto& node : currentNodes) {
        if (node[2] != 'Z') {
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

    std::unordered_map<std::string, Next> map;
    std::vector<std::string> currentNodes;

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

        if (start_s[2] == 'A') {
            std::cout << "Starting node: " << start_s << std::endl;
            currentNodes.emplace_back(start_s);
        }

        map[start_s] = {left_s, right_s};
    }

    std::cout << "Directions length: " << directions.size() << std::endl;

    for (int i = 0; i < currentNodes.size(); ++i) {
        const std::string startingNode = currentNodes[i];
        std::cout << "Current node: " << startingNode << std::endl;

        std::vector<int> end_nodes;

        std::unordered_set<std::string> visisted;
        visisted.emplace(startingNode + "0");

        std::string curNode = map[startingNode].getNext(directions[0]);
        int index = 1;
        while(visisted.find(curNode + std::to_string(index % directions.size())) == visisted.end()) {
            visisted.emplace(curNode + std::to_string(index % directions.size()));

            if (curNode[2] == 'Z') {
                end_nodes.emplace_back(index);
            }

            curNode = map[curNode].getNext(directions[index % directions.size()]);
            ++index;

            // std::cout << curNode << " " << index % directions.size() << std::endl;
        }

        std::cout << "Distance: " << index << std::endl;
        std::cout << "Final node: " << curNode << std::endl;
        std::cout << "Final index: " << index % directions.size() << std::endl;
        
        for (const auto& index : end_nodes) {
            std::cout << index << " ";
        }
        std::cout << std::endl;

        for (const auto& index : end_nodes) {
            std::cout << index % directions.size() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // int index = 0;
    // while (!allNodesOnEnd(currentNodes)) {
    //     for (int i = 0; i < currentNodes.size(); ++i) {
    //         currentNodes[i] = map[currentNodes[i]].getNext(directions[index % directions.size()]);
    //     }
    //     ++index;
    // }

    // std::cout << "Index: " << index << std::endl;
}