package com.company;

public class Zasoby {
//    protected Jedzenie Jedzenie;
    protected int Jedzenie;
    protected Ubrania Ubrania;
    protected Narzedzia Narzedzia;
    protected ProgramyKomputerowe ProgramyKomputerowe;
    protected double Diamenty;

//    Produkty[] Produkty; // kolejno Jedzenie Ubrania Narzedzia Programy Diamenty

    public int ileZasobuI(int i){
        if(i==0) return ileJedzenia();
        if(i==1) return ileUbran();
        if(i==2) return ileNarzedzi();
        return ileProgramow(); //i==3
    }
    public double ileDiamentow(){return Diamenty;/*Diamenty.podajIlosc();*/}

    public void jedz(){
        Jedzenie=Math.max(0, Jedzenie-100);
    }

    public void uzyjUbran(){
        Ubrania.uzyjUbran();
    }

    public void uzyjNarzedzia(){
        Narzedzia=new Narzedzia(0);
    }

    public int ileJedzenia(){return Jedzenie;/*Jedzenie.podajIlosc();*/}
    public int ileUbran(){return Ubrania.podajIlosc();}
    public int ileNarzedzi(){return Narzedzia.podajIlosc();}
    public int ileProgramow(){return ProgramyKomputerowe.podajIlosc();}

    public void ustawDiamenty(double wartosc){ this.Diamenty=wartosc;}

}
