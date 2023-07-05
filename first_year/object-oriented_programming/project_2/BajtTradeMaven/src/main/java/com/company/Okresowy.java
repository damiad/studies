package com.company;

public class Okresowy extends Robotnik{

    private final int OkresowoscNauki;

    Okresowy(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
            StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci,
            int okresowoscNauki) {
        super(id, zasoby, strategiaZK, strategiaP, strategiaK, sciezkaKariery, wektorProduktywnosci);
        this.OkresowoscNauki=okresowoscNauki;
    }

    @Override
    public boolean czyPracuje(Gielda Gie) {
        return Gie.podajObecnyDzien() % OkresowoscNauki != 0;
        //Uczy się tylko w dni będące wielokrotnościami OkresowoscNauki
    }
}
