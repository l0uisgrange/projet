/**
 Shape.cpp
 Louis Grange (60%) et Daniel Ataide (40%)
 Version 1.1
**/

#include "Shape.h"
#include <cmath>

S2d S2d::operator-(S2d p) const {
    S2d vecteur;
    vecteur.x = x-p.x;
    vecteur.y = y-p.y;
    return vecteur;
}

bool S2d::operator==(S2d p) const {
    if(x == p.x and y == p.y) {
        return true;
    } else {
        return false;
    }
}

bool Cercle::operator!=(Cercle c) const {
    if(c.rayon != rayon) {
        return true;
    } else if(centre.x == c.centre.x and centre.y == c.centre.y) {
        return false;
    }
    return true;
}

Cercle::Cercle(S2d p, double r) {
    centre = p;
    rayon = r;
}

bool superposition(Carre const& c1, Carre const& c2, bool epsil) {
    double somme;
    if(epsil) {
        somme = c1.cote/2 + c2.cote/2 + epsil_zero;
    } else {
        somme = c1.cote/2 + c2.cote/2;
    }
    if(abs(c2.centre.x - c1.centre.x) < somme
       and abs(c2.centre.y - c1.centre.y) < somme) {
        return true;
    }
    return false;
}

bool superposition(Cercle const& c1, Cercle const& c2, bool epsil) {
    double x(c1.centre.x-c2.centre.x);
    double y(c1.centre.y-c2.centre.y);
    double distance(sqrt(pow(x, 2)+pow(y, 2)));
    if(epsil) {
        if(distance < (c1.rayon + c2.rayon) + epsil_zero) {
            return true;
        }
    } else {
        if(distance < (c1.rayon + c2.rayon)) {
            return true;
        }
    }
    return false;
}

bool superposition(Carre const& c1, Cercle const& c2, bool epsil) {
    S2d C1C2;
    C1C2.x = c2.centre.x - c1.centre.x;
    C1C2.y = c2.centre.y - c1.centre.y;
    double L(sqrt(pow(abs(C1C2.x) - c1.cote/2,2)
            + pow(abs(C1C2.y) - c1.cote/2,2)));

    if(epsil) {
        if ((abs(C1C2.x) > (c1.cote/2))
            and (abs(C1C2.y) > (c1.cote/2))
            and (L > (c2.rayon + epsil_zero))) {
            return false;
        } else {
            if ((abs(C1C2.x) < (c1.cote/2 + c2.rayon + epsil_zero))
                and abs(C1C2.y) < (c1.cote/2 + c2.rayon + epsil_zero)) {
                return true;
            }
        }
    } else {
        if((abs(C1C2.x) > (c1.cote/2))
            and (abs(C1C2.y) > (c1.cote/2))
            and (L > c2.rayon)) {
            return false;
        } else {
            if((abs(C1C2.x) < (c1.cote/2 + c2.rayon))
                and abs(C1C2.y) < c1.cote/2 + c2.rayon) {
                return true;
            }
        }
    }
    return false;
}