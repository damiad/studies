package com.company;

public class SciezkaKariery{
                        //Kolejno Rolnik, Rzemieslnik, Inzynier, Programista, Gornik
    private int[] KarieryInt; //Premia do produkcji kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe, Diamenty
    int AktualnaKariera; //liczba od 0 do 4

    public void uczSie(){
        KarieryInt[AktualnaKariera]++;
    }

    public void zmienKariere(int kariera){
        this.AktualnaKariera=kariera;
    }

    public int podajAktualnaKariere(){
        return this.AktualnaKariera;
    }

//    public int podajPoziom(int produkt){
//        return KarieryInt[produkt];
//    }

    public int podajPremie(int produkt){
       // if(produkt!=AktualnaKariera) return 0;
        if(KarieryInt[produkt]==0) return 0;
        if(KarieryInt[produkt]==1) return 50;
        if(KarieryInt[produkt]==2) return 150;
        if(KarieryInt[produkt]==3) return 300;
        return 300+25*(KarieryInt[produkt]-3);
    }
}
