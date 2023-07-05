package com.company;

public class Technofob extends StrategiaKupowania{

    Technofob(){}

    @Override
    public int[] coKupuje(Robotnik Rob, Gielda Gie, int IloscProdukcjiDzis) {
        int[] tab= new int[4];
        tab[0]=100;
        tab[1]=0;
        tab[2]=0;
        tab[3]=0;
        return tab;
    }
}
