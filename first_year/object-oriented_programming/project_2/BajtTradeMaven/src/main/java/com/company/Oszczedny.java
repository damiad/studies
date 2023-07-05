package com.company;

public class Oszczedny extends Robotnik{

    private final double LimitDiamentow;

    Oszczedny(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
            StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci,
            int limitDiamentow) {
        super(id, zasoby, strategiaZK, strategiaP, strategiaK, sciezkaKariery, wektorProduktywnosci);
        this.LimitDiamentow=limitDiamentow;
    }


    @Override
    public boolean czyPracuje(Gielda Gie) {
        return !(this.ileDiamentow() > this.LimitDiamentow);//uczy się gdy ma powyżej LimitDiamentów
    }
}
