#include <bits/stdc++.h>
using namespace std;



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);


    int n, m, k;
    cin >> n >> m >> k;

    int price[n];
    for (int i = 0; i < n; i++)
        cin >> price[i];

    int x, y;
    vector<vector<int>> to(n);
    int deq[n] = {0};

    for (int i = 0; i < m; i++) {
        cin >> x >> y;
        deq[x - 1]++;
        to[y - 1].push_back(x - 1);
    }


    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
            que;
    for (int i = 0; i < n; i++) {
        if (deq[i] == 0) que.push(make_pair(price[i], i));
    }

    int result = 0;

    while (k--) {
        auto[x, y] = que.top();
        que.pop();
        result = max(result, x);
        for (auto var : to[y]) {
            deq[var]--;
            if (deq[var] == 0) que.push(make_pair(price[var], var));
        }
    }
    cout << result << "\n";
}