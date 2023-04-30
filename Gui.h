/**
 Gui.h
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Graphic.h"
#include "Simulation.h"
#include <memory>

#ifndef GUI_H
#define GUI_H

class Drawing : public Gtk::DrawingArea {
public:
    explicit Drawing(Simulation &sim);
    virtual ~Drawing(){};
    void set_frame(Frame f);
    void adjust_frame(int width, int height);
    void set_sim(Simulation& sim);
    Simulation& get_sim() { return sim_; };
private:
    Frame frame_;
    Simulation sim_;
protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

class Window : public Gtk::Window {
public:
	explicit Window(Simulation &sim);
    Gtk::Box append_boutons();
    Gtk::Box append_labels();
    void actualiser_stats();
    void exit_button_clicked();
    void open_button_clicked();
    void save_button_clicked();
    void start_button_clicked();
    void step_button_clicked();
    void fichier_selectionne(int reponse, Gtk::FileChooserDialog* dialogue);
    bool touche_clavier(guint keyval, guint keycode, Gdk::ModifierType state);
    bool on_timeout();
private:
    Gtk::Button exit_button_;
    Gtk::Button open_button_;
    Gtk::Button save_button_;
    Gtk::Button start_button_;
    Gtk::Button step_button_;
    Gtk::Label label_maj_;
    Gtk::Label label_pa_;
    Gtk::Label label_rs_;
    Gtk::Label label_rr_;
    Gtk::Label label_ns_;
    Gtk::Label label_np_;
    Gtk::Label label_nd_;
    Gtk::Label label_nr_;
    Drawing drawingArea_;
    bool minuteur_;
};

#endif
