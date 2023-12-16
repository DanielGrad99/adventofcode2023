#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_set>
#include <vector>

struct Race {
    long Time;
    long Distance;
};

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<Race> races;

    std::string token;

    std::string timeLine;
    std::string distanceLine;

    assert(std::getline(inputSS, timeLine));
    assert(std::getline(inputSS, distanceLine));

    std::cout << timeLine << std::endl;
    std::cout << distanceLine << std::endl;

    std::stringstream time_ss;
    std::stringstream distance_ss;

    time_ss << timeLine;
    distance_ss << distanceLine;

    assert(time_ss >> token);
    assert(token == "Time:");
    assert(distance_ss >> token);
    assert(token == "Distance:");
    
    long time;
    long distance;

    while (time_ss >> time) {
        assert(distance_ss >> distance);
        races.push_back({time, distance});
    }

    long product = 1;

    for (const auto& r : races) {
        std::cout << "Race: " << r.Time << " " << r.Distance << std::endl;

        long numWaysToWin = 0;
        for (long duration = 0; duration <= r.Time; ++duration) {
            long distance = duration * (r.Time - duration);
            if (distance > r.Distance) {
                // std::cout << "\t" << duration << std::endl;
                ++numWaysToWin;
            }
        }

        product *= numWaysToWin;

        if (product == 0) {
            break;
        }
    }

    std::cout << "Prod: " << product << std::endl;

}