/**
 Particule.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <iostream>
#include "../Shape/Shape.h"
#include "../Message/Message.h"

#ifndef PARTICULE_H
#define PARTICULE_H

class Particule {
public:
    Particule() = delete;
    explicit Particule(Carre c);
    Carre get_forme() const { return forme_; }
private:
    Carre forme_;
};

void test_domaine(Carre c);
#endif