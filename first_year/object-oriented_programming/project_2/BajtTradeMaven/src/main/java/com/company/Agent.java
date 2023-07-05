package com.company;

public abstract class Agent {
    protected int id;
    public Zasoby Zasoby;

    Agent(int id, Zasoby zasoby){
        this.id=id;
        this.Zasoby=zasoby;
    }



    public int podajID(){
        return this.id;
    }

    public int ileZasobuI(int numerProduktu){return this.Zasoby.ileZasobuI(numerProduktu);}
    public double ileDiamentow(){return this.Zasoby.ileDiamentow();}
    public void ustawDiamenty(double wartosc){ this.Zasoby.ustawDiamenty(wartosc);}

}
