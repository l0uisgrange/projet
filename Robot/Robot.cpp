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
    cout << "Spatial créé !" << endl;
}

void Spatial::draw() const {
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, BLEU_CLAIR);
}

Neutraliseur::Neutraliseur(S2d position, double angle, int coordination, bool panne,
                           int k_update_panne, int nbUpdate)
    : angle_(angle), panne_(panne), coordination_(coordination) {
    if(k_update_panne > nbUpdate) {
        cout << message::invalid_k_update(position.x, position.y, k_update_panne,
                                          nbUpdate);
        exit(EXIT_FAILURE);
    }
    forme_ = Cercle(position, r_neutraliseur);
    cout << "Neutraliseur créé !" << endl;
}

void Neutraliseur::draw() const {
    if(panne_) {
        draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, ORANGE);
    } else {
        draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, NOIR);
    }
    //TODO adapter pour la colision
}

Reparateur::Reparateur(S2d position)
    : forme_(Cercle(position, r_reparateur)) {
    cout << "Réparateur créé !" << endl;
}

void Reparateur::draw() const {
    draw_cercle(forme_.centre.x, forme_.centre.y, forme_.rayon, NOIR);
}


