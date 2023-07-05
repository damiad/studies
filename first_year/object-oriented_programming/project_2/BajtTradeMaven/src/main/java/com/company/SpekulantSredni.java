package com.company;

public class SpekulantSredni extends Spekulant{
    private final int HistoriaSpekulantaSredniego;

    SpekulantSredni(int id, Zasoby zasoby, int historiaSpekulantaSredniego){
        super(id, zasoby);
        this.HistoriaSpekulantaSredniego=historiaSpekulantaSredniego;
    }

    @Override
    public void ofertySpekulanta(Gielda Gie) {
        for(int produkt=0; produkt<4; produkt++){
            double srednia = Gie.sredniaCenaProduktuZNOstatnichDni(this.HistoriaSpekulantaSredniego,produkt);
            if(this.ileZasobuI(produkt)==0) {
                Gie.wstawOferteKupnaSpekulanta(produkt, srednia*0.95, this);
            }else{
                Gie.wstawOferteKupnaSpekulanta(produkt, srednia*0.9, this);
                Gie.wstawOferteSprzedazySpekulanta(produkt, srednia*1.1, this.ileZasobuI(produkt), this);
            }
        }
    }
}
