package com.company;

public abstract class OfertySpekulantow { //może wywal dziedziczene
    protected int IloscOfert;
    protected OfertaSpekulanta[] OfertyZestaw;

    OfertySpekulantow(int maksymalnieOfert){
        this.IloscOfert=0;
        this.OfertyZestaw= new OfertaSpekulanta[maksymalnieOfert];
    }
    public void dodajOferte(OfertaSpekulanta ofertaSpekulanta){
        OfertyZestaw[IloscOfert]=ofertaSpekulanta;
        this.IloscOfert++;
    }

    protected void zmienRozmiar(){
        OfertaSpekulanta[] SkroconyZestaw= new OfertaSpekulanta[IloscOfert];
        System.arraycopy(OfertyZestaw, 0, SkroconyZestaw, 0, IloscOfert);
        this.OfertyZestaw=SkroconyZestaw;
    }

    public abstract void sortuj(); //rosnąco bądź malejąco

    public boolean czySaOferty(){
        return IloscOfert>0;
    }

    public OfertaSpekulanta podajOferte(){
        return OfertyZestaw[IloscOfert-1];
    }

    public void zmienOferte(int iloscPozostala){
        if(iloscPozostala==0) IloscOfert--;
        else OfertyZestaw[IloscOfert-1].Ilosc=iloscPozostala;
    }
}
