package com.company;

public abstract class Produkty {
    protected int Ilosc;

    Produkty(int ile){
        this.Ilosc=ile;
    }

    public int podajIlosc(){
        return Ilosc;
    }
    
    public void zmienOiles(int ile){
        this.Ilosc+=ile;
    }
}
