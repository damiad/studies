#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, int>> X_sorted;
    vector<tuple<int, int, int>> Y_sorted;
    int x;
    int y;
    for (int i = 0; i < n ; i++) {
        cin >> x >> y;
        X_sorted.push_back(make_tuple(x, y, i));
        Y_sorted.push_back(make_tuple(y, x, i));
    }

    sort(X_sorted.begin(), X_sorted.end());
    sort(Y_sorted.begin(), Y_sorted.end());

    vector<vector<pair<int, int>>> graph(n);

    int distance_between;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            distance_between = get<0>(X_sorted[i]) - get<0>(X_sorted[i - 1]);
            graph[get<2>(X_sorted[i])].push_back(make_pair(distance_between,
                                                           get<2>(X_sorted[i - 1])));

            distance_between = get<0>(Y_sorted[i]) - get<0>(Y_sorted[i - 1]);
            graph[get<2>(Y_sorted[i])].push_back(make_pair(distance_between,
                                                           get<2>(Y_sorted[i - 1])));
        }

        if (i < n - 1) {
            distance_between = get<0>(X_sorted[i + 1]) - get<0>(X_sorted[i]);
            graph[get<2>(X_sorted[i])].push_back(make_pair(distance_between,
                                                           get<2>(X_sorted[i + 1])));
            distance_between = get<0>(Y_sorted[i + 1]) - get<0>(Y_sorted[i]);
            graph[get<2>(Y_sorted[i])].push_back(make_pair(distance_between,
                                                           get<2>(Y_sorted[i + 1])));
        }
    }


    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
            que; //distance, wierzchołek

    que.push(make_pair(0, 0)); //pierwszy wierzchołek

    vector<bool> vistited(n, false);
    vector<int> distance(n);

    while (!que.empty() && !vistited[n - 1]) {
        auto[sum_dist, prev_point] = que.top();
        que.pop();
        if (vistited[prev_point]) continue; //juz odwiedzony
        distance[prev_point] = sum_dist;
        vistited[prev_point] = true;
        for (auto[dist, point] : graph[prev_point]) {
            if (!vistited[point])
                que.push(make_pair(sum_dist + dist, point));
        }
    }

    cout << distance[n - 1] << "\n";
}