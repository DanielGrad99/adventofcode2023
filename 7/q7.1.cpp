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

int getValue(char c) {
    if (c == 'A') return 12;
    if (c == 'K') return 11;
    if (c == 'Q') return 10;
    if (c == 'J') return 9;
    if (c == 'T') return 8;
    if (c == '9') return 7;
    if (c == '8') return 6;
    if (c == '7') return 5;
    if (c == '6') return 4;
    if (c == '5') return 3;
    if (c == '4') return 2;
    if (c == '3') return 1;
    if (c == '2') return 0;

    assert(false);
}

struct Hand {
    std::string cards;
    int bid;

    int type;

    bool operator<(const Hand& other) const {
        if (type == other.type) {
            for (int i = 0; i < 5; ++i) {
                if (cards[i] != other.cards[i]) {
                    return getValue(cards[i]) < getValue(other.cards[i]);
                }
            }
        }

        return type < other.type;
    }
};

void setType(Hand& h) {
    std::string cards = h.cards;
    std::sort(cards.begin(), cards.end());

    bool allEqual = true;
    for (int i = 1; i < 5; ++i) {
        if (cards[i] != cards[i - 1]) {
            allEqual = false;
            break;
        }
    }

    if (allEqual) {
        h.type = 6;
        return;
    }

    std::unordered_map<char, int> charCount;
    for (char c : cards) {
        charCount[c] = 0;
    }

    for (char c : cards) {
        charCount[c] += 1;
    }

    std::vector<int> counts;
    for (const auto& cc : charCount) {
        counts.emplace_back(cc.second);
    }

    std::sort(counts.begin(), counts.end());
    for (int i = 0; i < counts.size(); ++i) {
        // std::cout << counts[i] << std::endl;
    }

    assert(counts.size() != 0);
    if (counts[0] == 5) {
        assert(counts.size() == 1);
        h.type = 6;
        return;
    }

    if (counts[0] == 1 && counts[1] == 4) {
        assert(counts.size() == 2);
        h.type = 5;
        return;
    }

    if (counts[0] == 2 && counts[1] == 3) {
        assert(counts.size() == 2);
        h.type = 4;
        return;
    }

    if (counts[0] == 1 && counts[1] == 1 && counts[2] == 3) {
        assert(counts.size() == 3);
        h.type = 3;
        return;
    }

    if (counts[0] == 1 && counts[1] == 2 && counts[2] == 2) {
        assert(counts.size() == 3);
        h.type = 2;
        return;
    }

    if (counts[0] == 1 && counts[1] == 1 && counts[2] == 1 && counts[3] == 2) {
        assert(counts.size() == 4);
        h.type = 1;
        return;
    }

    assert(counts[0] == 1 && counts[1] == 1 && counts[2] == 1 &&
           counts[3] == 1 && counts[4] == 1);
    h.type = 0;
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::vector<Hand> hands;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::stringstream ss;
        ss << line;

        Hand h;

        char c;
        for (int i = 0; i < 5; ++i) {
            assert(ss >> c);
            h.cards += c;
        }

        assert(ss >> h.bid);

        setType(h);
        hands.push_back(h);

        std::cout << line << " " << h.type << std::endl;
    }

    std::sort(hands.begin(), hands.end());

    int sum = 0;
    for (int i = 0; i < hands.size(); ++i) {
        std::cout << i << " " << hands[i].cards << " " << hands[i].bid << std::endl;
        sum += (i + 1) * hands[i].bid;
    }

    std::cout << "Sum: " << sum << std::endl; 
}