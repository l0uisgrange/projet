/**
 Graphic.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "GraphicGui.h"
#include <cmath>

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
    ptcr = &cr;
}

void graphic_draw_shape(const int width, const int height) {

}

void draw_cercle(double x, double y, double rayon, Couleur couleur){
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->arc(x, y, rayon, 0.0, 2*M_PI);
    (*ptcr)->stroke();
}

void draw_carre(double x, double y, double cote, Couleur couleur){
    rgb color(decode_couleur(couleur));
    (*ptcr)->set_source_rgb(color.r, color.g, color.b);
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->rectangle(x, y, cote, cote);
    (*ptcr)->stroke();
}

rgb decode_couleur(Couleur couleur){
    switch (couleur) {
        case BLANC:
            return {0,0,0};
        case GRIS:
            return {0.5, 0.5, 0.5};
        case ROUGE:
            return {1, 0, 0};
        case BLEU_CLAIR:
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

rgb::rgb(double r, double g, double b):r(r), g(g), b(b){};
