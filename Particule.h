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
    Carre& get_forme() { return forme_; }
    bool too_small();
    bool hors_domaine();
    void draw() const;
    void set_target(bool b) { target_ = b; }
    bool is_target() const { return target_; }
private:
    Carre forme_;
    bool target_ = false;

};

#endif