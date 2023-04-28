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
    if(abs(position.x) < dmax - r_spatial
       and abs(position.y) < dmax - r_spatial) {
        forme_ = Cercle(position, r_spatial);
    } else {
        cout << message::spatial_robot_ouside(position.x, position.y);
        exit(EXIT_FAILURE);
    }
}

void Spatial::draw() const {
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, BLEU_CLAIR);
    draw_cercle(forme_.centre.x, forme_.centre.y, 0.5, BLEU_CLAIR);
    fill_cercle(forme_.centre.x, forme_.centre.y, 0.5, BLEU_CLAIR);
}

string Spatial::get_info() const {
    string info;
    info= to_string(forme_.centre.x) +" "+ to_string(forme_.centre.y) + " "
            + to_string(nbUpdate_) +" "+ to_string(nbNr_) +" "+ to_string(nbNs_) +" "
            + to_string(nbRr_) +" "+ to_string(nbRs_);
    return info;
}

Neutraliseur::Neutraliseur(S2d position, double angle, int coordination, bool panne,
                           int k_update_panne, int nbUpdate)
    : angle_(angle), panne_(panne), coordination_(coordination),
    k_update_panne_(k_update_panne) {
    if(k_update_panne > nbUpdate) {
        cout << message::invalid_k_update(position.x, position.y, k_update_panne,
                                          nbUpdate);
        exit(EXIT_FAILURE);
    }
    forme_ = Cercle(position, r_neutraliseur);
}

void Neutraliseur::draw() const {
    Couleurs couleur(NOIR);
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
    string info;
    info = to_string(forme_.centre.x) +" "+ to_string(forme_.centre.y) +" "
            + to_string(angle_) +" "+ to_string(coordination_) +" "
            + to_string(panne_) +" "+ to_string(k_update_panne_);
    return info;
}

Reparateur::Reparateur(S2d position)
    : forme_(Cercle(position, r_reparateur)) {
}

void Reparateur::draw() const {
    fill_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, VERT);
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, NOIR);
}

void Spatial::set_update(int update) {
    nbUpdate_ = update;
}

