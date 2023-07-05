package com.company;

public abstract class StrategiaProdukcji {

    public abstract int coProdukuje(Robotnik Rob, Gielda Gie); //zwraca numer produkowanego przedmiotu.
    //Kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe, Diamenty
}
