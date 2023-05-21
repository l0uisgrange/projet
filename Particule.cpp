/**
 Particule.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Particule.h"
#include <sstream>
using namespace std;

Particule::Particule(Carre c) : forme_(c) {}

bool Particule::too_small() {
    if(forme_.cote < d_particule_min) { // TODO adapter les conditions
        cout << message::particle_too_small(forme_.centre.x, forme_.centre.y,
                                            forme_.cote);
        return true;
    }
    return false;
}

bool Particule::hors_domaine() {
    if(abs(forme_.centre.x) >= (dmax - forme_.cote/2)) {
        cout << message::particle_outside(forme_.centre.x, forme_.centre.y,
                                          forme_.cote);
        return true;
    }
    if(abs(forme_.centre.y) >= (dmax - forme_.cote/2)) {
        cout << message::particle_outside(forme_.centre.x, forme_.centre.y,
                                          forme_.cote);
        return true;
    }
    return false;
}

void Particule::draw() const {
    fill_carre(forme_.centre.x-forme_.cote/2, forme_.centre.y-forme_.cote/2,
               forme_.cote, GRIS);
    draw_carre(forme_.centre.x-forme_.cote/2, forme_.centre.y-forme_.cote/2,
               forme_.cote, ROUGE);
}