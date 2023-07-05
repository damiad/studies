package com.company;


import java.util.Arrays;

public class OfertyKupnaSpekulantow extends OfertySpekulantow{

    OfertyKupnaSpekulantow(int maksymalnieOfert){
        super(maksymalnieOfert);
    }

    @Override
    public void sortuj() {
        zmienRozmiar();
        Arrays.sort(OfertyZestaw,
                (a, b) -> {
                    if (a.Cena == b.Cena) {
                        return a.Spekulant.podajID() - b.Spekulant.podajID();
                    }
                    if(a.Cena < b.Cena) return -1;
                    return 1; //im wyższa cena tym wyżej w tabeli
                }
        );
    }
}
