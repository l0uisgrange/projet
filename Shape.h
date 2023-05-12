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
    S2d operator-(S2d p) const;
    double norme() const { return sqrt(pow(x,2)+pow(y,2)); }
    double diff_angle(S2d angle) const;
    double x = 0.;
    double y = 0.;
};

struct Cercle {
    bool operator!=(Cercle c) const;
    Cercle() = default;
    Cercle(S2d p, double r);
    S2d centre;
    double rayon = 0.;
};

struct Carre {
    S2d centre;
    double cote = 0.;
};

bool superposition(Carre const& carre_1, Carre const& carre_2, bool epsil = false);
bool superposition(Cercle const& cercle_1, Cercle const& cercle_2,
                   bool epsil = false);
bool superposition(Carre const& carre, Cercle const& cercle, bool epsil = false);

#endif