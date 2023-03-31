/**
 Robot.h
 Louis Grange et Daniel Ataide
 Version 1.2
**/

#include <vector>
#include "../Message/Message.h"
#include "../Shape/Shape.h"
#include "../Particule/Particule.h"
using namespace std;

#ifndef ROBOT_H
#define ROBOT_H

class Neutraliseur {
public:
    // CONSTRUCTEURS
    Neutraliseur() = delete;
    Neutraliseur(S2d position, double angle, int coordination, bool panne, int k_update_panne, int nbUpdate);
    // GETTERS
    Cercle get_forme() const { return forme_; }
private:
    Cercle forme_;
    double angle_;
    bool panne_;
    int coordination_;
    double vrot_ = 0;
    double vtran_ = 0;
};

class Reparateur {
public:
    // CONSTRUCTEURS
    Reparateur() = delete;
    explicit Reparateur(S2d position);
    // GETTERS
    Cercle get_forme() const { return forme_; }
private:
    Cercle forme_;
};

class Spatial {
public:
    // CONSTRUCTEURS
    Spatial() = delete;
    Spatial(S2d position, int nbUpdate, int nbNr, int nbNs, int nbNd, int nbRr, int nbRs);
    // GETTERS
    Cercle get_forme() const { return forme_; }
    int get_update() const { return nbUpdate_; }
    int get_nbNr() const { return nbNr_; }
    int get_nbNs() const { return nbNs_; }
    int get_nbNd() const { return nbNd_; }
    int get_nbRr() const { return nbRr_; }
    int get_nbRs() const { return nbRs_; }
    int get_nbR_tot() const;
    int get_nbN_tot() const;
private:
    Cercle forme_;
    int nbUpdate_;
    int nbNr_;
    int nbNs_;
    int nbNd_;
    int nbRr_;
    int nbRs_;
};

#endif
