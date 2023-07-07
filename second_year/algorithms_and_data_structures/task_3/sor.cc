#include <bits/stdc++.h>
using namespace std;

#define MOD 1000000000

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin>>n;

    int tab[n];
    for(int i=0; i<n; i++)
        cin>>tab[i];

    int L[n][n]={0};
    int R[n][n]={0};

    for(int i=0; i<n; i++){
        L[i][i]=1;
        R[i][i]=1;
    }
    
    for(int i=1; i<n; i++){
        L[i-1][i]=(tab[i-1]<tab[i]);
        R[i-1][i]=(tab[i-1]<tab[i]);
    }

   for(int j=2; j<n; j++){
        for(int i=0; i<n-j; i++){
            L[i][i+j]=((tab[i]<tab[i+1])*L[i+1][i+j]+(tab[i]<tab[i+j])*R[i+1][i+j])%MOD;
            R[i][i+j]=((tab[i+j-1]<tab[i+j])*R[i][i+j-1]+(tab[i]<tab[i+j])*L[i][i+j-1])%MOD;

        }
    }

    cout<<(L[0][n-1]+R[0][n-1]*(n>1))%MOD<<"\n";

    return 0;
}