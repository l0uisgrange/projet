/**
 Simulation.h
 Louis Grange et Daniel Ataide
 Version 1.1
**/

#include "../Particule/Particule.h"
#include "../Robot/Robot.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation {
public:
    Simulation() = delete;
    explicit Simulation(int nbP);
    void set_nbP(int value);
    void set_Spatial(Spatial& S);
    int get_nbP() const { return nbP_; };
    int get_nbNp();
    bool get_dessiner() const { return dessiner_; }
    void set_dessiner(bool dessin);
    vector<Cercle> get_cercles();
    vector<Carre> get_carres();
    vector<Neutraliseur> get_neutraliseurs() { return neutraliseurs_; };
    vector<Reparateur> get_reparateurs() { return reparateurs_; };
    vector<Particule> get_particules() { return particules_; };
    Spatial& get_spatial() { return spatial_; };
    void add_neutraliseur(Neutraliseur& N);
    void add_reparateur(Reparateur& R);
    void add_particule(Particule& P);
    void lecture(ifstream& entree);
    void erreurs_construction();
    void erreurs_superposition();
    void update_particules();
    void draw_simulation();
    void update();
    void clear();
private:
    int nbP_;
    vector<Neutraliseur> neutraliseurs_;
    vector<Reparateur> reparateurs_;
    vector<Particule> particules_;
    Spatial spatial_;
    bool dessiner_;
};

void decodage_ligne(const string &line, Etat &etape, Simulation* sim);
void init_Particule(const string& ligne, Etat& etape, Simulation* sim);
void init_Spatial(const string& ligne, Etat& etape, Simulation* sim);
void init_Reparateur(const string& ligne, Etat& etape, Simulation* sim);
void init_Neutraliseur(const string& ligne, Simulation* sim);
void superposition_R_N(vector<Cercle>& tab1, Simulation* sim);
void superposition_P(vector<Carre>& tab1, Simulation* sim);
void superposition_P_R_N(vector<Cercle>& tab1, vector<Carre>& tab2, Simulation* sim);

#endif