/**
 Graphic.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <cmath>
#include "Graphic.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
    ptcr = &cr;
}

void draw_cercle(double x, double y, double rayon, Couleurs couleur) {
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.5);
    (*ptcr)->arc(x, y, rayon, 0.0, 2*M_PI);
    (*ptcr)->stroke();
}

void fill_cercle(double x, double y, double rayon, Couleurs couleur) {
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.5);
    (*ptcr)->arc(x, y, rayon, 0.0, 2*M_PI);
    (*ptcr)->fill();
}

void draw_carre(double x, double y, double cote, Couleurs couleur) {
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.5);
    (*ptcr)->rectangle(x, y, cote, cote);
    (*ptcr)->stroke();
}

void fill_carre(double x, double y, double cote, Couleurs couleur) {
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.5);
    (*ptcr)->rectangle(x, y, cote, cote);
    (*ptcr)->fill();
}

rgb decode_couleur(Couleurs couleur) {
    switch(couleur) {
        case BLANC:
            return {1,1,1};
        case GRIS:
            return {0.808, 0.808, 0.808};
        case ROUGE:
            return {1, 0, 0};
        case BLEU_CLAIR:
            return {0.3137, 0.686, 0.918};
        case BLEU_FONCE:
            return {0, 1, 1};
        case NOIR:
            return {0, 0, 0};
        case VIOLET:
            return {0.5, 0, 0.5};
        case ORANGE:
            return {1, 0.65, 0};
        case VERT:
            return {0, 1, 0};
    }
}

rgb::rgb(double r, double g, double b) : r(r), g(g), b(b) {};

void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                    const Frame& frame) {
    // déplace l'origine au centre du carré le plus grand possible
    double centre_carre(std::min(frame.width, frame.height)/2.0);
    cr->translate(centre_carre, centre_carre);

    // normalise la largeur et hauteur selon default frame
    // ET inverse la direction de l'axe Y
    double axe_min_diff(0.); //donne xMax-xMin ou yMax-yMin
    double axe_min_sum(0.); // xMin+xMax ou yMin+yMin
    if (frame.xMax > frame.yMax) {
        axe_min_diff = frame.yMax - frame.yMin;
        axe_min_sum =frame.yMin + frame.yMax;
    }else{
        axe_min_diff = frame.xMax - frame.xMin;
        axe_min_sum = frame.xMin + frame.xMax;
    }
    cr->scale((double)frame.width/axe_min_diff,
              (double)-frame.height/axe_min_diff);
    // décalage vers centre du cadrage
    cr->translate(-(axe_min_sum)/2, -(axe_min_sum)/2);
}

void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame) {
    cr->set_line_width(2.0);
    cr->set_source_rgb(1, 1, 1);
    cr->rectangle(0,0, frame.width, frame.height);
    cr->fill();
    cr->rectangle(0,0, frame.width, frame.height);
    cr->set_source_rgb(0.847, 0.8314, 0.8157);
    cr->stroke();
}