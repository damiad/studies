package com.company;

public class Krotkowzroczny extends StrategiaProdukcji{

    Krotkowzroczny(){}
    @Override
    public int coProdukuje(Robotnik Rob, Gielda Gie) {
        return Gie.najwyzszaSredniaCenaZWczoraj();
    }
}
