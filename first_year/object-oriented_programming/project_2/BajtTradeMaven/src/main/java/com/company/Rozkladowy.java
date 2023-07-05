package com.company;

import java.util.Random;

public class Rozkladowy extends Robotnik{

    Rozkladowy(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
            StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci) {
        super(id, zasoby, strategiaZK, strategiaP, strategiaK, sciezkaKariery, wektorProduktywnosci);
    }
    @Override
    public boolean czyPracuje(Gielda Gie) {
        double compare=1.0/(Gie.podajObecnyDzien()+3); //prawdopodobienstwo nauki
        double x= new Random().nextDouble();
        return !(x < compare);
    }
}
