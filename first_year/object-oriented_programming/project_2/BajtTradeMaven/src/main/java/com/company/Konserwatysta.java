package com.company;

public class Konserwatysta extends StrategiaZmianyKariery{

    Konserwatysta(){}
    @Override
    public boolean czyZmieniaKariere(Robotnik Rob, Gielda Gie) {
        return false;
    }
}
