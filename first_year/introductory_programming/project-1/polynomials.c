#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STOPIEN 10 //Maksymalny stopien wielomianu

void zeruj(int w[]){
     for(int i=0; i<=STOPIEN; i++){
         w[i]=0;
    }
}

void wczytaj_wielomian(int Vielomian[]){
    int znak;
    int wspolczynnik=0;
    int potega=0;
    int plus_minus=1;
    bool wspolczynnik_czy_potega=false; //False to współczynnik True to potęga;
    while((znak=getchar())!='\n'){ //Sprawdzamy moment przejścia do kolejnego wiersza
    //    if(znak==' ' albo ^)  to nic
        if(znak=='-' || znak=='+'){
            Vielomian[potega]=(plus_minus)*wspolczynnik;
            wspolczynnik=0;
            potega=0;
            wspolczynnik_czy_potega=false;  //Znowu wczytujemy najpierw wspólczynnik
            if(znak=='-') plus_minus=-1;
            else plus_minus=1; //Albo wartość będzie ujemna albo dodatnia
        }
        else if(znak>='0' && znak <='9'){  //mamy liczbę
            if(wspolczynnik_czy_potega==false){
                wspolczynnik*=10;
                wspolczynnik+=(znak-'0');
                }
            else{
                 potega=(znak-'0');
                 if(potega==0) potega=10;
            }
        }
        else if(znak=='x'){ // wtedy zaczynamy zliczać potęgę zamiast współczynnika;
            wspolczynnik_czy_potega=true;
            potega=1;
            if(wspolczynnik==0) wspolczynnik=1;
        }
    }
    Vielomian[potega]=(plus_minus)*wspolczynnik;  //teraz wstawiamy  ostatni wspólczynnik w odpowiedniej potędze po wyjściu z while
}

void mnozenie(int w[], int v[]){
    int copyw[STOPIEN+1];
    for(int i=0; i<=STOPIEN; i++){
        copyw[i]=w[i]; //Wileomian pomocniczy
        w[i]=0;     //Zerujemy wielomian główny
    }
    for(int i=0; i<=STOPIEN; i++){
        for(int j=0; j<=STOPIEN-i; j++){   //Bo mamy dane że wynikowy wielomian ma stopień co najwyżej 10
            w[i+j]+=(copyw[i]*v[j]);
        }
    }
}

void dodawanie(int w[], int v[]){
    for(int i=0; i<=STOPIEN; i++){
        w[i]+=v[i];
    }
}

void wypisz(int w[]){ //pomiń 1 lub -1 w wypisywaniu
    bool czy_pierwszy=true;  //Pierwszy niezerowy współczynnik wielomianu (true to pierwszy)
    for(int i=STOPIEN; i>1; i--){
        if(czy_pierwszy==true){
            if(w[i]!=0){
                if(w[i]==1) printf("x^%d",i);
                else if(w[i]==-1) printf("-x^%d",i);
                else printf("%dx^%d",w[i],i);
                czy_pierwszy=false;
            }
        }
        else if(w[i]>0){
             if(w[i]==1) printf(" + x^%d",i);
             else printf(" + %dx^%d",w[i],i);
        }
        else if(w[i]<0){
             if(w[i]==-1) printf(" - x^%d",i);
             else printf(" - %dx^%d",-w[i],i);
        }
    }
    //jeszce osobno w[1] bo tam wypisujemy x zamiast x^1
    if(czy_pierwszy==true){
        if(w[1]!=0){
            if(w[1]==1) printf("x");
            else if(w[1]==-1) printf("-x");
            else printf("%dx",w[1]);
            czy_pierwszy=false;
        }
    }
    else if(w[1]>0){
        if(w[1]==1) printf(" + x");
        else printf(" + %dx",w[1]);
    }
    else if(w[1]<0){
        if(w[1]==-1) printf(" - x");
        else printf(" - %dx",-w[1]);
    }
    // na koniec w[0] bo tam wypisujemy sam współczynnik
    if(czy_pierwszy==true){
        printf("%d",w[0]);
    }
    else if(w[0]>0){
        printf(" + %d",w[0]);
    }
    else if(w[0]<0){
        printf(" - %d",-w[0]);
    }
    printf("\n");
}

int main(void) {

    int Wielomian[STOPIEN+1]; //Wielomian Główny
    zeruj(Wielomian);
    int Vielomian[STOPIEN+1]; //Wielomian z wejścia (dodawany albo mnożony)
    zeruj(Vielomian);
    int dzialanie;
    while((dzialanie=getchar())!='.'){
    wczytaj_wielomian(Vielomian);
    if(dzialanie=='*') mnozenie(Wielomian, Vielomian);// wykonaj dodawanie albo mnozenie
    else dodawanie(Wielomian, Vielomian);               //Jak nie * to +
    wypisz(Wielomian);
    zeruj(Vielomian);
    }
    return 0;
}
