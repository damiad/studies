package com.company;

public abstract class Gielda {
    protected final int IloscTur;
    protected int ObecnyDzien;

    protected final int IloscRobotnikow;
    protected final int IloscSpekulantow;

    protected double [][] Ceny; //Średnie Ceny (dzień 0, dzień 1...) Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
//    public static Dzien[] Dzien; // historia dni na giełdzie
//prawdopodobnie należy odkomentować klasę Dzień i do niej wrzucić większość operacji wykonywanych.
//przekopiowanie tam niektórych tu napisanych metod nie stanowi problemu.
// Wtedy możnaby dać tam toJson i wypisywać co turę
    protected Robotnik[] Robotnicy;
    protected Spekulant[] Spekulanci;
    protected int KaraZaBrakUbran;
    protected int[] MaksOfert; //Maksymalna Ilosc Ofert: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
//    protected double[][] MinimalnaCenaKupnaProduktuWczoraj; //Minimalna: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe

    //WRZUĆ TO DO DNIA :) dorób te klasy, wywal pozostałe //Sortuj ilośc diamentów, id
    protected OfertyKupnaSpekulantow[] OfertyKupnaSpekulantow; //kolejno produkty(0-3), zawsze 100jednostek, tylko ceny
    protected OfertySprzedazySpekulantow[] OfertySprzedazySpekulantow; //kolejno produkty(0-3) sortujemy Poziom,Cena,Ilosc
    protected OfertaRobotnika[] OfertyRobotnikow; //sortujesz względem typu giełdy, kolejka robotników co pracowali
                                                    // Kolejno Sprzedaż J,U,N,P.
                                                    //Potem Kupno J,U,N,P
                                                    //max długość to ilość wszystkich robotnikow
    protected int IleOfertRobotnikow;

    Gielda(int iloscTur, int iloscRobotnikow, int iloscSpekulantow, double[] cenyDzienZero, int karaZaBrakUbran,
           Robotnik[] robotnicy, Spekulant[] spekulanci, int maksOfert){
        this.IloscTur=iloscTur;
        this.ObecnyDzien=1;
        this.IloscRobotnikow=iloscRobotnikow;
        this.IloscSpekulantow=iloscSpekulantow;
        this.Ceny= new double[4][iloscTur+1];
        Ceny[0]=cenyDzienZero;
        this.Robotnicy=robotnicy;
        this.Spekulanci=spekulanci;
        this.KaraZaBrakUbran=karaZaBrakUbran;
        this.MaksOfert=new int[iloscTur+1];
        MaksOfert[0]=maksOfert;
        this.IleOfertRobotnikow=0;
    }

    public int podajObecnyDzien(){
        return ObecnyDzien;
    }
    public double sredniaCena(int dzien, int produkt){
        return Ceny[dzien][produkt];
    }
    public double sredniaCenaWczoraj(int produkt){
        return Ceny[ObecnyDzien-1][produkt];
    }

    public double sredniaCenaProduktuZNOstatnichDni(int dniWstecz, int produkt){
        double srednia=0;
        if(dniWstecz>this.ObecnyDzien) dniWstecz=this.ObecnyDzien;
        for(int i=this.ObecnyDzien-1; i>=this.ObecnyDzien-dniWstecz; i--){
            srednia+=Ceny[i][produkt];
        }
        return srednia/dniWstecz;
    }

    public int najwyzszaSredniaCenaZWczoraj(){
        double max=0;
        int numerProduktu=0;
        for(int i=0; i<4; i++){
            if(Ceny[ObecnyDzien-1][i]>max){
                max=Ceny[ObecnyDzien-1][i];
                numerProduktu=i;
            }
        }
        return numerProduktu;
    }

    public int podajKareZaBrakUbran(){
        return -KaraZaBrakUbran;
    }

    public void wstawOferteKupnaSpekulanta(int produkt, double cena, Spekulant Spekulant){
        //cena liczba jednostek, spekulant, ilość to 100 sztuk.
        OfertyKupnaSpekulantow[produkt].dodajOferte(new OfertaSpekulanta(100, cena, Spekulant));
    }

    public void wstawOferteSprzedazySpekulanta(int produkt, double cena, int ilosc, Spekulant Spekulant){
        //cena liczba jednostek spekulant
        OfertySprzedazySpekulantow[produkt].dodajOferte(new OfertaSpekulanta(ilosc, cena, Spekulant));
    }

    public void wstawOferteRobotnika(OfertaRobotnika ofertaRobotnika, int numer){
        OfertyRobotnikow[numer]=ofertaRobotnika;
    }

    public int ileOfertProduktu(int produkt){
        int suma=0;
        for(int i=0; i<IleOfertRobotnikow; i++){
            if(produkt==OfertyRobotnikow[i].SprzedawanyProdukt){
                suma+=OfertyRobotnikow[i].IloscSprzedawana;
            }
        }
        return suma; //sumaryczna ilosc ofert produktu dzisiaj dana przez robotnikow.
    }

    public int maxOfertProduktu(int produkt){
        return this.MaksOfert[produkt];
    }

    protected abstract void sortujOfertyRobotnikow();

    protected void zmienRozmiarOfertRobotnikow(){
        OfertaRobotnika[] SkroconyZestaw= new OfertaRobotnika[IleOfertRobotnikow];
        System.arraycopy(OfertyRobotnikow, 0, SkroconyZestaw, 0, IleOfertRobotnikow);
        this.OfertyRobotnikow=SkroconyZestaw;
    }

    protected void dzienRobotnikow(){
        for(int numer=0; numer<IloscRobotnikow; numer++){
            Robotnicy[numer].dzienRobotnika(this, numer);
        }
    }

    protected void dzienSpekulantow(){
        for(int numer=0; numer<IloscSpekulantow; numer++){
            Spekulanci[numer].ofertySpekulanta(this); //wykonuje dzien spekulanta
        }
    }

    protected void obsluzTransakcje(){
        //dokonuje transakcji gieldowych...
    }

    protected void wypiszStan(){
        //wypisz toString albo toJoson
    }

    protected void obsluzTure(){
        dzienRobotnikow();
        dzienSpekulantow();
        obsluzTransakcje();
        wypiszStan();
    }

    protected void obsluzTury(){
        for(int dzien=1; dzien<=IloscTur; dzien++){
            obsluzTure();
        }
    }
}
