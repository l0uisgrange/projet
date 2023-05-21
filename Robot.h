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

typedef std::vector<Particule> V_particule;

class Robot {
public:
    virtual void draw() const = 0;
    virtual ~Robot() = default;
};

class Mobile : public Robot {
public:
    using Robot::Robot;
    ~Mobile() override = default;
    virtual void set_forme(Cercle c) { forme_ = c; }
    virtual Cercle & get_forme() const { return forme_; }
    virtual void set_job(bool b) { job_ = b; }
    virtual bool has_job() const { return job_; }
    virtual void set_collision(bool b) { collision_ = b; }
    virtual void turn(Carre& cible) {};
    virtual double& get_angle() {}
    virtual void set_angle(double angle) {}
    virtual double& get_vrot() {}
    virtual void set_vrot(double vrot) {}
protected:
    void draw() const override {};
private:
    Cercle forme_;
    bool job_;
    bool collision_;
};

class Neutraliseur : public Mobile {
public:
    Neutraliseur() = delete;
    Neutraliseur(S2d position, double angle, int coordination, bool panne,
                 int k_update_panne, int nbUpdate);
    Cercle& get_forme() const override { return forme_; }
    int get_k_update_panne() const { return k_update_panne_; }
    int get_nbUpdate() const { return nbUpdate_; }
    bool get_panne() const { return panne_; }
    double& get_angle() override { return angle_; }
    double& get_vrot() override { return vrot_; }
    bool get_collision() { return collision_; }
    void draw() const override;
    void turn(Carre& cible) override;
    void move(Carre cible);
    std::string get_info() const;
    void set_angle(double angle) override {angle_ = angle;}
    void set_panne(bool panne) {panne_ = panne;}
    void set_k_update_panne(int update);
    void set_collision(bool b) override { collision_ = b; }
    void set_forme(Cercle c) override { forme_ = c; }
    void set_vrot(double vrot) override { vrot_ = vrot; }
    void set_job(bool b) override { job_ = b; }
    bool has_job() const override { return job_; }
    virtual ~Neutraliseur() = default;
private:
    Cercle forme_;
    double angle_;
    bool panne_;
    bool collision_;
    int coordination_;
    double vrot_ = vrot_max;
    double vtran_ = vtran_max;
    int k_update_panne_;
    int nbUpdate_;
    bool job_;
};

typedef std::vector<Neutraliseur> V_neutraliseur;

class Reparateur : public Mobile {
public:
    Reparateur() = delete;
    explicit Reparateur(S2d position);
    Cercle& get_forme() const override { return forme_; }
    void set_forme(Cercle c) override { forme_ = c; }
    S2d& get_but() { return but_; }
    void set_but(S2d n) { but_ = n; }
    void draw() const override;
    void move();
    void set_job(bool b) override { job_ = b; }
    bool has_job() const override { return job_; }
    virtual ~Reparateur() = default;
private:
    Cercle forme_;
    S2d but_;
    bool job_;
};

typedef std::vector<Reparateur> V_reparateur;

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
    bool hors_domaine() const;
    void draw() const override;
    void clear();
    void set_update(int update);
    void set_nbRr(int nb) { nbRr_ = nb; }
    void set_nbRs(int nb) { nbRs_ = nb; }
    void set_nbNs(int nb) { nbNs_ = nb; }
    void set_nbNr(int nb) { nbNr_ = nb; }
    void update(V_particule& particules,
                V_neutraliseur& neutraliseurs,
                V_reparateur& reparateurs);
    void assigner_N(V_neutraliseur& neutraliseurs, V_particule& particules) const;
    void assigner_R(V_reparateur& reparateurs, V_neutraliseur& neutraliseurs) const;
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

S2d direction_type1(Neutraliseur* N, Carre cible);
double normalise_delta(double& delta_angle);
double choix_vrot(double& delta_angle);
V_neutraliseur creer_neutraliseurs_detresse(V_reparateur& reparateurs,
            V_neutraliseur &neutraliseurs);
void choix_R_ou_N(Spatial *spatial, bool& spawn_R, bool& spawn_N,
                  int& nbP, int& nbN_detresse);
void creation_reparateur(Spatial *spatial, bool &spawn_N, bool &spawn_R,
                         V_neutraliseur &neutraliseurs_detresse,
                         V_neutraliseur& neutraliseurs, V_reparateur& reparateurs);
void creation_neutraliseur(Spatial *spatial, V_neutraliseur &neutraliseurs,
                           V_particule& particules, V_reparateur& reparateurs,
                           bool& spawn_N);
Particule trouver_P_proche(Spatial *spatial, V_particule& particules_libres);
//vérifie superposition pour création de robot
bool single_superposition_R_N(V_neutraliseur& neutraliseurs,
                              V_reparateur& reparateurs, Cercle& new_N);

#endif
