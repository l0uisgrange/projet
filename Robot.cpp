/**
 Robot.cpp
 Louis Grange et Daniel Ataide
 Version 1.1
**/

#include "Robot.h"
using namespace std;

Spatial::Spatial(S2d position, int nbUpdate, int nbNr, int nbNs, int nbNd,
                 int nbRr, int nbRs)
        : nbUpdate_(nbUpdate), nbNr_(nbNr), nbNs_(nbNs), nbNd_(nbNd),
          nbRr_(nbRr), nbRs_(nbRs) {
    forme_.centre.x = position.x;
    forme_.centre.y = position.y;
    forme_.rayon = r_spatial;
}

void Spatial::clear() {
    nbUpdate_ = 0;
    nbNr_ = 0;
    nbNd_ = 0;
    nbRs_ = 0;
    nbRr_ = 0;
    nbNs_ = 0;
}

void Spatial::draw() const {
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, BLEU_CLAIR);
    draw_cercle(forme_.centre.x, forme_.centre.y, 0.5, BLEU_CLAIR);
    fill_cercle(forme_.centre.x, forme_.centre.y, 0.5, BLEU_CLAIR);
}

string Spatial::get_info() const {
    ostringstream info;
    info << forme_.centre.x << " " << forme_.centre.y << " " << nbUpdate_ << " ";
    info << nbNr_ << " " << nbNs_ << " " << nbNd_ << " " << nbRr_ << " " << nbRs_;
    return info.str();
}

bool Spatial::hors_domaine() const {
    if(abs(forme_.centre.x) < dmax - r_spatial
       and abs(forme_.centre.y) < dmax - r_spatial) {
        return false;
    } else {
        cout << message::spatial_robot_ouside(forme_.centre.x, forme_.centre.y);
        return true;
    }
}

Neutraliseur::Neutraliseur(S2d position, double angle, int coordination, bool panne,
                           int k_update_panne, int nbUpdate)
    : angle_(angle), panne_(panne), collision_(false), coordination_(coordination),
    k_update_panne_(k_update_panne), nbUpdate_(nbUpdate), job_(false) {
    forme_.centre.x = position.x;
    forme_.centre.y = position.y;
    forme_.rayon = r_neutraliseur;
}

void Neutraliseur::draw() const {
    Couleurs couleur(NOIR);
    if(collision_) {
        couleur = VIOLET;
    }
    if(panne_) {
        couleur = ORANGE;
    }
    double x_stop = forme_.centre.x + std::cos(angle_) * r_neutraliseur;
    double y_stop = forme_.centre.y + std::sin(angle_) * r_neutraliseur;
    draw_line(forme_.centre.x, forme_.centre.y, x_stop, y_stop, VERT);
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, couleur);
    draw_cercle(forme_.centre.x, forme_.centre.y, 0.5, couleur);
    fill_cercle(forme_.centre.x, forme_.centre.y, 0.5, couleur);
}

string Neutraliseur::get_info() const {
    string panne;
    if(panne_) {
        panne = "true";
    } else {
        panne = "false";
    }
    ostringstream info;
    info << forme_.centre.x << " " << forme_.centre.y << " " << angle_ << " ";
    info << coordination_ << " " << panne << " " << k_update_panne_;
    return info.str();
}

void Neutraliseur::set_k_update_panne(int update) {
    k_update_panne_ = update;
}

Reparateur::Reparateur(S2d position)
    : forme_(Cercle(position, r_reparateur)) {}

void Reparateur::draw() const {
    fill_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, VERT);
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, NOIR);
}

void Reparateur::move() {
    S2d direction = but_ - forme_.centre;
    S2d direction_normalisee;
    double distance = direction.norme();
    direction_normalisee.x = direction.x / distance;
    direction_normalisee.y = direction.y / distance;
    double vitesse = vtran_max;
    if(distance < r_neutraliseur) {
        vitesse = vtran_max / 2;
    }
    forme_.centre.x += direction_normalisee.x * vitesse * delta_t;
    forme_.centre.y += direction_normalisee.y * vitesse * delta_t;
}

void Neutraliseur::turn(Carre& cible) {
    S2d direction;
    if(coordination_ == 1) {
        Carre zone_danger_large;
        zone_danger_large.centre = cible.centre;
        zone_danger_large.cote = cible.cote * risk_factor + r_neutraliseur + 1;
        if(superposition(zone_danger_large, this->forme_)) {
            direction = cible.centre - forme_.centre;
        } else {
            direction = direction_type1(this, cible) - forme_.centre;
        }
    } else {
        direction = cible.centre - forme_.centre;
    }
    double angle_direction(atan2(direction.y, direction.x));
    double delta_angle(angle_direction - angle_);
    normalise_delta(delta_angle);
    if(abs(delta_angle) <= vrot_*delta_t){
        angle_ = angle_direction;
    } else {
        angle_ += ((delta_angle > 0)? 1. : -1.)*vrot_*delta_t;
    }
}

