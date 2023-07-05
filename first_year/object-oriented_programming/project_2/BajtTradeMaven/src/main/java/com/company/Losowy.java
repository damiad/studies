package com.company;

import java.util.Random;

public class Losowy extends StrategiaProdukcji{

    Losowy(){}
    @Override
    public int coProdukuje(Robotnik Rob, Gielda Gie) {
        return new Random().nextInt(4);
    }
}
