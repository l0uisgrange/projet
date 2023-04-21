/**
 Gui.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filechooser.h>

#ifndef GUI_H
#define GUI_H

struct Frame {
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double asp;
	int width;
	int height;
};

class Drawing : public Gtk::DrawingArea {
public:
    Drawing();
protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

class Window : public Gtk::Window {
public:
	Window();
protected:
    void exit_button_clicked();
    void open_button_clicked();
    void fichier_selectionne(int reponse, Gtk::FileChooserDialog* dialogue);
    // Boîtes
    Gtk::Box box;
    Gtk::Box box_actions;
    // Labels
    Gtk::Label label_maj;
    Gtk::Label label_particules;
    Gtk::Label label_rs;
    Gtk::Label label_rr;
    Gtk::Label label_ns;
    Gtk::Label label_np;
    Gtk::Label label_nd;
    Gtk::Label label_nr;
    // Boutons
    Gtk::Button exit_button;
    Gtk::Button open_button;
    Gtk::Button save_button;
    Gtk::Button start_button;
    Gtk::Button stop_button;
    // Séparateurs
    Gtk::Separator separator;
    Gtk::Separator separator2;
    // Zone de dessin
    Drawing drawingArea_;
};

#endif
