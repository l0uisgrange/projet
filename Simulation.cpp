/**
 Simulation.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Simulation.h"
#include "Constantes.h"
#include <random>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

static default_random_engine e;

void Simulation::update() {
    spatial_.set_update(spatial_.get_update() + 1);
    update_particules();
}

void Simulation::update_particules() {
    vector<Particule> nouvelle_liste;
    bernoulli_distribution b(desintegration_rate/nbP_);
    for(auto& particule: particules_) {
        if(particule.get_forme().cote/2.0 - 2*epsil_zero >=
        d_particule_min + epsil_zero) {
            if(b(e)) {
                double rayon = sqrt(2*pow(particule.get_forme().cote/4, 2));
                for(int i=0; i<4; i++) {
                    Carre c;
                    double angle = (90*i+45)*3.14159/180;
                    c.centre.x = particule.get_forme().centre.x + rayon*cos(angle);
                    c.centre.y = particule.get_forme().centre.y + rayon*sin(angle);
                    double cote = particule.get_forme().cote;
                    c.cote = cote/2.0 - 2*epsil_zero;
                    Particule new_p(c);
                    nouvelle_liste.push_back(new_p);
                }
            } else {
                nouvelle_liste.push_back(particule);
            }
        } else {
            nouvelle_liste.push_back(particule);
        }
    }
    nbP_ = int(nouvelle_liste.size());
    particules_ = nouvelle_liste;
}

void Simulation::set_Spatial(Spatial &S) {
    spatial_ = S;
}

void superposition_R_N(vector<Cercle>& tab1, Simulation* sim) {
    for(int i(0); i<int(tab1.size()); ++i) {
        for(int j(0); j<int(tab1.size()); ++j) {
            if(superposition(tab1[i],tab1[j]) and i!=j) {
                if((tab1[i].rayon == r_neutraliseur) and
                (tab1[j].rayon == r_neutraliseur)) {
                    cout << message::neutralizers_superposition(tab1[i].centre.x,
                            tab1[i].centre.y, tab1[j].centre.x, tab1[j].centre.y);
                    sim->set_dessiner(false);
                    return;
                } else if((tab1[i].rayon == r_reparateur) and
                        (tab1[j].rayon == r_reparateur)) {
                    cout << message::repairers_superposition(tab1[i].centre.x,
                            tab1[i].centre.y, tab1[j].centre.x, tab1[j].centre.y);
                    sim->set_dessiner(false);
                    return;
                } else {
                    if(tab1[i].rayon == r_neutraliseur) {
                        cout << message::repairer_neutralizer_superposition(
                                tab1[j].centre.x,tab1[j].centre.y,
                                tab1[i].centre.x, tab1[i].centre.y);
                        sim->set_dessiner(false);
                        return;
                    } else {
                        cout << message::repairer_neutralizer_superposition(
                                tab1[i].centre.x, tab1[i].centre.y,
                                tab1[j].centre.x, tab1[j].centre.y);
                        sim->set_dessiner(false);
                        return;
                    }
                }
            }
        }
    }
}

void superposition_P(vector<Carre>& tab1, Simulation* sim) {
    for(int i(0); i<int(tab1.size()) ; ++i) {
        for(int j(0); j<int(tab1.size()) ; ++j) {
            if((superposition(tab1[i],tab1[j])) and (i!=j)) {
                cout << message::particle_superposition(tab1[i].centre.x,
                            tab1[i].centre.y, tab1[j].centre.x, tab1[j].centre.y);
                sim->set_dessiner(false);
                return;
            }
        }
    }
}

void superposition_P_R_N(vector<Cercle>& tab1, vector<Carre>& tab2, Simulation* sim) {
    for(auto& i : tab1) {
        for(auto& j : tab2) {
            if(superposition(j,i)) {
                cout << message::particle_robot_superposition(j.centre.x, j.centre.y,
                                        j.cote, i.centre.x, i.centre.y, i.rayon);
                sim->set_dessiner(false);
                return;
            }
        }
    }
}

void Simulation::erreurs_superposition() {
    vector<Cercle> cercles(get_cercles());
    vector<Carre> carres(get_carres());
    superposition_P(carres, this);
    if(dessiner_) {
        superposition_R_N(cercles, this);
        if(dessiner_) {
            cercles.push_back(get_spatial().get_forme());
            superposition_P_R_N(cercles, carres, this);
        }
    }
}

void Simulation::lecture(ifstream& entree) {
    string line;
    e.seed(1);
    if(!entree.fail()) {
        Etat etape(NBP);
        while(getline(entree >> ws, line)) {
            if(line.empty() or line[0] == '#') {
                continue;
            }
            decodage_ligne(line, etape, this);
        }
        entree.close();
        erreurs_construction();
        if(dessiner_) {
            erreurs_superposition();
        }
        if(dessiner_) {
            cout << message::success();
        } else {
            clear();
        }
    } else {
        dessiner_ = false;
    }
}

void Simulation::clear() {
    nbP_ = 0;
    neutraliseurs_.clear();
    reparateurs_.clear();
    particules_.clear();
    spatial_.clear();
}

void decodage_ligne(const string& line, Etat& etape, Simulation* simulation) {
    istringstream ligne(line);
    switch(etape) {
        case NBP: {
            int nbP(0);
            if(ligne >> nbP) {
                simulation->set_nbP(nbP);
                etape = PARTICULE;
            } else {
                simulation->set_dessiner(false);
            }
            break;
        }
        case PARTICULE:
            init_Particule(line, etape, simulation);
            break;
        case SPATIAL:
            init_Spatial(line, etape, simulation);
            break;
        case REPARATEUR:
            init_Reparateur(line, etape, simulation);
            break;
        case NEUTRALISEUR:
            init_Neutraliseur(line, simulation);
            break;
    }
}

void Simulation::erreurs_construction() {
    dessiner_ = true;
    for(auto P : particules_) {
        if(P.too_small() or P.hors_domaine()) {
            dessiner_ = false;
        }
    }
    if(spatial_.hors_domaine()) {
        dessiner_ = false;
    }
    for(auto N : neutraliseurs_) {
        if(N.get_k_update_panne() > N.get_nbUpdate()) {
            cout << message::invalid_k_update(N.get_forme().centre.x,
                              N.get_forme().centre.y,
                              N.get_k_update_panne(), N.get_nbUpdate());
            dessiner_ = false;
        }
    }
}


void Simulation::set_nbP(int value) {
    if(value >= 0) {
        nbP_ = value;
    }
}

void Simulation::set_dessiner(bool dessin) {
    dessiner_ = dessin;
}

void init_Particule(const string& line, Etat& etape, Simulation* sim) {
    istringstream ligne(line);
    Carre c;
    if(ligne >> c.centre.x >> c.centre.y >> c.cote) {
        Particule P(c);
        sim->add_particule(P);
        if(sim->get_particules().size() == sim->get_nbP()) {
            etape = SPATIAL;
        }
    } else {
        sim->set_dessiner(false);
    }
}

void init_Spatial(const string& line, Etat& etape, Simulation* sim) {
    istringstream ligne(line);
    S2d position;
    int nbUpdate, nbNr, nbNs, nbNd, nbRr, nbRs;
    if(ligne >> position.x >> position.y >> nbUpdate >> nbNr >>
        nbNs >> nbNd >> nbRr >> nbRs) {
        Spatial S(position, nbUpdate, nbNr, nbNs, nbNd, nbRr, nbNs);
        sim->set_Spatial(S);
        etape = REPARATEUR;
    } else {
        sim->set_dessiner(false);
    }
}

void init_Reparateur(const string& line, Etat& etape, Simulation* sim) {
    istringstream ligne(line);
    S2d position;
    if(ligne >> position.x >> position.y) {
        Reparateur R(position);
        sim->add_reparateur(R);
        if(sim->get_reparateurs().size() == sim->get_spatial().get_nbRs()) {
            etape = NEUTRALISEUR;
        }
    } else {
        sim->set_dessiner(false);
    }
}

void init_Neutraliseur(const string& line, Simulation* sim) {
    istringstream ligne(line);
    S2d position;
    double a1;
    int c_n, k_update_panne;
    bool panne;
    string h;
    if(ligne >> position.x >> position.y >> a1 >> c_n >> h >> k_update_panne) {
        if(h == "false") {
            panne = false;
        } else if(h == "true") {
            panne = true;
        } else {
            sim->set_dessiner(false);
            return;
        }
        if(int(sim->get_neutraliseurs().size()) < sim->get_spatial().get_nbNs()) {
            Neutraliseur N(position, a1, c_n, panne,
                           k_update_panne,
                           sim->get_spatial().get_update());
            sim->add_neutraliseur(N);
        }
    } else {
        sim->set_dessiner(false);
    }
}

void Simulation::add_particule(Particule& P){
    particules_.push_back(P);
}

void Simulation::add_reparateur(Reparateur& R) {
    reparateurs_.push_back(R);
}

void Simulation::add_neutraliseur(Neutraliseur& N) {
    neutraliseurs_.push_back(N);
}

int Simulation::get_nbNp(){
    int nbNp(0);
    for(auto& N: neutraliseurs_) {
        if (N.get_panne()) {
            nbNp++;
        }
    }
    return nbNp;
}

vector<Cercle> Simulation::get_cercles() {
    vector<Cercle> tab;
    for(const auto& neutraliseur : neutraliseurs_) {
        tab.push_back(neutraliseur.get_forme());
    }
    for(const auto& reparateur : reparateurs_) {
        tab.push_back(reparateur.get_forme());
    }
    return tab;
}

vector<Carre> Simulation::get_carres() {
    vector<Carre> tab;
    tab.reserve(particules_.size());
    for(const auto& particule : particules_) {
        tab.push_back(particule.get_forme());
    }
    return tab;
}

Simulation::Simulation(int nbP)
    : nbP_(nbP), spatial_(Spatial(S2d(), 0, 0, 0, 0, 0, 0)), dessiner_(false) {}

void Simulation::draw_simulation() {
    spatial_.draw();
    for(const auto &neutraliseur: neutraliseurs_) {
        neutraliseur.draw();
    }
    for(const auto &reparateur: reparateurs_) {
        reparateur.draw();
    }
    for(const auto &particule: particules_) {
        particule.draw();
    }

}
