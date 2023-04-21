/**
 Graphic.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "GraphicGui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
    ptcr = &cr;
}

void graphic_draw_shape(const int width, const int height) {

}