/**
 Shape.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <iostream>
#include <cmath>
#include "../Graphic/Graphic.h"

#ifndef SHAPE_H
#define SHAPE_H

constexpr double epsil_zero(0.125);

enum Etat { NBP, PARTICULE, SPATIAL, REPARATEUR, NEUTRALISEUR };

struct S2d {
    double x = 0.;
    double y = 0.;
};

struct Carre {
    S2d centre;
    double cote = 0.;
};

struct Cercle {
    Cercle() = default;
    Cercle(S2d p, double r);
    S2d centre;
    double rayon = 0.;
};

bool superposition(Carre const& carre_1, Carre const& carre_2, bool epsil = false);
bool superposition(Cercle const& cercle_1, Cercle const& cercle_2, bool epsil = false);
bool superposition(Carre const& carre, Cercle const& cercle, bool epsil = false);

#endif