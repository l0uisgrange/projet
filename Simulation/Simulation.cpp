/**
 Simulation.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Simulation.h"
#include "../Constantes.h"
#include <random>
#include <sstream>
#include <vector>
using namespace std;

void Simulation::update() {
    spatial_.set_update(spatial_.get_update() + 1);
    default_random_engine e;
    for(auto& particule: particules_) {
        bernoulli_distribution b(desintegration_rate/nbP_);
        cout << "Nouvelle particule :";
        if(particule.get_forme().cote > d_particule_min + epsil_zero) {
            if(b(e)) {
                cout << " oui" << endl;
                double rayon = sqrt(2*pow(particule.get_forme().cote, 2));
                for(int i=0; i<4; i++) {
                    Carre c;
                    c.centre.x = particule.get_forme().centre.x+rayon*cos(90*i+45);
                    c.centre.y = particule.get_forme().centre.y+rayon*sin(90*i+45);
                    cout << "Particule à " << particule.get_forme().centre.x << " de côté " << particule.get_forme().cote << endl;
                    c.cote = particule.get_forme().cote/2.0-2*epsil_zero;
                    Particule new_p(c);
                    particules_.push_back(new_p);
                }
                supp_particule(particule);
            } else {
                cout << " non, chance" << endl;
            }
        } else {
            cout << " non, petite" << endl;
        }
    }
}

void Simulation::supp_particule(Particule particule) {
    vector<Particule> nouvelle_liste;
    for(auto& part: particules_) {
        if(part.get_forme().centre.x != particule.get_forme().centre.x and
        part.get_forme().centre.y != particule.get_forme().centre.y) {
            nouvelle_liste.push_back(part);
        }
    }
    particules_ = nouvelle_liste;
}

void Simulation::set_Spatial(Spatial &S) {
    spatial_ = S;
}

void superposition_R_N(vector<Cercle>& tab1) {
    for(int i(0); i<tab1.size(); ++i) {
        for(int j(0); j<tab1.size(); ++j) {
            if(superposition(tab1[i],tab1[j]) and i!=j) {
                if((tab1[i].rayon == r_neutraliseur) and
                (tab1[j].rayon == r_neutraliseur)) {
                    cout << message::neutralizers_superposition(tab1[i].centre.x,
                                                        tab1[i].centre.y,
                                                        tab1[j].centre.x,
                                                        tab1[j].centre.y);
                    exit(EXIT_FAILURE);
                } else if((tab1[i].rayon == r_reparateur) and
                        (tab1[j].rayon == r_reparateur)) {
                    cout << message::repairers_superposition(tab1[i].centre.x,
                                                     tab1[i].centre.y,
                                                     tab1[j].centre.x,
                                                     tab1[j].centre.y);
                    exit(EXIT_FAILURE);
                } else {
                    if(tab1[i].rayon == r_neutraliseur) {
                        cout << message::repairer_neutralizer_superposition(tab1[j].centre.x,
                                                                    tab1[j].centre.y,
                                                                    tab1[i].centre.x,
                                                                    tab1[i].centre.y);
                        exit(EXIT_FAILURE);
                    } else {
                        cout << message::repairer_neutralizer_superposition(tab1[i].centre.x,
                                                                    tab1[i].centre.y,
                                                                    tab1[j].centre.x,
                                                                    tab1[j].centre.y);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
}

void superposition_P(vector<Carre>& tab1) {
    for(int i(0); i<tab1.size() ; ++i) {
        for(int j(0); j<tab1.size() ; ++j) {
            if((superposition(tab1[i],tab1[j])) and (i!=j)) {
                cout << message::particle_superposition(tab1[i].centre.x,
                                                        tab1[i].centre.y,
                                                        tab1[j].centre.x,
                                                        tab1[j].centre.y);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void superposition_P_R_N(vector<Cercle>& tab1, vector<Carre>& tab2) {
    for(auto& i : tab1) {
        for(auto& j : tab2) {
            if(superposition(j,i)) {
                cout << message::particle_robot_superposition(j.centre.x, j.centre.y,
                                                      j.cote, i.centre.x, i.centre.y,
                                                      i.rayon);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void superposition_erreurs(Simulation* sim) {
    vector<Cercle> cercles(sim->get_cercles());
    vector<Carre> carres(sim->get_carres());
    superposition_P(carres);
    superposition_R_N(cercles);
    cercles.push_back(sim->get_spatial().get_forme());
    superposition_P_R_N(cercles, carres);
}

void Simulation::lecture(ifstream& entree) {
    string line;
    if(!entree.fail()) {
        Etat etape(NBP);
        while(getline(entree >> ws, line)) {
            if(line.empty() or line[0] == '#') {
                continue;
            }
            decodage_ligne(line, etape, this);
        }
        entree.close();
        superposition_erreurs(this);
        cout << message::success();
    } else {
        exit(0);
    }
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
                exit(EXIT_FAILURE);
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

void Simulation::set_nbP(int value) {
    if(value >= 0) {
        nbP_ = value;
    }
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
        if(sim->get_neutraliseurs().size() < sim->get_spatial().get_nbNs()) {
            Neutraliseur N(position, a1, c_n, panne,
                           k_update_panne,
                           sim->get_spatial().get_update());
            sim->add_neutraliseur(N);
        }
    } else {
        exit(EXIT_FAILURE);
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
    : nbP_(nbP), spatial_(Spatial(S2d(), 0, 0, 0, 0, 0, 0)) {
}

void Simulation::draw_simulation() {
    spatial_.draw();
    for (const auto &neutraliseur: neutraliseurs_) {
        neutraliseur.draw();
    }
    for (const auto &reparateur: reparateurs_) {
        reparateur.draw();
    }
    for (const auto &particule: particules_) {
        particule.draw();
    }
}
