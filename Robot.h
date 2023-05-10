/**
 Robot.h
 Louis Grange et Daniel Ataide
 Version 1.4
**/

#include <vector>
#include "Constantes.h"
#include "Message.h"
#include "Shape.h"
#include "Particule.h"

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
public:
    virtual void draw() const = 0;
    virtual ~Robot() = default;
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
    void turn(Carre cible);
    void move();
    std::string get_info() const;
    void set_panne(bool panne) {panne_ = panne;}
    void set_k_update_panne(int update);
    void set_job(bool b) { job_ = b; }
    bool has_job() const { return job_; }
    virtual ~Neutraliseur() = default;
private:
    Cercle forme_;
    double angle_;
    bool panne_;
    int coordination_;
    double vrot_ = vrot_max;
    double vtran_ = vtran_max;
    int k_update_panne_;
    int nbUpdate_;
    bool job_;
};

class Reparateur : public Mobile {
public:
    Reparateur() = delete;
    explicit Reparateur(S2d position);
    Cercle get_forme() const { return forme_; }
    void draw() const override;
    void move(Cercle cible);
    virtual ~Reparateur() = default;
private:
    Cercle forme_;
};

class Spatial : public Robot {
public:
    Spatial() = delete;
    Spatial(S2d position, int nbUpdate, int nbNr, int nbNs, int nbNd,
            int nbRr, int nbRs);
    Cercle get_forme() const { return forme_; }
    int get_update() const { return nbUpdate_; }
    int get_nbNr() const { return nbNr_; }
    int get_nbNs() const { return nbNs_; }
    int get_nbNd() const { return nbNd_; }
    int get_nbRr() const { return nbRr_; }
    int get_nbRs() const { return nbRs_; }
    std::string get_info() const;
    bool hors_domaine();
    static int assigner_cible(const std::vector<Neutraliseur>& neutraliseurs, const Particule& particule);
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

#endif
