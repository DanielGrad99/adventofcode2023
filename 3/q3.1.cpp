#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>



int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    unsigned int sum = 0;

    std::vector<std::string> map;
    std::string line;
    while (std::getline(inputSS, line)) {
        map.emplace_back(line);
    }

    for (int i = 0; i < map.size(); ++i) {
        std::cout << "Line " << i << std::endl;

        std::string line = map[i];
        for (int j = 0; j < line.size(); ++j) {
            std::cout << "Col " << j << std::endl;

            if (line[j] < '0' || line[j] > '9') {
                continue;
            }

            int numberStart = j;
            int numValue = 0;

            while(j < line.size() && line[j] >= '0' && line[j] <= '9') {
                numValue = numValue * 10 + line[j] - '0';
                ++j;
            }

            std::cout << "NumValue " << numValue << std::endl;
            std::cout << "Index " << i << " " << j << " " << numberStart << " " << std::min((unsigned long)(j), line.size() - 1) << std::endl;

            bool adjacentSymbol = false;

            for (int ii = i - 1; ii < i + 2; ++ii) {
                // assert(numberStart - 1 < std::min((unsigned long)(j), line.size() - 1));
                for (int jj = numberStart - 1; jj <= std::min(j, (int)line.size() - 1); ++jj) {
                    std::cout << "Symbol Index (pre) " << ii << " " << jj << std::endl;
                    if (ii < 0 || ii >= map.size()) {
                        continue;
                    }

                    std::string ll = map[ii];

                    if (jj < 0 || jj >= ll.size()) {
                        continue;
                    }

                    char c = ll[jj];
                    std::cout << "Symbol Index " << ii << " " << jj << " " << c << " " << map[ii][jj] << std::endl;

                    if (c >= '0' && c <= '9') {
                        continue;
                    }

                    if (c == '.') {
                        continue;
                    }

                    std::cout << "HOOORAH " << numValue << std::endl;
                    // return 0;
                    adjacentSymbol = true;
                }

                if (adjacentSymbol) {
                    break;
                }
            }

            if (adjacentSymbol) {
                sum += numValue;
            }
        }

        // if (i == 1) {
            // break;
        // }
    }

    std::cout << "sum: " << sum << std::endl;
}