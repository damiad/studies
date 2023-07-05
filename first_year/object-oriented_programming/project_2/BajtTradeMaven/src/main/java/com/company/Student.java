package com.company;

public class Student extends Robotnik{
    private final int ZapasJednostekJedzenia;
    private final int Okres;

    Student(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
             StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci,
            int zapasJednostekJedzenia, int okres) {
        super(id, zasoby, strategiaZK, strategiaP, strategiaK, sciezkaKariery, wektorProduktywnosci);
        this.ZapasJednostekJedzenia=zapasJednostekJedzenia;
        this.Okres=okres;
    }

    public double liczSredniaCeneJedzenia(Gielda Gie){
        double wyn=0;
        for(int i=1; i<=this.Okres; i++){
            wyn+=Gie.sredniaCena(Gie.podajObecnyDzien()-i, 0);
        }
        return wyn/this.Okres;
    }

    @Override
    public boolean czyPracuje(Gielda Gie) {
        return 100 * this.ZapasJednostekJedzenia * this.liczSredniaCeneJedzenia(Gie) > this.ileDiamentow();
        //Pracuje gdy go nie stać na kupno jedzenia.
    }
}
