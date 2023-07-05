package com.company;

import java.util.Arrays;

public class GieldaKapitalistyczna extends Gielda{

    GieldaKapitalistyczna(int iloscTur, int iloscRobotnikow, int iloscSpekulantow, double[] cenyDzienZero,
                          int karaZaBrakUbran, Robotnik[] robotnicy, Spekulant[] spekulanci, int maksOfert){
        super(iloscTur, iloscRobotnikow, iloscSpekulantow, cenyDzienZero, karaZaBrakUbran, robotnicy, spekulanci, maksOfert);
    }
    @Override
    protected void sortujOfertyRobotnikow() {
        zmienRozmiarOfertRobotnikow();
        Arrays.sort(OfertyRobotnikow,
                (a, b) -> {
                    if (a.Robotnik.ileZasobuI(4) == b.Robotnik.ileZasobuI(4)) {
                        return a.Robotnik.podajID() - b.Robotnik.podajID();
                    }
                    if(a.Robotnik.ileZasobuI(4) < b.Robotnik.ileZasobuI(4)) return 1;
                    return -1; //Im więcej diamentów tym niższy numer (priorytet)
                }
        );
    }
}
