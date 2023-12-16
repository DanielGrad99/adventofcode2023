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

    bool operator==(const Cordinate& c) const {
        return Row == c.Row && Col == c.Col;
    }

    // bool operator==(Cordinate c) {
    //     return Row == c.Row && Col == c.Col;
    // }

    struct HashFunction {
        size_t operator()(const Cordinate& c) const {
            size_t xHash = std::hash<int>()(c.Row);
            size_t yHash = std::hash<int>()(c.Col) << 1;
            return xHash ^ yHash;
        }
    };
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
    std::cout << std::endl;
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

char GetCell(const std::vector<std::vector<char>>& map, const Cordinate& c) {
    return GetCell(map, c.Row, c.Col);
}

bool IsEdge (const std::vector<std::vector<char>>& map, int row, int col) {
    return row == 0 || col == 0 || row == map.size() - 1 || col == map[0].size() - 1;
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
bool IsEmpty(char c) {
    return c == ',' || c == '.';
}

bool HasPathToEdge(const std::vector<std::vector<char>>& map, int row, int col) {
    assert(map[row][col] == '.');

    std::unordered_set<Cordinate, Cordinate::HashFunction> added;
    std::unordered_set<Cordinate, Cordinate::HashFunction> nextToAdd;

    if (IsEdge(map, row, col)) {
        return true;
    }

    nextToAdd.insert({row, col});

    while (!nextToAdd.empty()) {
        for (const auto& c : nextToAdd) {
            added.insert(c);
        }

        nextToAdd.clear();

        for (const auto& c : added) {
            assert(IsEmpty(GetCell(map, c)));

            if (IsEmpty(GetCell(map, c.Row + 1, c.Col)) && !added.contains({c.Row + 1, c.Col})) { if(IsEdge(map, c.Row + 1, c.Col)) { return true; } nextToAdd.insert({c.Row + 1, c.Col}); }
            if (IsEmpty(GetCell(map, c.Row - 1, c.Col)) && !added.contains({c.Row - 1, c.Col})) { if(IsEdge(map, c.Row - 1, c.Col)) { return true; } nextToAdd.insert({c.Row - 1, c.Col}); }
            if (IsEmpty(GetCell(map, c.Row, c.Col + 1)) && !added.contains({c.Row, c.Col + 1})) { if(IsEdge(map, c.Row, c.Col + 1)) { return true; } nextToAdd.insert({c.Row, c.Col + 1}); }
            if (IsEmpty(GetCell(map, c.Row, c.Col - 1)) && !added.contains({c.Row, c.Col - 1})) { if(IsEdge(map, c.Row, c.Col - 1)) { return true; } nextToAdd.insert({c.Row, c.Col - 1}); }
        }
    }


    return false;
}

void BlowUp(std::vector<std::vector<char>>& map, int row, int col, bool isInside) {
    std::unordered_set<Cordinate, Cordinate::HashFunction> nextToAdd;
    nextToAdd.insert({row, col});

    while (!nextToAdd.empty()) {
        std::unordered_set<Cordinate, Cordinate::HashFunction> nextNextToAdd;
        for (const auto& c : nextToAdd) {
            assert(IsEmpty(GetCell(map, c)));
            if (map[c.Row][c.Col] == '.') {
                map[c.Row][c.Col] = 'I';
            } else if (map[c.Row][c.Col] == ',') {
                map[c.Row][c.Col] = 'i';
            }

            if (IsEdge(map, c.Row, c.Col)) {
                continue;
            }

            if (IsEmpty(GetCell(map, c.Row + 1, c.Col))) { nextNextToAdd.insert({c.Row + 1, c.Col}); }
            if (IsEmpty(GetCell(map, c.Row - 1, c.Col))) { nextNextToAdd.insert({c.Row - 1, c.Col}); }
            if (IsEmpty(GetCell(map, c.Row, c.Col + 1))) { nextNextToAdd.insert({c.Row, c.Col + 1}); }
            if (IsEmpty(GetCell(map, c.Row, c.Col - 1))) { nextNextToAdd.insert({c.Row, c.Col - 1}); }
        }

        nextToAdd = nextNextToAdd;
    }
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

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            Cordinate c;
            c.Row = i;
            c.Col = j;
            if (std::find(path.Coords.begin(), path.Coords.end(), c) == path.Coords.end()) {
                map[i][j] = '.';
            }
        }
    }

    PrintMap(map);

    std::vector<std::vector<char>> enlargedMap;
    for (int i = 0; i < map.size(); ++i) {
        enlargedMap.emplace_back(std::vector<char>());
        for (int j = 0; j < map[i].size(); ++j) {
            enlargedMap.back().emplace_back(map[i][j]);
            enlargedMap.back().emplace_back(',');
        }
        enlargedMap.emplace_back(std::vector<char>(enlargedMap.back().size(), ','));
    }

    PrintMap(enlargedMap);
    for (int i = 0; i < enlargedMap.size(); ++i) {
        for (int j = 0; j < enlargedMap[i].size(); ++j) {
            if (IsValidUp(GetCell(enlargedMap, i - 1, j)) && IsValidDown(GetCell(enlargedMap, i + 1, j))) { enlargedMap[i][j] = '|'; }
            if (IsValidLeft(GetCell(enlargedMap, i, j - 1)) && IsValidRight(GetCell(enlargedMap, i, j + 1))) { enlargedMap[i][j] = '-'; }
        }
    }
    PrintMap(enlargedMap);

    for (int i = 0; i < enlargedMap.size(); ++i) {
        for (int j = 0; j < enlargedMap[i].size(); ++j) {
            if (enlargedMap[i][j] != '.') {
                continue;
            }

            if (!HasPathToEdge(enlargedMap, i, j)) {
                BlowUp(enlargedMap, i, j, true);
            }
        }
    }

    PrintMap(enlargedMap);

    int count = 0;
    for (int i = 0; i < enlargedMap.size(); ++i) {
        for (int j = 0; j < enlargedMap[i].size(); ++j) {
            if (enlargedMap[i][j] == 'I') {
                ++count;
            }
        }
    }

    std::cout << "Ans: " << count << std::endl;


}
