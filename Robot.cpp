/**
 Robot.cpp
 Louis Grange (40%) et Daniel Ataide (60%)
 Version 2.4
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
    : angle_(angle), panne_(panne), collision_(false), collisionRN_(false),
    coordination_(coordination), k_update_panne_(k_update_panne),
    nbUpdate_(nbUpdate), job_(false) {
    forme_.centre.x = position.x;
    forme_.centre.y = position.y;
    forme_.rayon = r_neutraliseur;
}

void Neutraliseur::draw() const {
    Couleurs couleur(NOIR);
    if(collision_ or collisionRN_) {
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
    : forme_(Cercle(position, r_reparateur)), job_(false) {}

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
    if(coordination_ == 1 and !collision_) {
        S2d point = direction_type1(this, but_);
        if(dans_zone(but_.centre, point, forme_.centre)) {
            direction = but_.centre - forme_.centre;
        } else {
            direction = point - forme_.centre;
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

bool dans_zone(S2d but, S2d point, S2d position) {
    // But : centre de particule
    // Point : point en dehors de la zone
    // Position : position du neutraliseur
    if(abs(but.x - point.x) < epsil_zero) {
        if(abs(point.x - position.x) < vtran_max*delta_t) {
            return true;
        }
    } else if(abs(but.y - point.y) < epsil_zero) {
        if(abs(point.y - position.y) < vtran_max*delta_t) {
            return true;
        }
    }
    return false;
}

void Neutraliseur::move() {
    S2d direction;
    if(coordination_ == 1) {
        S2d point = direction_type1(this, but_);
        if(dans_zone(but_.centre, point, forme_.centre)) {
            direction = but_.centre - forme_.centre;
        } else {
            direction = point - forme_.centre;
        }
    } else {
        direction = but_.centre - forme_.centre;
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
                vtran_ = min(double(0.5*direction.norme()),
                             double(vtran_max));
                if(abs(delta_angle) > M_PI/6) {
                    vtran_ /= 2;
                }
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

double choix_vrot(double delta_angle){
    double vrot(vrot_max);
    if(abs(delta_angle) < M_PI/12){
        vrot = vrot_max/2;
    }
    return vrot;
}

void Spatial::set_update(int update) {
    nbUpdate_ = update;
}

void Spatial::update(V_particule& particules, V_neutraliseur &neutraliseurs,
                     V_reparateur &reparateurs) {
    if(fmod(nbUpdate_, modulo_update) == 0 and nbUpdate_ != 0) {
        vector<Neutraliseur> neutraliseurs_detresse;
        neutraliseurs_detresse = creer_neutraliseurs_detresse(reparateurs,
                                                             neutraliseurs);
        bool spawn_N(false);
        bool spawn_R(false);
        int R_en_manque(min(int(neutraliseurs_detresse.size()), nbRr_));
        // Priorité va sur le réparateur, sinon sur le neutraliseur
        R_en_manque > 0 ? spawn_R = true : spawn_N = true;
        if(nbRr_ > 0 and spawn_R) {
            creation_reparateur(this, spawn_N, spawn_R, neutraliseurs_detresse,
                                neutraliseurs, reparateurs);
        } else if(nbNr_ > 0) {
            creation_neutraliseur(this, neutraliseurs, particules,
                                  reparateurs, spawn_N);
        }
    }
}

double distance_min(Neutraliseur n, Particule p) {
    S2d vecteur_distance = n.get_forme().centre
                                       - p.get_forme().centre;
    double distance_temps = vecteur_distance.norme()*vtran_max;
    double angle_direction(atan2(vecteur_distance.y, vecteur_distance.x));
    double delta_angle(angle_direction - n.get_angle());
    normalise_delta(delta_angle);
    distance_temps += delta_angle / vrot_max;
    return distance_temps;
}

void Spatial::assigner_N(std::vector<Neutraliseur>& neu,
                         std::vector<Particule>& part) const {
    for(int p=0; p < int(part.size()); p++) {
        bool trouvee(false);
        if(part[p].is_target()) { continue; }
        while(!trouvee) {
            double distance_minimale(10*dmax);
            int id_n(-1), id_p(-1);
            // Recherche du neutraliseur le plus proche
            for(int n=0; n < int(neu.size()); n++) {
                if(neu[n].has_job()) { continue; }
                if(distance_min(neu[n], part[p]) < distance_minimale) {
                    id_n = n;
                    distance_minimale = distance_min(neu[n], part[p]);
                }
            }
            // Si aucun résultat (robots tous occupés par exemple)
            if(id_n < 0) { trouvee = true; continue; }
            distance_minimale = 10*dmax;
            for(int a=0; a < int(part.size()); a++) {
                if(part[a].is_target() or
                   part[a].get_forme().cote < part[p].get_forme().cote) { continue; }
                if(distance_min(neu[id_n], part[a]) < distance_minimale) {
                    distance_minimale = distance_min(neu[id_n], part[a]);
                    id_p = a;
                }
            } // Vérification que la particule b est la même que la courante
            if(id_p == p) {
                neu[id_n].set_job(true);
                neu[id_n].set_but(part[p].get_forme());
                part[p].set_target(true);
                trouvee = true;
            } else if(id_p > -1) {
                neu[id_n].set_job(true);
                neu[id_n].set_but(part[id_p].get_forme());
                part[id_p].set_target(true);
            }
        }
    } // Réinitialisation des jobs
    for(auto& neutraliseur : neu) {
        if(!neutraliseur.has_job()) {
            Carre forme;
            forme.centre = forme_.centre;
            neutraliseur.set_but(forme);
            neutraliseur.set_job(false);
        }
    }
    for(auto& particule : part) { particule.set_target(false); }
}

void Spatial::assigner_R(std::vector<Reparateur>& reparateurs,
                         std::vector<Neutraliseur>& neutraliseurs) const {
    for(auto& neutraliseur : neutraliseurs) {
        double distance_minimale(5*dmax);
        int id_r(-1);
        if(neutraliseur.get_panne()) {
            for(int r = 0; r < int(reparateurs.size()); r++) {
                if(reparateurs[r].has_job()) {
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
}

V_neutraliseur creer_neutraliseurs_detresse(V_reparateur& reparateurs,
        V_neutraliseur &neutraliseurs) {
    V_neutraliseur neutraliseurs_detresse;
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

void creation_reparateur(Spatial *spatial, bool &spawn_N, bool &spawn_R,
                         V_neutraliseur &neutraliseurs_detresse,
                         V_neutraliseur& neutraliseurs, V_reparateur& reparateurs) {
    // Création nouveau réparateur
    if(spawn_R) {
        if(!neutraliseurs_detresse.empty()) {
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
                Reparateur R(spatial->get_forme().centre);
                if(!single_superposition_R_N(neutraliseurs,
                                            reparateurs, R.get_forme())) {
                    reparateurs.push_back(R);
                    spatial->set_nbRs(spatial->get_nbRs() + 1);
                    spatial->set_nbRr(spatial->get_nbRr() - 1);
                }
            } else { //si réparateur trop loin, on crée un neutraliseur
                spawn_N = true;
            }
        } else {
            spawn_N = true;
        }
    }
}

void creation_neutraliseur(Spatial *spatial, V_neutraliseur &neutraliseurs,
                           V_particule& particules, V_reparateur& reparateurs,
                           bool& spawn_N) {
    if(spawn_N and spatial->get_nbNs() < 3 and
       spatial->get_nbNs() < int(particules.size())) {
        if(particules.size() > 0) {
            Particule P_max(trouver_P(spatial, particules));
            double angle(atan2(P_max.get_forme().centre.y,
                               P_max.get_forme().centre.x));
            int c_n((spatial->get_nbNs() + spatial->get_nbNd()) % 3);
            Neutraliseur new_N(spatial->get_forme().centre, angle, c_n,
                               false, 0, spatial->get_update());
            if(!single_superposition_R_N(neutraliseurs, reparateurs, new_N.get_forme())) {
                new_N.set_job(true);
                neutraliseurs.push_back(new_N);
                spatial->set_nbNr(spatial->get_nbNr() - 1);
                spatial->set_nbNs(spatial->get_nbNs() + 1);
            }
        }
    }
}

bool single_superposition_R_N(V_neutraliseur& neutraliseurs,
                              V_reparateur& reparateurs, Cercle& new_N){
    //superposition avec un neutraliseur en service
    bool collision(false);
    for(auto &N: neutraliseurs){
        if(superposition(new_N, N.get_forme())){
            collision = true;
        }
    }
    //superposition avec un reparateur en service
    for(auto &R: reparateurs){
        if(superposition(new_N, R.get_forme())){
            collision = true;
        }
    }
    return collision;
}

Particule trouver_P(Spatial *spatial, V_particule& particules_libres){
    Particule P_max(particules_libres[0]); //trouver particule la plus proche
    double taille_max(P_max.get_forme().cote);
    for (auto &P: particules_libres) {
        double taille(P.get_forme().cote);
        if (taille > taille_max) {
            P_max = P;
        }
    }
    return P_max;
}