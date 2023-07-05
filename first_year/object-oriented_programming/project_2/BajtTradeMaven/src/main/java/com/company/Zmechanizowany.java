package com.company;

public class Zmechanizowany extends StrategiaKupowania{

    private final int LiczbaNarzedzi;

    Zmechanizowany(int liczbaNarzedzi){
        this.LiczbaNarzedzi=liczbaNarzedzi;
    }

    @Override
    public int[] coKupuje(Robotnik Rob, Gielda Gie, int IloscProdukcjiDzis) {
        int[] tab= new int[4];
        tab[0]=100;
        tab[1]=Math.max(0,100-Rob.Zasoby.ileUbran());
        tab[2]=LiczbaNarzedzi;
        tab[3]=0;
        return tab;
    }
}
