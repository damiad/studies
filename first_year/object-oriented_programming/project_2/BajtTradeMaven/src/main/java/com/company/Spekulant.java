package com.company;

public abstract class Spekulant extends Agent{
    //nic nie produkują
    public abstract void ofertySpekulanta(Gielda Gie);
        //do giełdy dodajemy odpowiednie zlecenia

    Spekulant(int id, Zasoby zasoby){
        super(id, zasoby);
    }
}
