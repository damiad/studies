package com.company;

public class SpekulantWypukly extends Spekulant{

    SpekulantWypukly(int id, Zasoby zasoby){
        super(id, zasoby);
    }

    //-1 ściśle wklęsła, 1 ściśle wypukła, 0 żadna z nich
    private int jakaFunkcja(Gielda Gie, int produkt){
        if(Gie.podajObecnyDzien()<3) return 0;
        double mid=(Gie.sredniaCenaWczoraj(produkt) +
                Gie.sredniaCena(Gie.podajObecnyDzien()-3, produkt));
        mid/=2;
        return Double.compare(mid, Gie.sredniaCena(Gie.podajObecnyDzien() - 2, produkt));
    }

    @Override
    public void ofertySpekulanta(Gielda Gie) {
        for(int produkt=0; produkt<4; produkt++){
            int sign=jakaFunkcja(Gie, produkt);
            if(sign == -1){
                Gie.wstawOferteKupnaSpekulanta(produkt, Gie.sredniaCenaWczoraj(produkt)*0.9, this);
            }else if(sign == 1){
                if(this.ileZasobuI(produkt)!=0){
                    Gie.wstawOferteSprzedazySpekulanta(produkt, Gie.sredniaCenaWczoraj(produkt)*1.1,
                            this.ileZasobuI(produkt), this);
                }
            }
        }
    }
}
