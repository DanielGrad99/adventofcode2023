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

    std::vector<int> copies = std::vector<int>(201, 1);

    std::string line;
    while (std::getline(inputSS, line)) {
        std::cout << line << std::endl;
        std::stringstream ss;
        ss << line;
        
        std::string token;
        int cardNumber;
        int num;
        
        assert(ss >> token);
        std::cout << "1: " << token << std::endl;

        assert(ss >> cardNumber);
        // std::cout << "Card: " << cardNumber << std::endl;
        if (copies[cardNumber] == 0) {
            copies[cardNumber] = 1;
        }
        
        assert(ss >> token);
        // std::cout << "2: " << token << std::endl;

        std::vector<int> my_nums;

        while(ss >> num) {
            my_nums.emplace_back(num);
            // std::cout << num << std::endl;
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
            // std::cout << num << std::endl;
        }

        int matches = 0;
        for (const auto& n : my_nums) {
            std::cout << n << std::endl;
            if (winning_nums.find(n) != winning_nums.end()) {
                matches += 1;
            }
        }

        std::cout << "Card " << cardNumber << ", Matches " << matches << std::endl; 
        for (int i = 0; i < matches; ++i) {
            std::cout << i << std::endl;
            copies[cardNumber + i] = copies[cardNumber + i] + copies[cardNumber - 1];
        }

        // for (int i = 0; i < copies.size(); ++i) {
        //     if (copies[i] == 0) continue;
        //     std::cout << "Card " << i << ", Copies " << copies[i] << std::endl; 
        // }

        // return 0;

        // break;
    }

    for (int i = 0; i < copies.size(); ++i) {
        if (copies[i] == 0) continue;
        std::cout << "Card " << i << ", Copies " << copies[i] << std::endl; 
    }

    sum = 0;
    for (const auto& n : copies) {
        sum += n;
    }

    std::cout << "sum: " << sum << std::endl;
}