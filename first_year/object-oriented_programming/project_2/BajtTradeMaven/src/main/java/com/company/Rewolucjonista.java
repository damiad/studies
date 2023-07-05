package com.company;

public class Rewolucjonista extends StrategiaZmianyKariery{

    Rewolucjonista(){}

    public int liczNajczestszyProdukt(int n, Gielda Gie){
        int max=0;
        int numerProduktu=0; //Kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
        for(int j=0; j<4; j++){
            int sumarycznie=0;
            for(int i=Gie.podajObecnyDzien()-1; i>=Gie.podajObecnyDzien()-n && i>=0; i++){
                sumarycznie+=Gie.Dzien[i].podajIloscOfertLacznie(j);
            }
            if(sumarycznie>max){
                max=sumarycznie;
                numerProduktu=j;
            }
        }
        return numerProduktu;
    }

    @Override
    public boolean czyZmieniaKariere(Robotnik Rob, Gielda Gie) {
        if(Gie.podajObecnyDzien()%7!=0) return false; //nie zmienia kariery
        int n = Math.max(1,Rob.podajID()%17);
        int PremiowanyProdukt=liczNajczestszyProdukt(n, Gie);
        if(PremiowanyProdukt==Rob.podajAktualnaKariere()) return false;
        Rob.zmienKariere(PremiowanyProdukt);
        return true; //zmienia na sciezkę odpowiadjącą premiowanyProdukt
    }
}
