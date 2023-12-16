#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <set>
#include <unordered_set>
#include <cassert>


struct Round {
    int Blue;
    int Red;
    int Green;
};

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    unsigned int sum = 0;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::cout << line << std::endl;
        std::stringstream ss;
        ss << line;
        
        std::string token;
        
        assert(ss >> token);
        std::cout << "1: " << token << std::endl;

        assert(ss >> token);
        std::cout << "2: " << token << std::endl;

        std::vector<int> my_nums;

        int num;

        while(ss >> num) {
            my_nums.emplace_back(num);
            std::cout << num << std::endl;
        }
        assert(my_nums.size() != 0);

        ss.clear();

        // std::cout << ss.peek() << std::endl;

        // char c;
        // while(ss >> c) std::cout << c << std::endl;
        assert(ss >> token);
        std::cout << "3: " << token << std::endl;

        std::set<int> winning_nums;
        while(ss >> num) {
            winning_nums.emplace(num);
            std::cout << num << std::endl;
        }

        int score = 0;
        for (const auto& n : my_nums) {
            std::cout << n << std::endl;
            if (winning_nums.find(n) != winning_nums.end()) {
                if (score == 0) {
                    score = 1;
                } else {
                    score *= 2;
                }
            }
        }

        sum += score;

        // break;
    }
    std::cout << "sum: " << sum << std::endl;
}