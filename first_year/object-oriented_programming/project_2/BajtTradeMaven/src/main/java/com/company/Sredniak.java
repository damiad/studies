package com.company;

public class Sredniak extends StrategiaProdukcji{

    private final int HistoriaSredniejProdukcji;

    Sredniak(int historiaSredniejProdukcji){
        this.HistoriaSredniejProdukcji=historiaSredniejProdukcji;
    }


    @Override
    public int coProdukuje(Robotnik Rob, Gielda Gie) {
        double max=0;
        int numerProduktu=0; //Kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
        for(int j=0; j<4; j++){
            for(int i=Gie.podajObecnyDzien()-1; i>=Gie.podajObecnyDzien()-this.HistoriaSredniejProdukcji && i>=0; i++){
                if(Gie.sredniaCena(i,j)>max) {
                    max = Gie.sredniaCena(i, j);
                    numerProduktu = j;
                }
            }
        }
        return numerProduktu;
    }

}
