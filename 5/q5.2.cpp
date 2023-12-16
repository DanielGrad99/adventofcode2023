#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_set>
#include <vector>

struct Triplet {
    long Dest;
    long Src;
    long Range;
};

struct Range {
    long Start;
    long Range;
};

struct Result {
    long Seed;
    long Location;
};

void Func2() {
    
}

void Func(const std::vector<std::vector<Triplet>*>& vectorsOfTriplets,
          const Range& range, Result& r) {
    
    r.Seed = -1;
    r.Location = -1;

    for (long i = 0; i < range.Range; ++i) {
        long seed = range.Start + i;

        long value = seed;
        // std::cout << value;

        for (const auto& map : vectorsOfTriplets) {
            for (const auto& range : *map) {
                if (range.Src <= value && range.Src + range.Range > value) {
                    value = range.Dest + (value - range.Src);
                    break;
                }
            }

            // std::cout << " <- " << value;
        }

        // std::cout << std::endl;

        if (r.Location == -1 || r.Location > value) {
            r.Seed = seed;
            r.Location = value;
        }
    }
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<Range> seeds;

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

        while (true) {
            Range r;
            if (ss >> r.Start) {
                assert(ss >> r.Range);
                seeds.emplace_back(r);
            } else {
                break;
            }
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

    std::thread threads[seeds.size()];

    std::vector<Result> results (seeds.size());

    for (int i = 0; i < seeds.size(); ++i) {
        // Func(vectorsOfTriplets, seeds[i], results[i]);

        threads[i] = std::thread(Func, vectorsOfTriplets, seeds[i], std::ref(results[i]));
    }

    Result r = {-1, -1};
    for (int i = 0; i < seeds.size(); ++i) {
        assert(threads[i].joinable());
        threads[i].join();

        if (r.Location == -1 || r.Location > results[i].Location) {
            r = results[i];
        }
    }



    // for (const auto& range : seeds) {
        // for (long i = 0; i < range.Range; ++i) {
        //     long seed = range.Start + i;

        //     long value = seed;
        //     // std::cout << value;

        //     for (const auto& map : vectorsOfTriplets) {
        //         for (const auto& range : *map) {
        //             if (range.Src <= value && range.Src + range.Range > value) {
        //                 value = range.Dest + (value - range.Src);
        //                 break;
        //             }
        //         }

        //         // std::cout << " <- " << value;
        //     }

        //     // std::cout << std::endl;

        //     if (bestSeedLocation == -1 || bestSeedLocation > value) {
        //         bestSeed = seed;
        //         bestSeedLocation = value;
        //     }
        // }
    // }

    std::cout << "Best seed = " << r.Seed << " " << r.Location
              << std::endl;
}
