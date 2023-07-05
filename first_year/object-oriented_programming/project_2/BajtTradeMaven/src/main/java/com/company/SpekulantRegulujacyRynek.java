package com.company;

public class SpekulantRegulujacyRynek extends Spekulant{

    SpekulantRegulujacyRynek(int id, Zasoby zasoby){
        super(id, zasoby);
    }

    @Override
    public void ofertySpekulanta(Gielda Gie) {
        if(Gie.podajObecnyDzien()!=1){
            for(int produkt=0; produkt<4; produkt++){
                double wyn=Gie.sredniaCenaWczoraj(produkt)*Gie.ileOfertProduktu(produkt)/Gie.maxOfertProduktu(produkt);
                Gie.wstawOferteKupnaSpekulanta(produkt, wyn*0.9, this);
                if(this.ileZasobuI(produkt)!=0){
                    Gie.wstawOferteSprzedazySpekulanta(produkt, wyn*1.1, this.ileZasobuI(produkt), this);
                }
            }
        }
    }
}
