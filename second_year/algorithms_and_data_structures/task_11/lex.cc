#include <bits/stdc++.h>
using namespace std;

static const long long p = 37;
vector<long long> power(300008);
// static const long long q = 61277953494854149; //maybe larger
static const long long q = 829442459;
// static const long long q = 10e9 + 7;


// long long multiply(long long a, long long b) {
//     long long res = 0;

//     while (b > 0) {
//         if (b & 1) //nieparszyst
//             res = (res + a) % q;

//         a = (a * 2) % q;
//         b = b >> 1;
//     }
//     return res;
// }

//funkcja dla duzych q nie mnozy poprawnie, a dla małych q ma powtórki...
long long show_hash(vector<long long>& hash, int l, int r) {
    // return (q + hash[l] - multiply(hash[r + 1], power[r - l + 1])) % q;
    return (q + (hash[l] - hash[r + 1] * power[r - l + 1]) % q) % q;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    power[0] = 1;
    for (int i = 1; i < n + 1; i++)
        power[i] = (power[i - 1] * p) % q;

    vector<long long> word;
    char c;
    for (int i = 0; i < n; i++) {
        cin >> c;
        word.push_back(c - 'a' + 1);
    }
    word.push_back(0);

    vector<long long> hash(n + 1);
    hash[n] = 0;
    for (int i = n - 1; i >= 0; i--)
        hash[i] = (word[i] + hash[i + 1] * p) % q;


    int x1, x2, y1, y2;
    int l1, l2;
    int left, right;
    int mid;
    long long v1, v2;
    for (int i = 0; i < m; i++) {
        cin >> x1 >> x2 >> y1 >> y2;
        x1--;
        x2--;
        y1--;
        y2--;
        l1 = x2 - x1;
        l2 = y2 - y1;

        //binsearch maksymalnego wspolnego prefiksu...
        right = min(l1, l2);
        left = 0;
        while (left < right) { //zwraca ostatni element gdzie hashe się równają...
            mid = (left + right + 1) / 2;
            v1 = show_hash(hash, x1, x1 + mid);
            v2 = show_hash(hash, y1, y1 + mid);
            if (v1 != v2) right = mid - 1;
            else left = mid;
        }
        mid = left;

        //jezeli hashe to elemnety też sie nie równają i konczymy
        v1 = word[x1 + mid];
        v2 = word[y1 + mid];
        if (v1 < v2) { cout << "<\n"; continue;}
        else if (v1 > v2) { cout << ">\n"; continue;}
        //elementy i hashe na mid równe, patrzymy na kolejny znak

        if (mid == l1) {
            if (mid == l2) cout << "=\n";
            else cout << "<\n";
            continue;
        } else if (mid == l2) {cout << ">\n"; continue;}
        //to Ci nie dziala niestety...
        //kolejny element taki sam, zatem hash też powiniem byc taki sam!!
        v1 = word[x1 + mid + 1];
        v2 = word[y1 + mid + 1];
        if (v1 < v2) cout << "<\n";
        else if (v1 > v2) cout << ">\n";
        else cout << "Impossible\n";
    }
}

