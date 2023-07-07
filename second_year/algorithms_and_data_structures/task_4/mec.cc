#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, x;
    cin >> n >> m;

    long long teams[n] = {0};
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n / 2; j++) {
            cin >> x;
            teams[x - 1] |= 1LL << i;
        }
        for (int j = 0; j < n / 2; j++)
            cin >> x;
    }

    unordered_set<long long> res(teams, teams + n);
    if ((int) res.size() == n) cout << "TAK\n";
    else cout << "NIE\n";

    return 0;
}