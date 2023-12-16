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

struct Cordinate {
    int Row;
    int Col;

    bool operator==(const Cordinate& c) {
        return Row == c.Row && Col == c.Col;
    }
};

struct Path {
    std::vector<Cordinate> Coords;
    std::string Path;
};

void PrintPath(const Path& path) {
    for (const auto& c : path.Path) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    for (const auto& c : path.Coords) {
        std::cout << "(" << c.Row << "," << c.Col << "), ";
    }
    std::cout << std::endl;
}

void PrintMap(const std::vector<std::vector<char>>& map) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
}

char GetCell(const std::vector<std::vector<char>>& map, int row, int col) {
    if (row < 0 || row >= map.size()) {
        return '.';
    }

    const std::vector<char>& maprow = map[row];

    if (col < 0 || col >= maprow.size()) {
        return '.';
    }

    return maprow[col];
}



bool IsValidUp(char c) {
    return c == 'S' || c == 'F' || c == '|' || c == '7';
}
bool IsValidRight(char c) {
    return c == 'S' || c == 'J' || c == '-' || c == '7';
}
bool IsValidDown(char c) {
    return c == 'S' || c == 'J' || c == '|' || c == 'L';
}
bool IsValidLeft(char c) {
    return c == 'S' || c == 'F' || c == '-' || c == 'L';
}


void AddNextPos(const std::vector<std::vector<char>>& map, Path& path) {
    char lastPlace = path.Path.back();

    int nextRow = path.Coords.back().Row;
    int nextCol = path.Coords.back().Col;


    if (lastPlace == 'S') {

        if (IsValidUp(GetCell(map, nextRow - 1, nextCol))) { nextRow -= 1; }
        else if (IsValidRight(GetCell(map, nextRow, nextCol + 1))) { nextCol += 1; }
        else if (IsValidDown(GetCell(map, nextRow + 1, nextCol))) { nextRow += 1; }
        else if (IsValidLeft(GetCell(map, nextRow, nextCol - 1))) { nextCol -= 1; }
        else { assert (false); }

        path.Path += GetCell(map, nextRow, nextCol);
        path.Coords.push_back({nextRow, nextCol});

        return;
    }

    const int lastRow = path.Coords[path.Coords.size() - 2].Row;
    const int lastCol = path.Coords[path.Coords.size() - 2].Col;

    assert(nextRow == lastRow || nextCol == lastCol);

    if (lastPlace == '|') {
        assert(nextCol == lastCol);

        if (lastRow == nextRow + 1) {
            nextRow = nextRow - 1;
        } else if (lastRow == nextRow - 1) {
            nextRow = nextRow + 1;
        } else {
            assert(false);
        }

    } else if (lastPlace == '-') {
        assert(nextRow == lastRow);

        if (lastCol == nextCol + 1) {
            nextCol = nextCol - 1;
        } else if (lastCol == nextCol - 1) {
            nextCol = nextCol + 1;
        } else {
            assert(false);
        }
    } else if (lastPlace == 'L') {
        if (lastCol == nextCol) {
            nextCol += 1;
        } else if (lastRow == nextRow) {
            nextRow -= 1;
        } else {
            assert(false);
        }
    } else if (lastPlace == 'J') {
        if (lastCol == nextCol) {
            nextCol -= 1;
        } else if (lastRow == nextRow) {
            nextRow -= 1;
        } else {
            assert(false);
        }
    } else if (lastPlace == '7') {
        if (lastCol == nextCol) {
            nextCol -= 1;
        } else if (lastRow == nextRow) {
            nextRow += 1;
        } else {
            assert(false);
        }
    } else if (lastPlace == 'F') {
        if (lastCol == nextCol) {
            nextCol += 1;
        } else if (lastRow == nextRow) {
            nextRow += 1;
        } else {
            assert(false);
        }
    } else if (lastPlace == '.') {
        PrintPath(path);
        assert(false);
    }

    path.Path += GetCell(map, nextRow, nextCol);
    path.Coords.push_back({nextRow, nextCol});
}

int main() {
    std::ifstream inputSS;
    inputSS.open("input.txt");

    int startRow = -1;
    int startCol = -1;

    std::vector<std::vector<char>> map;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::stringstream ss;
        ss << line;

        std::vector<char> row;

        char c;
        while (ss >> c) {
            if (c == 'S') {
                assert(startRow == -1);
                assert(startCol == -1);

                startRow = map.size();
                startCol = row.size();
            }

            row.emplace_back(c);
        }

        map.emplace_back(row);
    }

    assert(startRow != -1);
    assert(startCol != -1);

    Path path = {{{startRow, startCol}}, "S"};
    PrintPath(path);

    do {
        AddNextPos(map, path);
        // PrintPath(path);

        // return 0;
    } while (path.Path.back() != 'S');

    PrintPath(path);

    std::cout << "Ans: " << path.Path.size() / 2 << std::endl;
}
