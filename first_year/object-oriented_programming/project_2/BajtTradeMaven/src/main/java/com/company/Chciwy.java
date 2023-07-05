package com.company;

public class Chciwy extends StrategiaProdukcji{

    Chciwy(){}
    @Override
    public int coProdukuje(Robotnik Rob, Gielda Gie) {
        //max z
        //liczba produktów danego typu * cena z wczoraj
        double max=0;
        int numerProduktu=0;
        for(int i=0; i<4; i++){
            double wyn=Rob.obecnaProdukcjaSurowca(i)*Gie.sredniaCenaWczoraj(i); //do zmiany
            if(wyn>max){
                max=wyn;
                numerProduktu=i;
            }
        }
        return numerProduktu;
    }
}
