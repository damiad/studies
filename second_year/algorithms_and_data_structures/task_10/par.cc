#include <bits/stdc++.h>
using namespace std;


int bfs(int start, vector<vector<int>>& graph) {
    queue<int> que;
    vector<bool> visited(graph.size(), false); //graph.size=n
    que.push(start);
    visited[start] = true;
    int last = 0;
    while (!que.empty()) {
        last = que.front();
        que.pop();
        for (int val : graph[last])
            if (!visited[val]) {
                que.push(val);
                visited[val] = true;
            }
    }
    return last;
}

void dfs_recursion(vector<int>& stack, vector<bool>& visited,
                   vector<vector<int>>& graph,
                   vector<vector<pair<int, int>>>& questions, vector<int>& answers ) {
    visited[stack.back()] = true;
    for (auto val : questions[stack.back()]) {
        if ((int) stack.size() > val.second)
            answers[val.first] = stack[(stack.size() - 1 - val.second)] + 1;
    }
    for (auto var : graph[stack.back()]) {
        if (!visited[var]) {
            stack.push_back(var);
            dfs_recursion(stack, visited, graph, questions, answers);
        }
    }
    stack.pop_back();
}

void dfs(int start, vector<vector<int>>& graph,
         vector<vector<pair<int, int>>>& questions, vector<int>& answers) {
    vector<int> stack;
    vector<bool> visited(graph.size(), false); //graph.size=n
    stack.push_back(start);
    dfs_recursion(stack, visited, graph, questions, answers);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> graph(n);
    int x, y;
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        if (x != -1) {
            graph[i].push_back(x - 1);
            graph[x - 1].push_back(i);
        }
        if (y != -1) {
            graph[i].push_back(y - 1);
            graph[y - 1].push_back(i);
        }
    }

    //para (numer pytania, odległość)
    vector<vector<pair<int, int>>> questions(n);
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        cin >> x >> y;
        questions[x - 1].push_back(make_pair(i, y));
    }

    // 2 x bfs
    int t_left = bfs(0, graph);
    int t_right = bfs(t_left, graph);

    vector<int> answers(m, -1);
    // 2 x dfs
    dfs(t_left, graph, questions, answers);
    dfs(t_right, graph, questions, answers);

    //wypisuje wynik
    for (int answ : answers)
        cout << answ << "\n";

}