package com.company;

public abstract class Robotnik extends Agent{
    protected final StrategiaZmianyKariery StrategiaZmianyKariery; //Konserwatysta, Rewolucjonista
    protected final StrategiaProdukcji StrategiaProdukcji; //Krotkowzroczny, Chciwy, Sredniak, Perspektywiczny, Losowy
    protected final StrategiaKupowania StrategiaKupowania;
    protected SciezkaKariery SciezkaKariery;
    //ZbiórProduktów albo wyposażenie
    protected final int[] WektorProduktywnosci; //Kolejne Jednostki ile czego może wyprodukować.
    protected int[] BonusProdukcji; //Kolejno dla: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe
    private int DniBezJedzenia;
    private boolean CzyTrup; //true to umarł, false to żyje

    Robotnik(int id, Zasoby zasoby, StrategiaZmianyKariery strategiaZK, StrategiaProdukcji strategiaP,
             StrategiaKupowania strategiaK, SciezkaKariery sciezkaKariery, int[] wektorProduktywnosci){
        super(id,zasoby);
        this.StrategiaZmianyKariery=strategiaZK;
        this.StrategiaProdukcji=strategiaP;
        this.StrategiaKupowania=strategiaK;
        this.SciezkaKariery=sciezkaKariery;
        this.WektorProduktywnosci=wektorProduktywnosci;
        this.DniBezJedzenia=0;
        this.CzyTrup=false;
    }


    public int obecnaProdukcjaSurowca(int produkt){
        return Math.max(0, WektorProduktywnosci[produkt] * (100+BonusProdukcji[produkt]) );
    }

    //    Pracus, Oszczedny, Student, Okresowy, Rozkladowy
    public boolean czyPracuje(Gielda Gie){
        return true; //false gdy nie pracuje
    }

    //    Konserwatysta, Rewolucjonista
    public boolean czyZmieniaKariere(Gielda Gie) {
        return StrategiaZmianyKariery.czyZmieniaKariere(this, Gie);
    }

    //    Krotkowzroczny, Chciwy, Sredniak, Perspektywiczny, Losowy
    public int coProdukuje(Gielda Gie){ //może wywalić
        return StrategiaProdukcji.coProdukuje(this, Gie);
    }

    //sprzedaje to co wyprodukował.

    //    Technofob, Czyscioszek, Zmechanizowany, Gadzeciarz
    public int[] coKupujeRobotnik(Gielda Gie, int IloscProdukcjiDzis) {
        return StrategiaKupowania.coKupuje(this, Gie, IloscProdukcjiDzis);
    } //do dopisania
    //Kolejno: Jedzenie, Ubrania, Narzedzia, ProgramyKomputerowe

    protected int karaZaBrakUbran(Gielda Gie){
        if(this.ileZasobuI(1)<100) return Gie.podajKareZaBrakUbran();
        return 0;
    }

    protected int karaZaJedzenie(){
        if(this.DniBezJedzenia==2) return -300;
        if(this.DniBezJedzenia==1) return -100;
        return 0;
    }

    protected int bonusZaNarzedzia(){
        return this.ileZasobuI(2);
    }

    protected int bonusZaKariere(int produkt){
        return this.SciezkaKariery.podajPremie(produkt);
    }

    protected int liczSumeBonusow(Gielda Gie, int produkt){
        //zlicza wszystkie bonusy....
        return 100+ karaZaJedzenie()+karaZaBrakUbran(Gie)+bonusZaNarzedzia()+bonusZaKariere(produkt);
    }

    protected void uczSie(){
        this.SciezkaKariery.uczSie(); //awansuje w aktualnej karierze
    }
    public void zmienKariere(int kariera){
        this.SciezkaKariery.zmienKariere(kariera);
    }

    public int podajAktualnaKariere(){
        return this.SciezkaKariery.podajAktualnaKariere();
    }

    protected void uzyjJedzenie(){
        if(this.ileZasobuI(0)<100){
            this.DniBezJedzenia++;
            if(DniBezJedzenia>2){
                this.CzyTrup=true;
                this.ustawDiamenty(0);
            }
        }
        this.Zasoby.jedz();
    }

    protected void uzyjUbran(){
        this.Zasoby.uzyjUbran();
    }

    protected void uzyjNarzedzia(){
        this.Zasoby.uzyjNarzedzia();
    }

    protected void pracuj(Gielda Gie, int numer){

        int produkt=this.coProdukuje(Gie);
        int iloscProduktu=(WektorProduktywnosci[produkt]/100)*Math.max(0, liczSumeBonusow(Gie, produkt));
           //teraz programy komputerowe zmieniają poziom i są zuzywane...
        uzyjJedzenie();
        uzyjUbran();
        uzyjNarzedzia();
        if(produkt==4){
            this.ustawDiamenty(ileDiamentow()+iloscProduktu);  //dodaj diamenty
        }
        Gie.wstawOferteRobotnika(new OfertaRobotnika(coKupujeRobotnik(Gie, iloscProduktu),
                                produkt, iloscProduktu, this), numer);
                                                            //dodaj JakoscProduktu
    }

    public void dzienRobotnika(Gielda Gie, int numer) {
        if (!CzyTrup) {
            if (czyPracuje(Gie) == false) {
                this.DniBezJedzenia = 0;
                uzyjUbran();
                if (czyZmieniaKariere(Gie) == false) this.uczSie();  //uczy sie
                //w przeciwnym wypadku kariera zostala zmieniona
            } else {
                this.pracuj(Gie, numer);   //pracuje
            }
        }
    }
}