void Neutraliseur::move(Carre cible) {
    S2d direction;
    if(coordination_ == 1) {
        Carre zone_danger_large;
        zone_danger_large.centre = cible.centre;
        zone_danger_large.cote = cible.cote * risk_factor + r_neutraliseur + 1;
        if(superposition(zone_danger_large, this->forme_)) {
            direction = cible.centre - forme_.centre;
        } else {
            direction = direction_type1(this, cible) - forme_.centre;
        }
    } else {
        direction = cible.centre - forme_.centre;
    }
    double angle_direction(atan2(direction.y, direction.x));
    double delta_angle(angle_direction - angle_);
    normalise_delta(delta_angle);
    vrot_ = choix_vrot(delta_angle);
    S2d vect_angle;
    vect_angle.x = cos(angle_);
    vect_angle.y = sin(angle_);
    switch(coordination_) {
        case 0: {
            if(abs(delta_angle) < epsil_alignement or collision_) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
        }
        case 1:
            if(abs(delta_angle) < epsil_alignement or collision_) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
        case 2:
            if(abs(delta_angle) < M_PI/3 or collision_) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
    }
}

S2d direction_type1(Neutraliseur* N, Carre cible) {
    vector<S2d> points;
    double distance = (cible.cote * risk_factor)/2.0 + r_neutraliseur + epsil_zero;
    double d = sqrt(pow(distance, 2) + pow(cible.cote/2.0, 2));
    // On crée les 8 points de la cible (sur la frontière du carré)
    for(int i = 0; i < 4; i++) {
        S2d point;
        point.x = cible.centre.x + d * cos(i * M_PI/2.0);
        point.y = cible.centre.y + d * sin(i * M_PI/2.0);
        points.push_back(point);
    }
    S2d point_choisi;
    double distance_minimale = 15 * dmax;
    for(auto& point : points) {
        // Calcul du temps pour atteindre le point
        S2d vecteur_distance = point - N->get_forme().centre;
        double distance_temps = vecteur_distance.norme()*vtran_max;
        double angle_direction(atan2(vecteur_distance.y, vecteur_distance.x));
        double delta_angle(angle_direction - N->get_angle());
        normalise_delta(delta_angle);
        distance_temps += delta_angle / vrot_max;
        if(distance_temps < distance_minimale) {
            distance_minimale = distance_temps;
            point_choisi = point;
        }
    }
    return point_choisi;
}

double normalise_delta(double& delta_angle){
    if(delta_angle > M_PI) {
        delta_angle -= 2*M_PI;
    } else if(delta_angle < -M_PI){
        delta_angle += 2*M_PI;
    }
    return delta_angle;
}

double choix_vrot(double& delta_angle){
    double vrot(vrot_max);
    if(abs(delta_angle) < M_PI/12){
        vrot = vrot_max/2;
    }
    return vrot;
}

void Spatial::set_update(int update) {
    nbUpdate_ = update;
}

void Spatial::update(vector<Particule> &particules,
                     vector<Neutraliseur> &neutraliseurs,
                     vector<Reparateur> &reparateurs) {
    if(fmod(nbUpdate_, modulo_update) == 0 or nbUpdate_ == 0) {
        vector<Neutraliseur> neutraliseurs_detresse;
        neutraliseurs_detresse = creer_neutraliseurs_detresse(reparateurs,
                                                             neutraliseurs);
        bool spawn_N(false);
        bool spawn_R(false);
        choix_R_ou_N(this, spawn_R, spawn_N, particules.size(),
                     neutraliseurs_detresse.size());
        creation_reparateur(this, neutraliseurs_detresse, spawn_N, spawn_R,
                            reparateurs);
        // Création nouveau neutraliseur
        if(spawn_N and nbNs_ < 3){
            vector<Particule> particules_libres; //celles qui sont pas ciblés
            for(const auto & particule : particules){
                if(!particule.is_target()){
                    particules_libres.push_back(particule);
                }
            }
            //trouver particule libre plus proche
            Particule P_proche(particules_libres[0]);
            double dist_min((forme_.centre-P_proche.get_forme().centre).norme());
            for(auto &P : particules_libres){
                double dist((forme_.centre-P.get_forme().centre).norme());
                if(dist < dist_min){
                    P_proche = P;
                }
            }
            //création nouveau neutraliseur, avec bonnes paramètres
            double angle(atan2(P_proche.get_forme().centre.y,
                               P_proche.get_forme().centre.x));
            int c_n((nbNs_+nbNd_)%3);
            Neutraliseur new_N(forme_.centre, angle, c_n, false, 0, nbUpdate_);
            //Check superposition avec un neutraliseur
            bool collision(false);
            for(auto &N: neutraliseurs){
                if(superposition(new_N.get_forme(), N.get_forme())){
                    collision = true;
                }
            }
            if(!collision){
                //TODO: mettre cible
                new_N.set_job(true);
                neutraliseurs.push_back(new_N);
                --nbNr_;
                ++nbNs_;
            }
            neutraliseurs.push_back(new_N);
        }
    }
}

