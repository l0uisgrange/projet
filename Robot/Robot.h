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

class Robot {
public:
    virtual void draw() const = 0;
    //DESTRUCTEUR
    virtual ~Robot() {};
};

class Spatial : public Robot {
public:
    // CONSTRUCTEUR
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
    //METHODES
    void draw() const override {};
    //DESTRUCTEUR
    virtual ~Spatial() {};
private:
    Cercle forme_;
    int nbUpdate_;
    int nbNr_;
    int nbNs_;
    int nbNd_;
    int nbRr_;
    int nbRs_;

};

class Mobile : public Robot {
public:
    using Robot :: Robot;
    //DESTRUCTEUR
    virtual ~Mobile() {};
protected:
    void draw() const override {};
};

class Neutraliseur : public Mobile {
public:
    // CONSTRUCTEURS
    Neutraliseur() = delete;
    Neutraliseur(S2d position, double angle, int coordination, bool panne,
                 int k_update_panne, int nbUpdate);
    // GETTERS
    Cercle get_forme() const { return forme_; }
    //METHODES
    void draw() const override {};
    //DESTRUCTEUR
    virtual ~Neutraliseur() {};
private:
    Cercle forme_;
    double angle_;
    bool panne_;
    int coordination_;
    double vrot_ = 0;
    double vtran_ = 0;
};

class Reparateur : public Mobile {
public:
    // CONSTRUCTEURS
    Reparateur() = delete;
    explicit Reparateur(S2d position);
    // GETTERS
    Cercle get_forme() const { return forme_; }
    //METHODES
    void draw() const override {};
    //DESTRUCTEUR
    virtual ~Reparateur() {};
private:
    Cercle forme_;
};

#endif
