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

#define NUM_FOLDS 5
#define NUM_THREADS 1

std::mutex lock;
std::mutex cache_lock;

std::unordered_map<std::string, long long> cache;

struct State {
    std::vector<char> states;
    std::vector<int> continous;

    int statesIndex;
    int continousIndex;

    int currentBroken;
    int currentUnknown;
    int requiredBroken;

    int GetRequiredBroken() const {
        int sum = 0;
        for (const auto& i : continous) {
            sum += i;
        }

        return sum;
    }

    int GetCurrentBroken() const {
        int count = 0;
        for (const auto& s : states) {
            if (s == '#') {
                ++count;
            }
        }

        return count;
    }

    int GetCurrentUnkown() const {
        int count = 0;
        for (const auto& s : states) {
            if (s == '?') {
                ++count;
            }
        }

        return count;
    }

    State() noexcept
        : statesIndex(0),
          continousIndex(0),
          currentBroken(0),
          currentUnknown(0),
          requiredBroken(0) {}

    State(std::vector<char> states, std::vector<int> continous) noexcept
        : states(states),
          continous(continous),
          statesIndex(0),
          continousIndex(0),
          currentBroken(0),
          currentUnknown(0),
          requiredBroken(0) {
        states.shrink_to_fit();
        continous.shrink_to_fit();

        currentBroken = GetCurrentBroken();
        currentUnknown = GetCurrentUnkown();
        requiredBroken = GetRequiredBroken();
    }

    State(std::vector<char> states, std::vector<int> continous, int statesIndex,
          int continousIndex, int currentBroken, int currentUnknown,
          int requiredBroken) noexcept
        : states(states),
          continous(continous),
          statesIndex(statesIndex),
          continousIndex(continousIndex),
          currentBroken(currentBroken),
          currentUnknown(currentUnknown),
          requiredBroken(requiredBroken) {}

    State(const State& s) noexcept
        : states(s.states),
          continous(s.continous),
          statesIndex(s.statesIndex),
          continousIndex(s.continousIndex),
          currentBroken(s.currentBroken),
          currentUnknown(s.currentUnknown),
          requiredBroken(s.requiredBroken) {}

    State(State&& s) noexcept
        : states(std::move(s.states)),
          continous(std::move(s.continous)),
          statesIndex(std::move(s.statesIndex)),
          continousIndex(std::move(s.continousIndex)),
          currentBroken(std::move(s.currentBroken)),
          currentUnknown(std::move(s.currentUnknown)),
          requiredBroken(std::move(s.requiredBroken)) {}

    void SetBroken(int i) {
        assert(i >= 0);
        assert(i < states.size());
        assert(states[i] == '?');
        assert(currentUnknown > 0);

        states[i] = '#';
        ++currentBroken;
        --currentUnknown;
    }

    void SetFixed(int i) {
        assert(i >= 0);
        assert(i < states.size());
        assert(states[i] == '?');
        assert(currentUnknown > 0);

        states[i] = '.';
        --currentUnknown;
    }

    bool IsInvalid() const {
        return currentBroken > requiredBroken ||
               currentBroken + currentUnknown < requiredBroken;
    }

    std::string GetEnd() const {
        std::string ret;

        for (int i = statesIndex; i < states.size(); ++i) {
            ret += states[i];
        }

        for (int i = continousIndex; i < continous.size(); ++i) {
            ret += std::to_string(continous[i]) + ",";
        }

        return ret;
    }

    void Print() const {
        for (int i = 0; i < states.size(); ++i) {
            std::cout << states[i] << "";
        }
        std::cout << " ";  // std::endl;

        for (int i = 0; i < continous.size(); ++i) {
            std::cout << continous[i] << ", ";
        }
        std::cout << std::endl;

        std::cout << "statesIndex: " << statesIndex << std::endl;
        std::cout << "continousIndex: " << continousIndex << std::endl;
        std::cout << "currentBroken: " << currentBroken << std::endl;
        std::cout << "currentUnknown: " << currentUnknown << std::endl;
        std::cout << "requiredBroken: " << requiredBroken << std::endl;
        std::cout << std::endl;
    }
};

long long GetNumCombinations(State& state) {
    if (state.IsInvalid()) {
        // std::cout << "INVALID!" << std::endl;
        return 0;
    }

    for (; state.statesIndex < state.states.size(); ++state.statesIndex) {
        if (state.states[state.statesIndex] == '.') {
            continue;
        }

        cache_lock.lock();
        auto cacheResult = cache.find(state.GetEnd());
        if (cacheResult != cache.end()) {
            long long result = (*cacheResult).second;
            cache_lock.unlock();
            return result;
        }
        cache_lock.unlock();

        if (state.states[state.statesIndex] == '.') {
            continue;
        } else if (state.states[state.statesIndex] == '#') {
            int runLength = 0;
            while (runLength < state.continous[state.continousIndex]) {
                if (state.statesIndex >= state.states.size()) {
                    return 0;
                }

                if (state.states[state.statesIndex] == '?') {
                    state.SetBroken(state.statesIndex);
                } else if (state.states[state.statesIndex] == '.') {
                    return 0;
                } else if (state.states[state.statesIndex] == '#') {
                } else {
                    assert(false);
                }

                ++state.statesIndex;
                ++runLength;
            }

            if (state.statesIndex < state.states.size()) {
                if (state.states[state.statesIndex] == '?') {
                    state.SetFixed(state.statesIndex);
                } else if (state.states[state.statesIndex] == '#') {
                    return 0;
                } else if (state.states[state.statesIndex] == '.') {
                } else {
                    assert(false);
                }
            }

            ++state.continousIndex;

        } else if (state.states[state.statesIndex] == '?') {
            std::string cacheKey = state.GetEnd();

            State s = State(state);
            s.SetBroken(state.statesIndex);

            State ss = State(state);
            ss.SetFixed(state.statesIndex);

            long long result = GetNumCombinations(s) + GetNumCombinations(ss);

            cache_lock.lock();
            cache[cacheKey] = result;
            cache_lock.unlock();

            return result;

        } else {
            state.Print();
            assert(false);
        }
    }

    if (state.continousIndex == state.continous.size()) {
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    std::ifstream inputSS;
    if (argc == 1) {
        inputSS.open("input.txt");
    } else {
        std::cout << "Opening " << argv[1] << std::endl;
        inputSS.open(argv[1]);
    }

    std::vector<State> work_stack;

    std::string line;
    while (std::getline(inputSS, line)) {
        std::vector<char> states;
        std::vector<int> continous;

        int index = 0;
        while (line[index] != ' ') {
            states.emplace_back(line[index]);
            ++index;
        }

        line = line.substr(index);

        std::stringstream ss;
        ss << line;

        int num;
        char c;
        while (ss >> num) {
            continous.emplace_back(num);

            if (!(ss >> c && c == ',')) {
                break;
            }
        }

        std::vector<char> final_states;
        std::vector<int> final_continous;

        for (int i = 0; i < NUM_FOLDS; ++i) {
            if (!final_states.empty()) {
                final_states.emplace_back('?');
            }

            for (const auto& s : states) {
                final_states.emplace_back(s);
            }

            for (const auto& c : continous) {
                final_continous.emplace_back(c);
            }
        }

        work_stack.emplace_back(final_states, final_continous);
        if (work_stack.back().IsInvalid()) {
            work_stack.back().Print();
        }
        assert(!work_stack.back().IsInvalid());
    }

    long long sum = 0;
    for (auto s : work_stack) {
        cache.clear();
        long long result = GetNumCombinations(s);

        sum += result;
    }

    std::cout << "Sum: " << sum << std::endl;
}
