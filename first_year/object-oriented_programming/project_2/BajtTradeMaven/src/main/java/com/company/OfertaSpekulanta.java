package com.company;

public class OfertaSpekulanta {
    //produkt już znany
    public int Ilosc; //publiczne bo giełda ma dostęp i modyfikuje
    public double Cena;
    public final Spekulant Spekulant;

    OfertaSpekulanta(int ilosc, double cena, Spekulant spekulant){
        this.Ilosc=ilosc;
        this.Cena=cena;
        this.Spekulant=spekulant;
    }


}
