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

void draw_cercle(double x, double y, double rayon, double r, double g, double b){
    (*ptcr)->set_source_rgb(r,g,b);
    (*ptcr)->set_line_width(2.0); //combien mettre ??
    (*ptcr)->arc(x, y, rayon, 0.0, 2*M_PI);
    (*ptcr)->stroke();
}

void draw_carre(double x, double y, double cote, double r, double g, double b){
    (*ptcr)->set_source_rgb(r,g,b);
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->rectangle(x, y, cote, cote);
}
