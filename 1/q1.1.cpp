#include <iostream>
#include <fstream>

int main() {

    std::ifstream inputSS;
    inputSS.open("input.txt");

    unsigned int sum = 0;

    std::string line;
    while (inputSS >> line) {
        int first = -1;
        int last = -1;

        for (const char& c : line) {
            if (c >= '0' && c <= '9') {
                if (first == -1) {
                    first = c - '0';
                }

                last = c - '0';
            }
        }

        std::cout << line << " " << first << " " << last << " " << (unsigned int)(first * 10 + last) << " " << sum << std::endl;
        sum += (unsigned int)(first * 10 + last);
    }

    std::cout << sum << std::endl;
}