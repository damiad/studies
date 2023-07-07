#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    string s;
    cin >> s;
    int n = s.size();
    int k = n;

    int position = s.find_first_not_of("*");
    if (position == std::string::npos) {
        cout << 1 << "\n";
        return 0;
    }
    char c = s[position];

    for (int i = position + 1; i < n; i++) {
        if (s[i] != '*') {
            if (c != s[i])
                k = min(k, i - position);
            position = i;
            c = s[i];
        }
    }

    cout << n - k + 1 << "\n";

    return 0;
}