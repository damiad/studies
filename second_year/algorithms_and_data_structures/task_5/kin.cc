#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1e9;

int father_index(int index)
{
    return index / 2;
}

int lson_index(int index)
{
    return 2 * index;
}

int rson_index(int index)
{
    return 2 * index + 1;
}

bool are_neighbors(int left, int right)
{
    return father_index(left) == father_index(right);
}

void put_into_tree(int size_tree, int index, int val, vector<int>& tree)
{
    int position = index + size_tree;
    while (position > 0) {
        tree[position] = (val + tree[position]) % MOD;
        position = father_index(position);
    }
}

int permutations_count(int index, int size_tree, vector<int>& tree)
{
    int sum = 0;
    int left = size_tree + index + 1;
    int right = 2 * size_tree - 1;

    if (left == right) return tree[left];
    sum = (sum + tree[left]) % MOD;
    sum = (sum + tree[right]) % MOD;

    while (left < right && !are_neighbors(left, right)) {
        if (left == lson_index(father_index(left)))
            sum = (sum + tree[left + 1]) % MOD;
        if (right == rson_index(father_index(right)))
            sum = (sum + tree[right - 1]) % MOD;
        left = father_index(left);
        right = father_index(right);
    }
    return sum;
}

int calculate(const int data[], int n, int size_tree, vector<int>& prev_results,
              int counter)
{
    if (counter == 1) {
        int sol = 0;
        for (int var : prev_results)
            sol = (sol + var) % MOD;
        return sol;
    }

    vector<int> tree(2 * size_tree, 0);
    vector<int> results(n);
    for (int i = 0; i < n; i++) {
        results[i] = permutations_count(data[i] - 1, size_tree, tree);
        put_into_tree(size_tree, data[i] - 1, prev_results[i], tree);
    }

    tree.clear();
    prev_results.clear();
    return calculate(data, n, size_tree, results, counter - 1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;

    int data[n];
    for (int i = 0; i < n; i++)
        cin >> data[i];

    vector<int> results(n, 1);
    int size_tree = 1;
    while (size_tree < n) size_tree *= 2;

    cout << calculate(data, n, size_tree, results, k) << "\n";

    return 0;
}