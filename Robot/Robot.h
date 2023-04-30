/**
 Robot.h
 Louis Grange et Daniel Ataide
 Version 1.2
**/

#include <vector>
#include "../Constantes.h"
#include "../Message/Message.h"
#include "../Shape/Shape.h"
#include "../Particule/Particule.h"
using namespace std;

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
public:
    virtual void draw() const = 0;
    virtual ~Robot() = default;
};

class Spatial : public Robot {
public:
    Spatial() = delete;
    Spatial(S2d position, int nbUpdate, int nbNr, int nbNs, int nbNd, int nbRr, int nbRs);
    Cercle get_forme() const { return forme_; }
    int get_update() const { return nbUpdate_; }
    int get_nbNr() const { return nbNr_; }
    int get_nbNs() const { return nbNs_; }
    int get_nbNd() const { return nbNd_; }
    int get_nbRr() const { return nbRr_; }
    int get_nbRs() const { return nbRs_; }
    string get_info() const;
    bool hors_domaine();
    void draw() const override;
    void clear();
    void set_update(int update);
    ~Spatial() override = default;
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
    ~Mobile() override = default;
protected:
    void draw() const override {};
};

class Neutraliseur : public Mobile {
public:
    Neutraliseur() = delete;
    Neutraliseur(S2d position, double angle, int coordination, bool panne,
                 int k_update_panne, int nbUpdate);
    Cercle get_forme() const { return forme_; }
    int get_k_update_panne() const { return k_update_panne_; }
    int get_nbUpdate() const { return nbUpdate_; }
    bool get_panne() const { return panne_; }
    void draw() const override;
    string get_info() const;
    virtual ~Neutraliseur() = default;


private:
    Cercle forme_;
    double angle_;
    bool panne_;
    int coordination_;
    double vrot_ = 0;
    double vtran_ = 0;
    int k_update_panne_;
    int nbUpdate_;
};

class Reparateur : public Mobile {
public:
    Reparateur() = delete;
    explicit Reparateur(S2d position);
    Cercle get_forme() const { return forme_; }
    void draw() const override;
    virtual ~Reparateur() = default;
private:
    Cercle forme_;
};

#endif
