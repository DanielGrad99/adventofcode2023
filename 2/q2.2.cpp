#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

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
        std::stringstream ss;
        ss << line;

        std::string token;
        assert(ss >> token && token == "Game");

        int gameNumber;
        assert(ss >> gameNumber);

        // std::cout << "GameNumber: " << gameNumber << std::endl;

        assert(ss >> token && token == ":");

        std::vector<Round> rounds;

        while (true) {
            Round round = {0, 0, 0};
            while (true) {
                int count = 0;

                if (!(ss >> count)) {
                    goto DONE;
                }
                assert(ss >> token);

                
                char lastChar = token[token.size() - 1];

                if (lastChar == ';' || lastChar == ',') {
                    token = token.substr(0, token.size() - 1);
                } else {
                    lastChar = 0;
                }

                // std::cout << count << " " << token << " " << lastChar << std::endl;

                if (token == "blue") {
                    round.Blue += count;
                } else if (token == "red") {
                    round.Red += count;
                } else if (token == "green") {
                    round.Green += count;
                } else {
                    assert(false);
                }

                if (lastChar == ',') {
                    continue;
                } else if (lastChar == ';' || lastChar == 0) {
                    break;
                }
            }

            // std::cout << "Round " << rounds.size()
            //     << ": Blue " << round.Blue
            //     << ", Red " << round.Red
            //     << ", Green " << round.Green
            //     << std::endl;

            rounds.emplace_back(round);
        }
    DONE:

        Round minRound = {0,0,0};

        std::cout << line << std::endl;
        for (const auto& round : rounds) {
            minRound.Blue = std::max(minRound.Blue, round.Blue);
            minRound.Red = std::max(minRound.Red, round.Red);
            minRound.Green = std::max(minRound.Green, round.Green);
            
            // std::cout << "Round "
            //     << ": Blue " << round.Blue
            //     << ", Red " << round.Red
            //     << ", Green " << round.Green
            //     << std::endl;

            // if (round.Red > 12 || round.Green > 13 || round.Blue > 14) {
            //     possible = false;
            //     break;
            // }
        }

        int power = minRound.Blue * minRound.Red * minRound.Green;
        sum += power;
        std::cout << "sum: " << sum << std::endl;
    }

}