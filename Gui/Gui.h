/**
 Gui.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <gtkmm.h>

#ifndef FENETRE_H
#define FENETRE_H

class Window : public Gtk::Window {
public:
	Window();
protected:
    // Boxes
    Gtk::Box box_1;
    // Buttons
    Gtk::Button exit_button;
    Gtk::Button open_button;
    Gtk::Button save_button;
    Gtk::Button start_button;
    Gtk::Button stop_button;
};

#endif
