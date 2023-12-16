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

#define NUM_FOLDS 1
#define NUM_THREADS 28

std::mutex lock;
std::mutex cache_lock;

std::unordered_map<std::string, long> cache;

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

    std::string GetEnd() {
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

long GetNumCombinations(State& state, std::vector<State>& work_stack) {
    if (state.IsInvalid()) {
        // std::cout << "INVALID!" << std::endl;
        return 0;
    }

    for (; state.statesIndex < state.states.size(); ++state.statesIndex) {
        // cache_lock.lock();
        // auto cacheResult = cache.find(state.GetEnd());
        // if (cacheResult != cache.end()) {
        //     long result = (*cacheResult).second;
        //     cache_lock.unlock();
        //     return result;
        // }
        // cache_lock.unlock();

        if (state.states[state.statesIndex] == '.') {
            continue;
        } else if (state.states[state.statesIndex] == '#') {
            // std::cout << "START RUN" << std::endl;
            // state.Print();

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

            // state.Print();
            // std::cout << "END RUN" << std::endl;

        } else if (state.states[state.statesIndex] == '?') {
            State s = State(state);
            s.SetBroken(state.statesIndex);
            if (!s.IsInvalid()) {
                lock.lock();
                work_stack.emplace_back(s);
                lock.unlock();
            }

            state.SetFixed(state.statesIndex);
            if (state.IsInvalid()) {
                return 0;
            }

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

        break;
    }

    std::thread threads[NUM_THREADS];
    long sums[NUM_THREADS] = {0};

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = std::thread([&sums, &work_stack, i]() {
            while (true) {
                lock.lock();
                if (work_stack.empty()) {
                    // std::cout << "Thread " << i << " exiting" << std::endl;
                    lock.unlock();
                    break;
                }

                State s = work_stack.back();
                work_stack.pop_back();
                
                lock.unlock();

                std::string cacheKey = s.GetEnd();
                long result = GetNumCombinations(s, work_stack);

                sums[i] += result;

                cache_lock.lock();
                std::cout << cacheKey << " ||| " << result << std::endl;
                // cache[cacheKey] = result;
                cache_lock.unlock();
            }
        });
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        if (threads[i].joinable()) threads[i].join();
    }

    long sum = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        sum += sums[i];
    }

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Cache: " << cache["???.###1,1,3,"] << std::endl;
}
