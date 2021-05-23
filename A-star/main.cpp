#include <iostream>
#include <array>
#include <utility>
#include <vector>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <string>
#include <unordered_map>

using namespace std;

enum EDir {
    D_Left = 0,
    D_Right,
    D_Up,
    D_Down
};

size_t CordY(size_t index) {
    return index / 4;
}

size_t CordX(size_t index) {
    return index % 4;
}

size_t GetInd(size_t x, size_t y) {
    return y * 4 + x;
}

size_t ZeroInd(const array<char, 16> &state) {
    size_t null_ind;
    for (null_ind = 0; null_ind < state.size(); null_ind++) {
        if (state[null_ind] == 0)
            break;
    }
    return null_ind;
}

size_t Counting_inv(const array<char, 16> &numbs) {
    size_t count = 0;
    for (size_t i = 0; i < numbs.size(); i++) {
        for (size_t j = i + 1; j < numbs.size(); j++) {
            if (numbs[i] == 0 || numbs[j] == 0) {
                continue;
            }
            if (numbs[j] < numbs[i]) {
                count++;
            }
        }
    }
    return count;
}

bool Solv_checking(const array<char, 16> &start) {
    return (Counting_inv(start) + CordY(ZeroInd(start))) % 2 == 1;
}

void Printing(const array<char, 16> &state) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++)
            cout << (int) state[GetInd(j, i)] << ' ';
        cout << '\n';
    }
}

pair<bool, array<char, 16>> Moving(EDir direc, const array<char, 16> &state) {
    size_t Xzero = CordX(ZeroInd(state));
    size_t Yzero = CordY(ZeroInd(state));
    if (direc == D_Left && Xzero == 0 || direc == D_Right && Xzero == 3
        || direc == D_Up && Yzero == 0 || direc == D_Down && Yzero == 3) {
        return make_pair(false, state);
    }
    size_t new_index = 0;
    if (direc == D_Left) {
        new_index = GetInd(Xzero - 1, Yzero);
    } else if (direc == D_Right) {
        new_index = GetInd(Xzero + 1, Yzero);
    } else if (direc == D_Up) {
        new_index = GetInd(Xzero, Yzero - 1);
    } else if (direc == D_Down) {
        new_index = GetInd(Xzero, Yzero + 1);
    }
    array<char, 16> new_state{};
    new_state = state;
    swap(new_state[new_index], new_state[ZeroInd(state)]);
    return make_pair(true, new_state);
}

unsigned int Manh_dist(const array<char, 16> &state) {
    unsigned int dist = 0;
    for (size_t i = 0; i < state.size(); i++) {
        if (state[i] == 0)
            continue;
        size_t numb = state[i];
        dist = dist + abs((int) CordX(numb - 1) - (int) CordX(i)) + abs((int) CordY(numb - 1) - (int) CordY(i));
    }
    return dist;
}

struct ArrayHash {
    std::size_t operator()(const array<char, 16> &s) const noexcept {
        std::string st;
        for (char ch : s) {
            st += ch;
        }
        return std::hash<std::string>{}(st);
    }
};

vector<array<char, 16>> path_constraction(unordered_map<array<char, 16>, array<char, 16>, ArrayHash> &came_from,
                                          array<char, 16> current) {
    vector<array<char, 16>> path;
    path.push_back(current);
    while (came_from.find(current) != came_from.end()) {
        current = came_from[current];
        path.push_back(current);
    }
    return path;
}

struct Myless{
    template<typename T, typename U> bool operator()(T const& l, U const& r) const{
        return l.second > r.second;
    }
};

vector<array<char, 16>> A_star(const array<char, 16> &start, const array<char, 16> &goal) {
    unordered_map<array<char, 16>, unsigned int, ArrayHash> GarantPath;
    unordered_map<array<char, 16>, unsigned int, ArrayHash> FinalPath;
    priority_queue<pair<array<char, 16>, unsigned int>, vector<pair<array<char, 16>, unsigned int>>, Myless> Q;
    unordered_map<array<char, 16>, array<char, 16>, ArrayHash> came_from;
    Q.push(make_pair(start, Manh_dist(start)));
    GarantPath[start] = 0;
    FinalPath[start] = Manh_dist(start);
    unsigned int tent_score;
    while (!Q.empty()) {
        auto current = Q.top();
        Q.pop();
        if (current.first == goal)
            return path_constraction(came_from, current.first);
        for (EDir d : {D_Left, D_Right, D_Up, D_Down}) { //Смежные с current вершины
            pair<bool, array<char, 16>> pair = Moving(d, current.first);
            if (!pair.first)
                continue;
            tent_score = GarantPath[current.first] + 1;
            if (GarantPath.find(pair.second) == GarantPath.end() ||
                GarantPath.find(pair.second) != GarantPath.end() && tent_score < GarantPath.at(pair.second)) {
                came_from[pair.second] = current.first;
                GarantPath[pair.second] = tent_score;
                FinalPath[pair.second] = tent_score + Manh_dist(pair.second);
                Q.push(make_pair(pair.second, tent_score + Manh_dist(pair.second))); // Возможно необходимо первоначально проверить на уже наличие элемента в очереди
            }
        }
    }
    return vector<array<char, 16>>();
}

int main() {
    array<char, 16> numbers{};
    const array<char, 16> goal{1, 2, 3, 4, 5, 6, 7, 8, 9,
                               10, 11, 12, 13, 14, 15, 0};
    int ch = 0;
    for (char &number : numbers) {
        cin >> ch;
        number = (char) ch;
    }
    if (Solv_checking(numbers))
        cout << A_star(numbers, goal).size()-1;
    else
        cout << "Error";
    return 0;
}
