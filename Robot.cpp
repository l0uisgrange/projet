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

void Reparateur::move(Cercle cible) {
    S2d direction = cible.centre - forme_.centre;
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

void Neutraliseur::turn(Carre cible) {
    S2d direction;
    if((coordination_ == 0) or (coordination_ == 2)) {
        direction = cible.centre - forme_.centre;
    } else {
        direction = direction_type1(this, cible) -forme_.centre;
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
    if(coordination_ == 0 or coordination_ == 2) {
        direction = cible.centre - forme_.centre;
    } else {
        direction = direction_type1(this, cible) - forme_.centre;
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
            if(abs(delta_angle) < epsil_alignement) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
        }
        case 1:
            if(abs(delta_angle) < epsil_alignement) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
        case 2:
            if(abs(delta_angle) < M_PI/3) {
                forme_.centre.x += vect_angle.x * vtran_ * delta_t;
                forme_.centre.y += vect_angle.y * vtran_ * delta_t;
            }
            break;
    }
}

S2d direction_type1(Neutraliseur* N, Carre cible) {
    vector<S2d> points;
    double angle_decallage = atan((cible.cote*risk_factor/2.0)/(cible.cote/2.0));
    """donne moi le point le plus proche du neutraliseur, situé sur le carre de centre cible.centre et de coté cible.cote*risk_factor. ce point doit etre sur une des 8 droites qui forment le carre de cible"""
    for(int i = 0; i < 4; i++) {
        S2d point;
        point.x = cible.centre.x + cible.cote*risk_factor/2.0 * cos(i * M_PI/2.0 + angle_decallage);
        point.y = cible.centre.y + cible.cote*risk_factor/2.0 * sin(i * M_PI/2.0 + angle_decallage);
        points.push_back(point);
    }
    for(int i = 0; i < 4; i++) {
        S2d point;
        point.x = cible.centre.x + cible.cote*risk_factor/2.0 * cos(i * M_PI/2.0 + M_PI-angle_decallage);
        point.y = cible.centre.y + cible.cote*risk_factor/2.0 * sin(i * M_PI/2.0 + M_PI-angle_decallage);
        points.push_back(point);
    }
    S2d point_choisi;
    double distance_minimale = 5 * dmax;
    for(auto& point : points) {
        S2d vecteur_distance = point - N->get_forme().centre;
        double distance = vecteur_distance.norme();
        if(distance < distance_minimale) {
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

int Spatial::assigner_cible(const vector<Neutraliseur>& neutraliseurs, const Particule& particule) {
    return 0;
}