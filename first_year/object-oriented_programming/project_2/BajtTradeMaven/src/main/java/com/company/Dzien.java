package com.company;

public class Dzien {


    public OfertyKupnaSpekulantow[] OfertyKupnaSpekulantow; //kolejno produkty(0-3), zawsze 100jednostek, tylko ceny
    public OfertySprzedazySpekulantow[] OfertySprzedazySpekulantow; //kolejno produkty(0-3) sortujemy Poziom,Cena,Ilosc
    public OfertaRobotnika[] OfertyRobotnikow; //sortujesz względem typu giełdy, kolejka robotników co pracowali
    // Kolejno Sprzedaż J,U,N,P.
    //Potem Kupno J,U,N,P
    //max długość to ilość wszystkich robotnikow



    //oferty kupna i sprzedaży
    //Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
    private int[] OfertyKupna; //Kolejno Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
    private int[] OfertySprzedazy; //Kolejno Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe

    public int podajIloscOfertKupna(int produkt){
        return this.OfertyKupna[produkt];
    }

    public int podajIloscOfertSprzedazy(int produkt){
        return this.OfertySprzedazy[produkt];
    }

    public int podajIloscOfertLacznie(int produkt){
        return podajIloscOfertKupna(produkt)+podajIloscOfertSprzedazy(produkt);
    }
}
