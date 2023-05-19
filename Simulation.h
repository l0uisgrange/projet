/**
 Simulation.h
 Louis Grange et Daniel Ataide
 Version 1.2
**/

#include "Particule.h"
#include "Robot.h"
#include <iostream>
#include <fstream>
#include <vector>

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
    std::vector<Cercle> get_cercles();
    std::vector<Carre> get_carres();
    std::vector<Neutraliseur>& get_neutraliseurs() { return neutraliseurs_; };
    std::vector<Reparateur>& get_reparateurs() { return reparateurs_; };
    std::vector<Particule>& get_particules() { return particules_; };
    Spatial& get_spatial() { return spatial_; };
    void add_neutraliseur(Neutraliseur& N);
    void add_reparateur(Reparateur& R);
    void add_particule(Particule& P);
    void lecture(std::ifstream& entree);
    void erreurs_construction();
    void erreurs_superposition();
    void update_particules();
    void update_neutraliseurs();
    bool contact(Mobile& robot);
    void destroy_neutraliseurs();
    void draw_simulation();
    void update();
    void clear();
private:
    int nbP_;
    std::vector<Neutraliseur> neutraliseurs_;
    std::vector<Reparateur> reparateurs_;
    std::vector<Particule> particules_;
    Spatial spatial_;
    bool dessiner_;
};

void decodage_ligne(const std::string &line, Etat &etape, Simulation* sim);
void init_Particule(const std::string& ligne, Etat& etape, Simulation* sim);
void init_Spatial(const std::string& ligne, Etat& etape, Simulation* sim);
void init_Reparateur(const std::string& ligne, Etat& etape, Simulation* sim);
void init_Neutraliseur(const std::string& ligne, Simulation* sim);
void superposition_R_N(std::vector<Cercle>& tab1, Simulation* sim);
void superposition_P(std::vector<Carre>& tab1, Simulation* sim);
void superposition_P_R_N(std::vector<Cercle>& tab1, std::vector<Carre>& tab2,
                         Simulation* sim);
std::vector<Particule> tri_particules(std::vector<Particule>& particules);
bool alignement_particule(Carre &cible, Mobile &robot);
int choix_quadrant(double angle);
#endif
