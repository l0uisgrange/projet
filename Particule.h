/**
 Particule.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <iostream>
#include "Shape.h"
#include "Message.h"
#include "Constantes.h"

#ifndef PARTICULE_H
#define PARTICULE_H

class Particule {
public:
    Particule() = delete;
    explicit Particule(Carre c);
    Carre get_forme() const { return forme_; }
    bool too_small();
    bool hors_domaine();
    void draw() const;
private:
    Carre forme_;
};

#endif