//Damian Dąbrowski dd439954
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    long long sum = 0;
    long long recent;
    long long val;
    cin >> recent;
    for (int i = 1; i < n; i++) {
        cin >> val;
        if (val > recent) sum += (val - recent);
        recent = val;
    }
    cout << sum << "\n";
}