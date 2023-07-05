package com.company;

public class Gadzeciarz extends StrategiaKupowania{

    public int LiczbaNarzedzi;

    Gadzeciarz(int liczbaNarzedzi){
        this.LiczbaNarzedzi=liczbaNarzedzi;
    }

    @Override
    public int[] coKupuje(Robotnik Rob, Gielda Gie, int IloscProdukcjiDzis) {
        int[] tab= new int[4];
        tab[0]=100;
        tab[1]=Math.max(0,100-Rob.Zasoby.ileUbran());
        tab[2]=LiczbaNarzedzi;
        tab[3]=Math.max(0, IloscProdukcjiDzis - Rob.Zasoby.ileProgramow());
        return tab;
    }

}
