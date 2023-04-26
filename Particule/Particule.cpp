/**
 Particule.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Particule.h"
#include <sstream>
using namespace std;

Particule::Particule(Carre c) {
    if (c.cote >= d_particule_min) {
        forme_ = c;
    } else {
        cout << message::particle_too_small(c.centre.x, c.centre.y, c.cote);
        exit(EXIT_FAILURE);
    }
    test_domaine(c);
}

void test_domaine(Carre c) {
    if(abs(c.centre.x) >= (dmax - c.cote/2)) {
        cout << message::particle_outside(c.centre.x, c.centre.y, c.cote);
        exit(EXIT_FAILURE);
    }
    if(abs(c.centre.y) >= (dmax - c.cote/2)) {
        cout << message::particle_outside(c.centre.x, c.centre.y, c.cote);
        exit(EXIT_FAILURE);
    }
}

void Particule::draw() const {
    draw_carre(forme_.centre.x, forme_.centre.y, forme_.cote, ROUGE);
}