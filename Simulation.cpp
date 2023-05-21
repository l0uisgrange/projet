/**
 Simulation.cpp
 Louis Grange et Daniel Ataide
 Version 1.5
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
    destroy_neutraliseurs();
    spatial_.update(particules_, neutraliseurs_, reparateurs_);
    spatial_.assigner_N(neutraliseurs_, particules_);
    spatial_.assigner_R(reparateurs_, neutraliseurs_);
    update_neutraliseurs();
    update_reparateurs();
}

void Simulation::update_particules() {
    vector<Particule> nouvelle_liste;
    bernoulli_distribution b(desintegration_rate/nbP_);
    for(auto& particule: particules_) {
        if(b(e) and particule.get_forme().cote/2.0 - 2*epsil_zero >=
        d_particule_min + epsil_zero) {
            // Création des 4 nouvelles particules
            double rayon = sqrt(2*pow(particule.get_forme().cote/4, 2));
            for(int i=0; i<4; i++) {
                Carre c;
                double angle = M_PI/2.0*i+M_PI/4.0;
                c.centre.x = particule.get_forme().centre.x + rayon*cos(angle);
                c.centre.y = particule.get_forme().centre.y + rayon*sin(angle);
                c.cote = particule.get_forme().cote/2.0 - 2*epsil_zero;
                nouvelle_liste.emplace_back(c);
            }
            Carre explosion(particule.get_forme());
            explosion.cote *= risk_factor;
            for(auto& neutraliseur : neutraliseurs_) {
                if(superposition(explosion, neutraliseur.get_forme())) {
                    neutraliseur.set_panne(true);
                    neutraliseur.set_k_update_panne(spatial_.get_update());
                }
            }
        } else {
            nouvelle_liste.push_back(particule);
        }
    }
    nbP_ = int(nouvelle_liste.size());
    particules_ = tri_particules(nouvelle_liste);
}

std::vector<Particule> tri_particules(std::vector<Particule>& p) {// Tri par insertion
    Carre c;
    Particule tmp(c);
    int j;
    for(int pos(1); pos<p.size(); ++pos) {
        tmp = p[pos];
        j = pos;
        while(j >= 1 and tmp.get_forme().cote > p[j-1].get_forme().cote) {
            p[j] = p[j-1];
            --j;
        }
        p[j] = tmp;
    }
    return p;
}

void Simulation::destroy_neutraliseurs() {
    for(int i = 0; i < neutraliseurs_.size(); i++) {
        if(neutraliseurs_[i].get_panne() and spatial_.get_update() -
        neutraliseurs_[i].get_k_update_panne() >= max_update) {
            neutraliseurs_[i] = neutraliseurs_[neutraliseurs_.size() - 1];
            neutraliseurs_.pop_back();
        }
    }
}

void Simulation::update_reparateurs() {
    double distance_minimale(5 * dmax);
    int id_r(-1);
    for(const auto& neutraliseur : neutraliseurs_) {
        if(neutraliseur.get_panne()) {
            for(int r = 0; r < reparateurs_.size(); r++) {
                if(reparateurs_[r].has_job()) {
                    continue;
                }
                S2d vecteur_distance = neutraliseur.get_forme().centre
                        - reparateurs_[r].get_forme().centre;
                double distance = vecteur_distance.norme();
                if(distance < distance_minimale
                   and distance < (max_update - (spatial_.get_update()
                   - neutraliseur.get_k_update_panne())) * vtran_max) {
                    id_r = r;
                    distance_minimale = distance;
                }
            }
        }
        if(id_r > -1) {
            Cercle forme = reparateurs_[id_r].get_forme();
            reparateurs_[id_r].move(neutraliseur.get_forme());
            if(contact(reparateurs_[id_r])) {
                reparateurs_[id_r].set_forme(forme);
            }
            reparateurs_[id_r].set_job(true);
        }
        distance_minimale = 5 * dmax;
    }
    for(int i = 0; i < reparateurs_.size(); i++) {
        if(!reparateurs_[i].has_job()) {
            reparateurs_[i].move(spatial_.get_forme());
            S2d vecteur_distance = reparateurs_[i].get_forme().centre - spatial_.get_forme().centre;
            if(vecteur_distance.norme() <= r_spatial) {
                spatial_.set_nbRs(spatial_.get_nbRs() - 1);
                spatial_.set_nbRr(spatial_.get_nbRr() + 1);
                reparateurs_[i] = reparateurs_[reparateurs_.size() - 1];
                reparateurs_.pop_back();
            }
        } else {
            reparateurs_[i].set_job(false);
        }
    }
}

bool Simulation::contact(Mobile& robot) {
    for(int i(0); i < particules_.size(); ++i) {
        if(superposition(particules_[i].get_forme(),
                         robot.get_forme(), true)) {
            if(robot.get_forme().rayon == r_neutraliseur) {
                robot.set_collision(true);
                if(alignement_particule(particules_[i].get_forme(), robot)){
                    particules_[i]=particules_[particules_.size()-1];
                    particules_.pop_back();
                }
            } else {
                robot.set_collision(false);
            }
            return true;
        }
    }
    for(auto& neutraliseur : neutraliseurs_) {
        if(superposition(neutraliseur.get_forme(),
                         robot.get_forme(), true)
                         and neutraliseur.get_forme() != robot.get_forme()) {
            if(robot.get_forme().rayon == r_reparateur and neutraliseur.get_panne()
            and robot.has_job()) {
                neutraliseur.set_panne(false);
                robot.set_job(false);
            }
            return true;
        }
    }
    for(auto& reparateur : reparateurs_) {
        if(superposition(reparateur.get_forme(),
                         robot.get_forme(), true)
                         and reparateur.get_forme() != robot.get_forme()) {
            return true;
        }
    }
    robot.set_collision(false);
    return false;
}

bool alignement_particule(Carre &cible, Mobile &robot) {
    bool coin(is_coin(cible, robot));
    S2d direction(cible.centre - robot.get_forme().centre);
    double angle_direction(atan2(direction.y, direction.x));
    double delta_angle(angle_direction - robot.get_angle());
    int quadrant(choix_quadrant(delta_angle));
    Carre new_cible(cible);
    new_cible.centre = robot.get_forme().centre;
    if(coin) {
        new_cible = cible;
    }
    switch(quadrant) {
        case 1: {
            new_cible.centre.y -= r_neutraliseur;
            break;
        }
        case 2:
            new_cible.centre.x += r_neutraliseur;
            break;
        case 3:
            new_cible.centre.y += r_neutraliseur;
            break;
        case 4:
            new_cible.centre.x -= r_neutraliseur;
    }
    robot.turn(new_cible);
    if(fmod(abs(robot.get_angle()), M_PI/2) < epsil_alignement and !coin) {
        robot.set_collision(false);
        return true;
    } else if (abs(delta_angle) < epsil_alignement and coin) {
         robot.set_collision(false);
        return true;
    }
    return false;
}

int choix_quadrant(double angle) { // Pour savoir quelle face on touche
    if(angle < 3*M_PI/4 and angle >= M_PI/4) {
        return 1;
    } else if(angle <= M_PI/4 and angle > -M_PI/4) {
        return 2;
    } else if(angle < -M_PI/4 and angle >= -3*M_PI/4) {
        return 3;
    } else {
        return 4;
    }
}

bool is_coin(Carre &cible, Mobile &robot){
    //distance min dont le robot doit être
    double rayon_lim(sqrt(pow(cible.cote/2,2) + pow(cible.cote/2+r_neutraliseur,2)));
    S2d vect_direction(cible.centre - robot.get_forme().centre);
    double distance_robot(vect_direction.norme());
    if(rayon_lim > distance_robot) {
        return false;
    } else {
        return true;
    }
}

void Simulation::update_neutraliseurs() {
    double distance_minimale;
    int id_n;
    int id_p;
    bool var;
    for(int p = 0; p < particules_.size(); p++) {
        if(particules_[p].is_target()) {
            continue;
        }
        var = true;
        while(var) {
            distance_minimale = 5 * dmax;
            id_n = -1;
            for(int n = 0; n < neutraliseurs_.size(); n++) {
                if(!neutraliseurs_[n].has_job() and !neutraliseurs_[n].get_panne()) {
                    S2d vecteur_distance = neutraliseurs_[n].get_forme().centre
                            - particules_[p].get_forme().centre;
                    double distance_temps = vecteur_distance.norme() * vtran_max;
                    double angle_direction(atan2(vecteur_distance.y, vecteur_distance.x));
                    double delta_angle(angle_direction - neutraliseurs_[n].get_angle());
                    normalise_delta(delta_angle);
                    distance_temps += delta_angle / vrot_max;
                    if(distance_temps < distance_minimale) {
                        distance_minimale = distance_temps;
                        id_n = n;
                    }
                }
            }
            distance_minimale = 5 * dmax;
            double cote = particules_[p].get_forme().cote;
            id_p = p;
            if(id_n > -1) {
                for(int a = 0; a < particules_.size(); a++) {
                    if(particules_[a].get_forme().cote < cote or particules_[a].is_target()) {
                        continue;
                    }
                    S2d vecteur_distance = neutraliseurs_[id_n].get_forme().centre
                            - particules_[a].get_forme().centre;
                    double distance = vecteur_distance.norme();
                    if(distance < distance_minimale) {
                        distance_minimale = distance;
                        id_p = a;
                    }
                }
                Cercle forme = neutraliseurs_[id_n].get_forme();
                cout << endl << "timer:  " << spatial_.get_update() << endl;
                if(!neutraliseurs_[id_n].get_collision()) {
                    neutraliseurs_[id_n].turn(particules_[id_p].get_forme());
                }
                neutraliseurs_[id_n].move(particules_[id_p].get_forme());
                if(contact(neutraliseurs_[id_n])) {
                    neutraliseurs_[id_n].set_forme(forme);
                }
                neutraliseurs_[id_n].set_job(true);
                particules_[id_p].set_target(true);
                if(id_p != p) {
                    var = true;
                } else {
                    var = false;
                }
            } else {
                var = false;
            }
        }
    }
    for(auto& neutraliseur : neutraliseurs_) {
        neutraliseur.set_job(false);
    }
    for(auto& particule : particules_) {
        particule.set_target(false);
    }
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
    for(const auto& N : neutraliseurs_) {
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
        sim->get_particules().push_back(P);
        if(int(sim->get_particules().size()) == sim->get_nbP()) {
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
        Spatial S(position, nbUpdate, nbNr, nbNs, nbNd, nbRr, nbRs);
        sim->set_Spatial(S);
        etape = REPARATEUR;
    } else {
        sim->set_dessiner(false);
    }
}

void init_Reparateur(const string& line, Etat& etape, Simulation* sim) {
    istringstream ligne(line);
    S2d position;
    if(sim->get_spatial().get_nbRs() == 0){
        etape = NEUTRALISEUR;
        init_Neutraliseur(line, sim);
        return;
    }
    if(ligne >> position.x >> position.y) {
        if(int(sim->get_reparateurs().size()+1) == sim->get_spatial().get_nbRs()){
            etape = NEUTRALISEUR;
        }
        Reparateur R(position);
        sim->get_reparateurs().push_back(R);
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
            sim->get_neutraliseurs().push_back(N);
        }
    } else {
        sim->set_dessiner(false);
    }
}

int Simulation::get_nbNp(){
    int nbNp(0);
    for(auto& N: neutraliseurs_) {
        if(N.get_panne()) {
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
    for(auto& particule : particules_) {
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
