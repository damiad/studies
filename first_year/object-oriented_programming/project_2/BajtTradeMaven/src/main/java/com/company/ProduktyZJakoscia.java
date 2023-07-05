package com.company;

import java.awt.*;
import java.util.Vector;

public abstract class ProduktyZJakoscia extends Produkty{
    //ma poziom jakosci/zaawansowania (jaki zakres poziomow?)
    protected Vector<java.awt.Point> PoziomIlosc; //Para poziom, ilosc
    //aktualizujemy Ilośc, która jest sumą ilosci
    ProduktyZJakoscia(int ile){
        super(ile);
        PoziomIlosc= new Vector<>();
        PoziomIlosc.add(new Point(1, ile));
    }

    public void dodajProdukty(int poziom, int ilosc){
        int i=0;
        while(i<PoziomIlosc.size()-1 && PoziomIlosc.elementAt(i).x < poziom){i++;}
        if(PoziomIlosc.elementAt(i).x < poziom) PoziomIlosc.add(i+1, new Point(poziom, ilosc));
        else if(PoziomIlosc.elementAt(i).x == poziom) PoziomIlosc.elementAt(i).y+=ilosc;
        else if(PoziomIlosc.elementAt(i).x > poziom) PoziomIlosc.add(i, new Point(poziom, ilosc));
    }
}
