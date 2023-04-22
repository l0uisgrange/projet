/**
 Graphic.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <gtkmm.h>

struct rgb {
    rgb(double r, double g, double b);
    double r,g,b;
};

enum Couleurs { BLANC, GRIS, ROUGE, BLEU_CLAIR, NOIR, VIOLET, ORANGE, VERT };

void graphic_draw_shape(int width, int height);
void draw_cercle(double x, double y, double rayon, Couleurs couleur);
void draw_carre(double x, double y, double cote, Couleurs couleur);
rgb decode_couleur(Couleurs couleur);
#endif
