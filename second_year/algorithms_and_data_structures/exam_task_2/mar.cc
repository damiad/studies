//Damian Dąbrowski dd439954
#include <bits/stdc++.h>
using namespace std;

void dfs(int gdzie, vector<int>& answ, vector<bool>& visited,
         vector<vector<int>>& graf) {
    answ.push_back(gdzie);
    visited[gdzie] = true;
    for (auto to : graf[gdzie]) {
        if (visited[to] == false) {
            dfs(to, answ, visited, graf);
            answ.push_back(gdzie);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> answ;
    vector<vector<int>> graf(n + 1);
    vector<bool> visited(n + 1, false);

    int x, y;
    for (int i = 0; i < m; i++) {
        cin >> x >> y;
        if (x != y) {
            graf[x].push_back(y);
            graf[y].push_back(x);
        }
    }

    dfs(1, answ, visited, graf);

    for (int i = 1; i < (int) answ.size(); i++) {
        answ.push_back(answ[i]);
        if (answ[i] == n) break;
    }

    cout << answ.size() << "\n";
    for (auto val : answ)
        cout << val << " ";
    cout << "\n";

}