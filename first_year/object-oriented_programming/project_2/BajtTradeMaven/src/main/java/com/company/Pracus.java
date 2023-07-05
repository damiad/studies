package com.company;

public class Pracus extends Robotnik{

    Pracus(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
            StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci) {
        super(id, zasoby, strategiaZK, strategiaP, strategiaK, sciezkaKariery, wektorProduktywnosci);
    }

    @Override
    public boolean czyPracuje(Gielda Gie){
        return true; //zawsze pracuje
    }
}
