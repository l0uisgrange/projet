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

struct Frame // Pour adapter afichage
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double asp; // aspect ratio
    int height;
    int width;
};

enum Couleurs { BLANC, GRIS, ROUGE, BLEU_CLAIR, NOIR, VIOLET, ORANGE, VERT };

void draw_cercle(double x, double y, double rayon, Couleurs couleur);
void draw_carre(double x, double y, double cote, Couleurs couleur);
rgb decode_couleur(Couleurs couleur);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
static void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame);
static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                    const Frame& frame);
#endif