void Spatial::assigner_N(std::vector<Neutraliseur>& neutraliseurs,
                         std::vector<Particule>& particules) const {
    for(auto& particule : particules) {
        while(!particule.is_target()) {
            double distance_minimale(5*dmax);
            int id_n(-1);
            for(int n=0; n<neutraliseurs.size(); n++) {
                if(neutraliseurs[n].has_job()) { continue; }
                S2d vecteur_distance = neutraliseurs[n].get_forme().centre
                                       - particule.get_forme().centre;
                if(vecteur_distance.norme() < distance_minimale) {
                    id_n = n;
                    distance_minimale = vecteur_distance.norme();
                }
            }
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
            if(id_n == -1) {
                particule.set_target(true);
            }
        }
    }
    // Réinitialisation des jobs
    for(auto& neutraliseur : neutraliseurs) { neutraliseur.set_job(false); }
    for(auto& particule : particules) { particule.set_target(false); }
}

void Spatial::assigner_R(std::vector<Reparateur>& reparateurs,
                         std::vector<Neutraliseur>& neutraliseurs) const {
    for(const auto& neutraliseur : neutraliseurs) {
        double distance_minimale(5*dmax);
        int id_r(-1);
        if(neutraliseur.get_panne()) {
            for(int r = 0; r < reparateurs.size(); r++) {
                if (reparateurs[r].has_job()) {
                    continue;
                }
                S2d vecteur_distance = neutraliseur.get_forme().centre
                                       - reparateurs[r].get_forme().centre;
                if (vecteur_distance.norme() < distance_minimale
                    and vecteur_distance.norme() < (max_update -
                    (nbUpdate_ - neutraliseur.get_k_update_panne())) * vtran_max) {
                    id_r = r;
                    distance_minimale = vecteur_distance.norme();
                }
            }
        }
        if(id_r != -1) {
            reparateurs[id_r].set_but(neutraliseur.get_forme().centre);
            reparateurs[id_r].set_job(true);
        }
    }
    for(auto& reparateur : reparateurs) {
        reparateur.set_job(false);
    }
}

std::vector<Neutraliseur> creer_neutraliseurs_detresse(
        std::vector<Reparateur> &reparateurs,
        std::vector<Neutraliseur> &neutraliseurs) {
    vector<Neutraliseur> neutraliseurs_detresse;
    for(auto& neutraliseur : neutraliseurs) {
        if(neutraliseur.get_panne()) {
            bool est_attribue = false;
            for(auto& reparateur : reparateurs) {
                if(reparateur.get_but() == neutraliseur.get_forme().centre) {
                    est_attribue = true;
                }
            }
            if(!est_attribue) {
                neutraliseurs_detresse.push_back(neutraliseur);
            }
        }
    }
    return neutraliseurs_detresse;
}

void choix_R_ou_N(Spatial *spatial, bool& spawn_R, bool& spawn_N,
                  int& nbP, int& nbN_detresse){
    int N_en_manque(nbP-spatial->get_nbNs());
    int R_en_manque(nbN_detresse);
    if(N_en_manque > spatial->get_nbNr() and N_en_manque > 0) {
        N_en_manque = spatial->get_nbNr();
    }
    if(R_en_manque > spatial->get_nbRr() and R_en_manque > 0) {
        R_en_manque = spatial->get_nbRr();
    }
    //Priorité va sur le réparateur, sinon sur le neutraliseur
    R_en_manque > 0 ? spawn_R = true : spawn_N = true;
}

void creation_reparateur(Spatial *spatial,
                         vector<Neutraliseur> &neutraliseurs_detresse,
                         bool &spawn_N, bool &spawn_R,
                         vector<Reparateur> &reparateurs) {
    // Création nouveau réparateur
    if(spawn_R) {
        if(neutraliseurs_detresse.size() > 0) {
            Neutraliseur N_proche(neutraliseurs_detresse[0]);
            double dist_min((spatial->get_forme().centre -
                            N_proche.get_forme().centre).norme());
            for (auto &neutraliseur: neutraliseurs_detresse) {
                //trouver celui le plus proche
                S2d direction(spatial->get_forme().centre -
                            neutraliseur.get_forme().centre);
                double dist(direction.norme());
                if (dist < dist_min) {
                    dist_min = dist;
                    N_proche = neutraliseur;
                }
            }
            //distance jusqu'au robot
            double dist_robot((max_update - (N_proche.get_nbUpdate()-
                                             N_proche.get_k_update_panne()))*vtran_max);
            if(dist_min < dist_robot){
                Reparateur R(spatial->get_forme().centre); //TODO: mettre contrôle superpostion
                reparateurs.push_back(R);
                spatial->set_nbRs(spatial->get_nbRs()+1);
                spatial->set_nbRr(spatial->get_nbRr()-1);
            } else { //si réparateur trop loin, on crée un neutraliseur
                spawn_N = true;
            }
        } else {
            spawn_N = true;
        }
    }
}