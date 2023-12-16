#include <iostream>
#include <fstream>

int getDigit(const std::string& line, int index) {

    if (line[index] >= '0' && line[index] <= '9') {
        return line[index] - '0';
    }

    const std::string substr = line.substr(index);

    if (substr.starts_with("one")) { return 1; }
    if (substr.starts_with("two")) { return 2; }
    if (substr.starts_with("three")) { return 3; }
    if (substr.starts_with("four")) { return 4; }
    if (substr.starts_with("five")) { return 5; }
    if (substr.starts_with("six")) { return 6; }
    if (substr.starts_with("seven")) { return 7; }
    if (substr.starts_with("eight")) { return 8; }
    if (substr.starts_with("nine")) { return 9; }

    return -1;
}

int main() {

    std::ifstream inputSS;
    inputSS.open("input.txt");

    unsigned int sum = 0;

    std::string line;
    while (inputSS >> line) {
        int first = -1;
        int last = -1;

        for (int i = 0; i < line.size(); ++i) {
            int digit = getDigit(line, i);
            if (digit == -1) {
                continue;
            }

            if (first == -1) {
                first = digit;
            }

            last = digit;
        }

        std::cout << line << " " << first << " " << last << " " << (unsigned int)(first * 10 + last) << " " << sum << std::endl;
        sum += (unsigned int)(first * 10 + last);
    }

    std::cout << sum << std::endl;
}