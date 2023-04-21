/**
 Gui.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Gui.h"
#include <vector>
#include "../Graphic/GraphicGui.h"
#include <iostream>
#include "../Simulation/Simulation.h"
using namespace std;

Drawing::Drawing() {
    set_content_width(200);
	set_content_height(200);
    set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    graphic_set_context(cr);
    graphic_draw_shape(width, height);
};

Window::Window() : drawingArea_() {
	set_default_size(820, 500);
	set_title("Mission Propre En Ordre");

    Gtk::Box fenetre(Gtk::Orientation::HORIZONTAL, 0);
    Gtk::Box menu(Gtk::Orientation::VERTICAL, 0);
    Gtk::Separator separator1;
    Gtk::Separator separator2;

    Gtk::Box box(Gtk::Orientation::VERTICAL, 0);
    exit_button_.set_margin(2);
    open_button_.set_margin(2);
    save_button_.set_margin(2);
    start_button_.set_margin(2);
    step_button_.set_margin(2);
    box.append(exit_button_);
    box.append(open_button_);
    box.append(save_button_);
    box.append(start_button_);
    box.append(step_button_);
    exit_button_.signal_clicked().connect(sigc::mem_fun(*this, &Window::exit_button_clicked));
    open_button_.signal_clicked().connect(sigc::mem_fun(*this, &Window::open_button_clicked));
    save_button_.signal_clicked().connect(sigc::mem_fun(*this, &Window::save_button_clicked));
    start_button_.signal_clicked().connect(sigc::mem_fun(*this, &Window::start_button_clicked));
    step_button_.signal_clicked().connect(sigc::mem_fun(*this, &Window::step_button_clicked));

    menu.append(box_actions_);
    menu.append(separator1);
    menu.append(label);
    fenetre.append(menu);
    fenetre.append(separator2);
    fenetre.append(drawingArea_);
    set_child(fenetre);

    /*for(auto& item : namedObjects) {
        Gtk::Box box_ligne = Gtk::Box(Gtk::Orientation::HORIZONTAL, 50);
        Gtk::Label label = Gtk::Label(item.first);
        label.set_halign(Gtk::Align::START);
        label.set_hexpand(true);
        Gtk::Label valeur = Gtk::Label(to_string(item.second));
        valeur.set_halign(Gtk::Align::END);
        box_ligne.append(label);
        box_ligne.append(valeur);
        box_actions.append(box_ligne);
    }*/
}

void Window::actualiser_stats(int maj, int pa, int rs, int rr, int ns, int np, int nd, int nr) {
    label_maj_.set_label(to_string(maj));
    label_pa_.set_label(to_string(pa));
    label_rs_.set_label(to_string(rs));
    label_rr_.set_label(to_string(rr));
    label_ns_.set_label(to_string(ns));
    label_np_.set_label(to_string(np));
    label_nd_.set_label(to_string(nd));
    label_nr_.set_label(to_string(nr));
}

void Window::fichier_selectionne(int reponse, Gtk::FileChooserDialog* dialogue) {
    if(reponse == Gtk::ResponseType::OK) {
        auto fichier = dialogue->get_file()->get_path();
        dialogue->hide();
        //Simulation simulation(0);
        //simulation.lecture(fichier);
        //TODO : Simulation par fichier
    } else {
        dialogue->hide();
    }
    delete dialogue;
}

void Window::exit_button_clicked() {
    exit(0);
}

void Window::open_button_clicked() {
    auto dialogue = new Gtk::FileChooserDialog("Sélectionner un fichier",
                                               Gtk::FileChooser::Action::OPEN);
    dialogue->set_modal(true);
    dialogue->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &Window::fichier_selectionne), dialogue));
    dialogue->add_button("Annuler", Gtk::ResponseType::CANCEL);
    dialogue->add_button("Ouvrir", Gtk::ResponseType::OK);
    dialogue->show();
}

void Window::save_button_clicked() {
    // TODO : sauvegarder
}

void Window::start_button_clicked() {
    // TODO : sauvegarder
}

void Window::step_button_clicked() {
    // TODO : sauvegarder
}