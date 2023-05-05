/**
 Shape.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <iostream>
#include <cmath>
#include "Graphic.h"

#ifndef SHAPE_H
#define SHAPE_H

constexpr double epsil_zero(0.125);

enum Etat { NBP, PARTICULE, SPATIAL, REPARATEUR, NEUTRALISEUR };

struct S2d {
    S2d operator-(S2d p) const {
        S2d vecteur;
        vecteur.x = x-p.x;
        vecteur.y = y-p.y;
        return vecteur;
    }
    double x = 0.;
    double y = 0.;
};

struct Cercle {
    Cercle() = default;
    Cercle(S2d p, double r);
    S2d centre;
    double rayon = 0.;
};

struct Carre {
    double operator-(Cercle c) const {
        S2d vecteur;
        return 2.0;
    }
    S2d centre;
    double cote = 0.;
};

bool superposition(Carre const& carre_1, Carre const& carre_2, bool epsil = false);
bool superposition(Cercle const& cercle_1, Cercle const& cercle_2,
                   bool epsil = false);
bool superposition(Carre const& carre, Cercle const& cercle, bool epsil = false);

#endif