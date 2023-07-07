#include <bits/stdc++.h>
using namespace std;

class LazyTree {
private:
    int size_base_tree;
    vector<char> Color;
    vector<int> Sum;
    //dużo funkcji

    int lson_index(int index)
    {
        return 2 * index;
    }

    int rson_index(int index)
    {
        return 2 * index + 1;
    }

    void unpack(int index, int left, int right)
    {
        if (Color[index] == '?') return;

        Color[lson_index(index)] = Color[index];
        Color[rson_index(index)] = Color[index];

        if (Color[index] == 'C') {
            Sum[lson_index(index)] = 0;
            Sum[rson_index(index)] = 0;
        } else {//Color[index] == 'B'
            int childs = (right - left + 1) / 2;
            Sum[lson_index(index)] = childs;
            Sum[rson_index(index)] = childs;
        }
    }

    int up(int i, int j, char c, int index, int left, int right)
    {
        if (i <= left && right <= j) {
            Color[index] = c;
            Sum[index] = (c == 'B' ? right + 1 - left : 0);
        } else if (j < left || right < i) {
            //NIC nie robimy
        } else {
            unpack(index, left, right);
            Color[index] = '?';
            int mid = (left + right) / 2;
            Sum[index] = up(i, j, c, lson_index(index), left, mid) + up(i, j, c,
                         rson_index(index), mid + 1, right);
        }
        return Sum[index];
    }

public:
    LazyTree(int n)
    {
        size_base_tree = 1;
        while (size_base_tree < n)  size_base_tree *= 2;
        Color.resize(2 * size_base_tree);
        Sum.resize(2 * size_base_tree);
        Color[1] = 'C';
        Sum[1] = 0;
    }
    int update(int i, int j, char c)
    {
        return up(i, j, c, 1, 1, size_base_tree);
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    LazyTree lTree(n);

    int x, y;
    char c;
    for (int i = 0; i < m; i++) {
        cin >> x >> y >> c;
        cout << lTree.update(x, y, c) << "\n";
    }
    return 0;
}