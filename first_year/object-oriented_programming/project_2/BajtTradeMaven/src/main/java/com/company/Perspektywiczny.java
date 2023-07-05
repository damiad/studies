package com.company;

public class Perspektywiczny extends StrategiaProdukcji{

    private final int HistoriaPerspektyw;

    Perspektywiczny(int historiaPerspektyw){
        this.HistoriaPerspektyw=historiaPerspektyw;
    }

    @Override
    public int coProdukuje(Robotnik Rob, Gielda Gie) {
        double max=0;
        int numerProduktu=0;
        for(int i=0; i<4; i++){
            double wyn=Gie.sredniaCenaWczoraj(i) - Gie.sredniaCena(Math.max(0,Gie.podajObecnyDzien()
                    - this.HistoriaPerspektyw),i);
            if(wyn>max){
                max=wyn;
                numerProduktu=i;
            }
        }
        return numerProduktu;
    }
}
