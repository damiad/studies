#include <bits/stdc++.h>
using namespace std;

class node {
public:
    pair<int, int> col1; //color ilość
    pair<int, int> col2; //color ilość
    node() {
        col1 = make_pair(1, 2);
        col2 = make_pair(3, 2);
    }
    node(int val) {
        col1 = make_pair(val, 1);
        col2 = make_pair(0, 0);
    }
    node(pair<int, int> a, pair<int, int> b) {
        col1 = a;
        col2 = b;
    }
    node(const node& other) = default;
};

class DynamicTree {
private:
    int size_base_tree;
    int n;
    vector<pair<int, int>> range;
    vector<node> ChristmasTree;

    int father_index(int index) {
        return index / 2;
    }

    int lson_index(int index) {
        return 2 * index;
    }

    int rson_index(int index) {
        return 2 * index + 1;
    }

    bool are_neighbors(int left, int right) {
        return father_index(left) == father_index(right);
    }

    node update_node(node& a, node& b) {
        node wrong;
        if (a.col2.second > 1) return wrong;
        if (b.col2.second > 1) return wrong;

        if (a.col2.second == 1) {
            if (b.col2.second == 1)
                return wrong;
            if (a.col1.first == b.col1.first) {
                node t(make_pair(a.col1.first, 2), a.col2);
                return t;
            }
            if (a.col2.first == b.col1.first && a.col1.second == 1) {
                node t(make_pair(a.col2.first, 2), a.col1);
                return t;
            }
            return wrong;
        }
        if (b.col2.second == 1) {
            if (b.col1.first == a.col1.first) {
                node t(make_pair(b.col1.first, 2), b.col2);
                return t;
            }
            if (b.col2.first == a.col1.first && b.col1.second == 1) {
                node t(make_pair(b.col2.first, 2), b.col1);
                return t;
            }
            return wrong;
        }

        if (b.col1.first == a.col1.first) {
            node t(make_pair(a.col1.first, 2), b.col2);
            return t;
        }
        if (a.col1.second >= b.col2.second) {
            node t(a.col1, b.col1);
            return t;
        }
        node t(b.col1, a.col2);
        return t;

    }

    void put_into_tree(int index, int x) {
        int position = index + size_base_tree;
        ChristmasTree[position] = node(x);
        while (position > 1) {
            position = father_index(position);
            ChristmasTree[position] = update_node(ChristmasTree[lson_index(position)],
                                                  ChristmasTree[rson_index(position)]);
        }
    }

    node solution_in_range(int left_index, int right_index) {

        left_index += size_base_tree;
        right_index += size_base_tree;
        node solution(ChristmasTree[left_index]);

        if (left_index == right_index)  return solution;
        solution = update_node(solution, ChristmasTree[right_index]);

        while (left_index < right_index && !are_neighbors(left_index, right_index)) {
            if (left_index == lson_index(father_index(left_index)))
                solution = update_node(solution, ChristmasTree[left_index + 1]);
            if (right_index == rson_index(father_index(right_index)))
                solution = update_node(solution, ChristmasTree[right_index - 1]);
            left_index = father_index(left_index);
            right_index = father_index(right_index);
        }
        return solution;
    }


    void create_range(int index, int& position, vector<vector<int>>& v) {

        int start_position = position;
        for (uint64_t i = 0; i < v[index].size(); i++)
            create_range(v[index][i], position, v);

        range[index - 1] = make_pair(start_position, position);
        position++;
    }

public:
    DynamicTree(int n) {
        this->n = n;
        size_base_tree = 1;
        while (size_base_tree < n)  size_base_tree *= 2;
        ChristmasTree.resize(2 * size_base_tree);
        range.resize(n);
    }

    void construct_preorder(vector<vector<int>>& v) {
        int start = 0; //uzupełnia range
        create_range(1, start, v); //var jest pod indexem range[index].second;
    }

    void update(int number, int value) {
        put_into_tree(range[number - 1].second, value);
    }

    void result(int number) {
        node sol = solution_in_range(range[number - 1].first, range[number - 1].second);
        if (sol.col2.second < 2) cout << "TAK\n";
        else cout << "NIE\n";
    }

};

int main() {

    int n, q;
    cin >> n >> q;
    DynamicTree dTree(n);

    vector<vector<int>> v(n + 1);

    int x;
    for (int i = 2; i < n + 1; i++) {
        cin >> x;
        v[x].push_back(i);
    }
    dTree.construct_preorder(v);

    for (int i = 1; i <= n ; i++) {
        cin >> x;
        dTree.update(i, x); //można przyspieszyć wstawianie 2n zamiastnlogn...
    }

    char c;
    int y;
    for (int i = 0; i < q; i++) {
        cin >> c >> x;
        if (c == '?') dTree.result(x);
        else {
            cin >> y;
            dTree.update(x, y);
        }
    }
}