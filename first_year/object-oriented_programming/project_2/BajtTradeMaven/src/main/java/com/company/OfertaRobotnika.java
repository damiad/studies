package com.company;
//wywal
public class OfertaRobotnika {
    public int[] OfertyKupna; //Ilość Kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
    public int SprzedawanyProdukt;
    public int IloscSprzedawana;
    public Robotnik Robotnik;

    OfertaRobotnika(int[]ofertyKupna, int sprzedawanyProdukt, int iloscSprzedawana, Robotnik Rob){
        this.OfertyKupna=ofertyKupna;
        this.SprzedawanyProdukt=sprzedawanyProdukt; //jak diament to nie zostanie wystawiona przez Gielde
        this.IloscSprzedawana=iloscSprzedawana;
        this.Robotnik=Rob;
    }
}
