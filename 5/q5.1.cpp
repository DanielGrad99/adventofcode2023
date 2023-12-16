#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <vector>

struct Triplet {
    long Dest;
    long Src;
    long Range;
};

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<long> seeds;

    std::string line;
    std::string token;
    long num;

    std::vector<Triplet> seedToSoilMap;
    std::vector<Triplet> soilToFertilizer;
    std::vector<Triplet> fertilizerToWater;
    std::vector<Triplet> waterToLight;
    std::vector<Triplet> lightToTemperature;
    std::vector<Triplet> temperatureToHumidity;
    std::vector<Triplet> humidityToLocation;

    std::vector<std::vector<Triplet>*> vectorsOfTriplets = {
        &seedToSoilMap,     &soilToFertilizer,   &fertilizerToWater,
        &waterToLight,      &lightToTemperature, &temperatureToHumidity,
        &humidityToLocation};

    const std::vector<std::string> vectorsOfStrings{
        "seed-to-soil map:",         "soil-to-fertilizer map:",
        "fertilizer-to-water map:",  "water-to-light map:",
        "light-to-temperature map:", "temperature-to-humidity map:",
        "humidity-to-location map:"};
    assert(vectorsOfTriplets.size() == vectorsOfStrings.size());

    assert(std::getline(inputSS, line));
    {
        std::stringstream ss;
        ss << line;

        assert(ss >> token);
        assert(token == "seeds:");

        while (ss >> num) {
            seeds.emplace_back(num);
        }

        assert(std::getline(inputSS, line));
    }

    for (int i = 0; i < vectorsOfStrings.size(); ++i) {
        std::string lineStart = vectorsOfStrings[i];
        std::vector<Triplet>* triplets = vectorsOfTriplets[i];

        std::cout << i << ": " << lineStart << std::endl;

        assert(std::getline(inputSS, line));
        assert(line.starts_with(lineStart));

        while (true) {
            if (!std::getline(inputSS, line) || line == "") {
                break;
            }
            // std::cout << line << std::endl;

            std::stringstream ss;
            ss << line;

            Triplet t;
            assert(ss >> t.Dest);
            assert(ss >> t.Src);
            assert(ss >> t.Range);
            assert(t.Range != 0);

            triplets->emplace_back(t);
        }
    }

    long bestSeed = -1;
    long bestSeedLocation = -1;

    for (const auto& seed : seeds) {
        
        long value = seed;
        std::cout << value;

        for (const auto& map : vectorsOfTriplets) {

            for (const auto& range : *map) {
                if (range.Src <= value && range.Src + range.Range > value) {
                    value = range.Dest + (value - range.Src);
                    break;
                }
            }

            std::cout << " <- " << value;
        }

        std::cout << std::endl;

        if (bestSeedLocation == -1 || bestSeedLocation > value) {
            bestSeed = seed;
            bestSeedLocation = value;
        }
    }

    std::cout << "Best seed = " << bestSeed << " " << bestSeedLocation << std::endl;
}
