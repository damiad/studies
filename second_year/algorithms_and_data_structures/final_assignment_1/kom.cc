#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1e9 + 7;
static const pair<int, int> infinity(MOD, 0);

class DynamicTree {
private:
    int size_base_tree;
    int n;
    int k;
    int l;
    vector<pair<int, int>> Representant_Permutation;

    int father_index(int index) {
        return index / 2;
    }

    int lson_index(int index) {
        return 2 * index;
    }

    int rson_index(int index) {
        return 2 * index + 1;
    }

    int neighbor_index(int index) {
        if (index % 2 == 0)
            return index + 1;
        return index - 1;
    }

    bool are_neighbors(int left, int right) {
        return father_index(left) == father_index(right);
    }

    pair<int, int> update_pair(pair<int, int> a, pair<int, int> b) {
        if (a.first < b.first) return a;
        if (b.first < a.first) return b;
        return make_pair(a.first, (a.second + b.second) % MOD);
    }

    void put_into_tree(int index, pair<int, int> p) {
        int position = index + size_base_tree;
        Representant_Permutation[position] = p;
        while (position > 1) {
            pair<int, int> x = Representant_Permutation[position];
            pair<int, int> y = Representant_Permutation[neighbor_index(position)];
            position = father_index(position);
            Representant_Permutation[position] = update_pair(x, y);
        }
    }

    pair<int, int> solution_in_range(int left_index, int right_index) {
        if (left_index > right_index) return infinity;

        pair<int, int> solution;
        left_index += size_base_tree;
        right_index += size_base_tree;

        if (left_index == right_index)  return Representant_Permutation[left_index];
        solution = Representant_Permutation[left_index];
        solution = update_pair(solution, Representant_Permutation[right_index]);

        while (left_index < right_index && !are_neighbors(left_index, right_index)) {
            if (left_index == lson_index(father_index(left_index)))
                solution = update_pair(solution, Representant_Permutation[left_index + 1]);
            if (right_index == rson_index(father_index(right_index)))
                solution = update_pair(solution, Representant_Permutation[right_index - 1]);
            left_index = father_index(left_index);
            right_index = father_index(right_index);
        }
        return solution;
    }


public:
    DynamicTree(int n, int k, int l) {
        this->n = n;
        this->k = k;
        this->l = l;
        size_base_tree = 1;
        while (size_base_tree < n)  size_base_tree *= 2;
        Representant_Permutation.resize(2 * size_base_tree);
        fill(Representant_Permutation.begin(), Representant_Permutation.end(),
             infinity);
    }

    void solution(vector<int>& vec) {

        vec.push_back(-MOD); // guardian to avoid corner cases with lower, uper bound..
        sort(vec.begin(), vec.end());

        put_into_tree(0, make_pair(0, 1));


        for (int i = 1; i <= n; i++) {

            //może zwrócić sztuczne vec[0]
            int last_not_represented_value = *prev(lower_bound(vec.begin(), vec.begin() + i,
                                                   vec[i] - k));

            //może zwrócić sztuczne vec[0]
            int min_possible_index = lower_bound(vec.begin(), vec.begin() + i,
                                                 last_not_represented_value - k) - vec.begin();

            //może zwrócić sztuczne vec[0]
            int max_possible_index = prev(lower_bound(vec.begin(), vec.begin() + i,
                                                      vec[i] - l + 1)) - vec.begin();


            pair<int, int> sol = solution_in_range(min_possible_index, max_possible_index);

            if (sol.first != MOD) {
                sol.first++;
                put_into_tree(i, sol);
            }
            //W przeciwnym razie domyślnie wstawione infinity
        }

        //musi istnieć reprezentant ostatniej grupy
        int min_possible_index = lower_bound(vec.begin(), vec.end(),
                                             vec[n] - k) - vec.begin();


        pair<int, int> ans = solution_in_range(min_possible_index, n);

        cout << ans.first << " " << ans.second << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, l;
    cin >> n >> k >> l;
    DynamicTree dTree(n, k, l);

    vector<int> vec(n);
    for (int i = 0; i < n; i++)
        cin >> vec[i];

    dTree.solution(vec);

    return 0;
}