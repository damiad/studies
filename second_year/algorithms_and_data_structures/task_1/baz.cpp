#include <bits/stdc++.h>
using namespace std;

#define INF 1000000001

struct {
    vector<long long> l_even; //Największa parzysta z lewej
    vector<long long> l_odd; //Największa nieparzysta z lewej
    vector<long long> r_even; //Największa parzysta z prawej
    vector<long long> r_odd; //Największa nieparzysta z prawej
    vector<long long> sufix; //Suma sufixowa
    vector<long long> result; //Ostateczny wynik
} sol;

long long result(long long k){
    return sol.result[k];
}

void create_res(long long n){
    sol.result.push_back(-1);
    long long temp, c1, c2;
    for(long long i=1; i<=n; i++){
        if(sol.sufix[i]%2==1) sol.result.push_back(sol.sufix[i]);
        else{ //Parzysta suma
            temp=n-i;
            c1=INF;
            c2=INF;
            if(sol.l_even[temp]!=INF && sol.r_odd[i]!=INF) 
                c1=sol.r_odd[i]-sol.l_even[temp];
            if(sol.l_odd[temp]!=INF && sol.r_even[i]!=INF) 
                c2=sol.r_even[i]-sol.l_odd[temp];
            
            temp=min(c1,c2); //Zamieniamy mniejszą parę
            if(temp!=INF) sol.result.push_back(sol.sufix[i]-temp);
            else sol.result.push_back(-1);
        }
    }
}

void create_sol(long long n, const long long arr[]){
    
    //Początkowi "strażnicy"
    sol.l_even.push_back(INF); 
    sol.l_odd.push_back(INF);
    sol.r_even.push_back(INF);
    sol.r_odd.push_back(INF);
    sol.sufix.push_back(0);

    long long temp;
    for(long long i=0; i<n; i++){
        if(arr[i]%2==0){
            sol.l_even.push_back(arr[i]);
            sol.l_odd.push_back(sol.l_odd[i]);
        }else{
            sol.l_odd.push_back(arr[i]);
            sol.l_even.push_back(sol.l_even[i]);
        }

        temp=n-1-i; //Analogicznie od końca tablicy
        if(arr[temp]%2==0){
            sol.r_even.push_back(arr[temp]);
            sol.r_odd.push_back(sol.r_odd[i]);
        }else{
            sol.r_odd.push_back(arr[temp]);
            sol.r_even.push_back(sol.r_even[i]);
        }
        sol.sufix.push_back(sol.sufix[i]+arr[temp]);  
    }

    create_res(n); //Uzupełniamy result
}



int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);

    long long n; //Liczba produktów
    cin>>n;
    long long arr[n];

    long long x;
    for(long long i=0; i<n; i++){
        cin>>arr[i];
    }

    create_sol(n, arr); //Wyliczamy wszystkie wyniki od razu

    long long m,k;
    cin>>m;
    for(long long i=0; i<m; i++){
        cin>>k;
        cout<<result(k)<<"\n";
    }
    
    return 0;
}