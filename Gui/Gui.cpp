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

Window::Window() : label_maj_("0"), label_pa_("0"),
                   label_rs_("0"), label_rr_("0"),
                   label_ns_("0"), label_np_("0"),
                   label_nd_("0"), label_nr_("0"),
                   drawingArea_() {
	set_default_size(820, 500);
	set_title("Mission Propre En Ordre");
    Gtk::Box fenetre(Gtk::Orientation::HORIZONTAL, 0);
    Gtk::Box menu(Gtk::Orientation::VERTICAL, 0);
    Gtk::Box texte(Gtk::Orientation::HORIZONTAL, 0);
    Gtk::Box labels(Gtk::Orientation::VERTICAL, 0);
    Gtk::Box stats(Gtk::Orientation::VERTICAL, 0);
    Gtk::Separator separator1;
    Gtk::Separator separator2;
    Gtk::Box box(Gtk::Orientation::VERTICAL, 0);

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

    Gtk::Label label_maj("Mises à jour");
    Gtk::Label label_pa("Particules");
    Gtk::Label label_rs("Robots réparateurs en service");
    Gtk::Label label_rr("Robots réparateurs en réserve");
    Gtk::Label label_ns("Robots neutraliseurs en service");
    Gtk::Label label_np("Robots neutraliseurs en panne");
    Gtk::Label label_nd("Robots neutraliseurs détruits");
    Gtk::Label label_nr("Robots neutraliseurs en réserve");
    labels.append(label_maj);
    labels.append(label_pa);
    labels.append(label_rs);
    labels.append(label_rr);
    labels.append(label_ns);
    labels.append(label_np);
    labels.append(label_nd);
    labels.append(label_nd);
    stats.append(label_maj);
    stats.append(label_pa);
    stats.append(label_rs);
    stats.append(label_rr);
    stats.append(label_ns);
    stats.append(label_np);
    stats.append(label_nd);
    stats.append(label_nd);

    texte.append(labels);
    texte.append(stats);
    menu.append(box);
    menu.append(separator1);
    menu.append(texte);
    fenetre.append(menu);
    fenetre.append(separator2);
    fenetre.append(drawingArea_);
    set_child(fenetre);
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
        Simulation simulation(0);
        simulation.lecture(fichier);
        actualiser_stats(simulation.get_spatial().get_update(),
                         simulation.get_nbP(),
                         simulation.get_spatial().get_update(),
                         simulation.get_spatial().get_nbRr(),
                         simulation.get_spatial().get_nbNs(),
                         simulation.get_spatial().get_nbNd(), // TODO changer pour np
                         simulation.get_spatial().get_nbNd(),
                         simulation.get_spatial().get_nbNr());
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