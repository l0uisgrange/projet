/**
 Graphic.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

struct rgb {
    rgb(double r, double g, double b);
    double r,g,b;
};

enum Couleur { BLANC, GRIS, ROUGE, BLEU_CLAIR, NOIR, VIOLET, ORANGE, VERT };

#ifndef GRAPHIC_H
#define GRAPHIC_H


void graphic_draw_shape(int width, int height);
void draw_cercle(double x, double y, double rayon, Couleur couleur);
void draw_carre(double x, double y, double cote, Couleur couleur);
rgb decode_couleur(Couleur couleur);


#endif
