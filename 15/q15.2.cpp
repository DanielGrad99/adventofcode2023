#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Lens {
    std::string label = "";
    int focalLength = 0;
};

struct Box {
    std::vector<Lens> lenses = {};
};

int hash(std::string s) {
    long val = 0;
    for (char c : s) {
        val += (int)c;
        val *= 17;
        val = val % 256;
    }

    assert(0 <= val && val < 256);

    return val;
}

int main() {
    assert(hash("HASH") == 52);

    std::ifstream inputSS;
    inputSS.open("input.txt");

    std::string line;
    std::getline(inputSS, line);

    // Box boxes[256] = {};
    std::vector<Box> boxes = std::vector<Box>(256, Box());

    Lens lens = {};
    for (int i = 0; i < line.size(); ++i) {
        std::string seq = "";
        while (line[i] != '-' && line[i] != '=') {
            assert(line[i] != ',' && i < line.size());
            assert(line[i] >= 'a' && line[i] <= 'z');
            seq += line[i];
            ++i;
        }
        std::cout << seq << line[i];

        int h = hash(seq);

        if (line[i] == '-') {
            std::cout << std::endl;
            int n = 0;
            while (n < boxes[h].lenses.size() &&
                   seq != boxes[h].lenses[n].label) {
                ++n;
            }
            std::cout << "deleting with " << n << " " << boxes[h].lenses.size()
                      << std::endl;

            if (n < boxes[h].lenses.size()) {
                assert(seq == boxes[h].lenses[n].label);
                std::vector<Lens> newVec;
                for (int iii = 0; iii < boxes[h].lenses.size(); ++iii) {
                    if (iii != n) newVec.push_back(boxes[h].lenses[iii]);
                }

                boxes[h].lenses = newVec;
                // boxes[h].lenses.erase(boxes[h].lenses.begin(), boxes[h].lenses.begin() + n + 1);
            }

        } else if (line[i] == '=') {
            ++i;
            assert(line[i] >= '0' && line[i] <= '9');
            int val = line[i] - '0';
            std::cout << val << std::endl;

            for (int ii = 0; ii <= boxes[h].lenses.size(); ++ii) {
                if (ii == boxes[h].lenses.size()) {
                    boxes[h].lenses.emplace_back(seq, val);
                    break;
                } else if (boxes[h].lenses[ii].label == seq) {
                    std::cout << boxes[h].lenses[ii].label << " ";
                    boxes[h].lenses[ii] = {seq, val};
                    break;
                }
            }
            std::cout << std::endl;

            // bool found = false;
            // for (int ii = 0; ii < lenses.size(); ++ii) {
            //     if (lenses[ii].label == seq) {
            //         lenses[ii].focalLength = val;
            //         found = true;
            //         break;
            //     }
            // }

            // if (!found) {
            //     lenses.emplace_back(seq, val);
            // }

        } else {
            assert(false);
        }

        ++i;
        assert(i == line.size() || line[i] == ',');

        // long sum = 0;
        // for (long i = 0; i < 256; ++i) {
        //     for (long j = 0; j < boxes[i].lenses.size(); ++j) {
        //         std::cout << "Box: " << i + 1 << " * " << j + 1 << " * "
        //                   << boxes[i].lenses[j].focalLength << " ||| "
        //                   << boxes[i].lenses[j].label << " "
        //                   << (i + 1) * (j + 1) * boxes[i].lenses[j].focalLength
        //                   << std::endl;
        //         assert(hash(boxes[i].lenses[j].label) == i);
        //         sum += (i + 1) * (j + 1) * boxes[i].lenses[j].focalLength;
        //     }
        // }
        // std::cout << "sum: " << sum << std::endl;
        // std::cout << std::endl;
    }

    std::unordered_set<std::string> s;

    long sum = 0;
    for (long i = 0; i < boxes.size(); ++i) {
        for (long j = 0; j < boxes[i].lenses.size(); ++j) {
            if (s.find(boxes[i].lenses[j].label) != s.end()) {
                std::cout << "PANIC!" << std::endl;
                return 0;
            }
            s.insert(boxes[i].lenses[j].label);

            std::cout << "Box: " << i + 1 << " * " << j + 1 << " * "
                      << boxes[i].lenses[j].focalLength << " ||| "
                      << boxes[i].lenses[j].label << " "
                      << (i + 1) * (j + 1) * boxes[i].lenses[j].focalLength
                      << std::endl;
            assert(hash(boxes[i].lenses[j].label) == i);
            sum += (i + 1) * (j + 1) * boxes[i].lenses[j].focalLength;
        }
    }
    std::cout << "sum: " << sum << std::endl;
}