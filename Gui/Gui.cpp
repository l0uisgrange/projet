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

static Frame default_frame = {-150., 150., -100., 100., 1.5, 300, 200};

Drawing::Drawing() {
    set_content_width(default_frame.width);
	set_content_height(default_frame.height);
    set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    graphic_set_context(cr);
    graphic_draw_shape(width, height);
};

Window::Window() : box(Gtk::Orientation::HORIZONTAL, 0),
                   box_actions(Gtk::Orientation::VERTICAL, 10),
                   label_maj("Mises à jour"), label_particules("Particules"),
                   label_rs("Robots réparateurs en service"),
                   label_rr("Robots réparateurs en réserve"),
                   label_ns("Robots neutraliseurs en service"),
                   label_np("Robots neutraliseurs en panne"),
                   label_nd("Robots neutraliseurs détruits"),
                   label_nr("Robots neutraliseurs en réserve"),
                   exit_button("exit"), open_button("open"),
                   save_button("save"), start_button("start"),
                   stop_button("stop"), drawingArea_() {
	set_default_size(820, 500);
	set_title("Mission Propre En Ordre");
    // Marges
    exit_button.set_margin(2);
    open_button.set_margin(2);
    save_button.set_margin(2);
    start_button.set_margin(2);
    stop_button.set_margin(2);
    box_actions.set_margin(10);
    drawingArea_.set_margin(10);
    // Connexion aux actions
    exit_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::exit_button_clicked));
    open_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::open_button_clicked));
    // Ajout à la boîte
    box_actions.append(exit_button);
    box_actions.append(open_button);
    box_actions.append(save_button);
    box_actions.append(start_button);
    box_actions.append(stop_button);
    box_actions.append(separator);
    std::vector<std::pair<std::string, int>> namedObjects = {
            {"Mises à jour",                    2},
            {"Particules",                      3},
            {"Robots réparateurs en service",   10},
            {"Robots réparateurs en réserve",   10},
            {"Robots neutraliseurs en service", 10},
            {"Robots neutraliseurs en panne",   10},
            {"Robots neutraliseurs détruits",   10},
            {"Robots neutraliseurs en réserve", 10}
    };
    for(auto& item : namedObjects) {
        Gtk::Box box_ligne = Gtk::Box(Gtk::Orientation::HORIZONTAL, 50);
        Gtk::Label label = Gtk::Label(item.first);
        label.set_halign(Gtk::Align::START);
        label.set_hexpand(true);
        Gtk::Label valeur = Gtk::Label(to_string(item.second));
        valeur.set_halign(Gtk::Align::END);
        box_ligne.append(label);
        box_ligne.append(valeur);
        box_actions.append(box_ligne);
    }
    box_actions.set_halign(Gtk::Align::START);
    box.append(box_actions);
    box.append(separator2);
    box.append(drawingArea_);

    set_child(box);
}

void Window::exit_button_clicked() {
    exit(0);
}

void Window::fichier_selectionne(int reponse, Gtk::FileChooserDialog* dialogue) {
    if(reponse == Gtk::ResponseType::OK) {
        auto fichier = dialogue->get_file()->get_path();
        dialogue->hide();
        Simulation simulation(0);
        simulation.lecture(fichier);
    } else {
        dialogue->hide();
    }
    delete dialogue;
}

void Window::open_button_clicked() {
    auto dialogue = new Gtk::FileChooserDialog("Please choose a file",
                                               Gtk::FileChooser::Action::OPEN);
    dialogue->set_transient_for(*this);
    dialogue->set_modal(true);
    dialogue->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &Window::fichier_selectionne), dialogue));
    dialogue->add_button("Annuler", Gtk::ResponseType::CANCEL);
    dialogue->add_button("Ouvrir", Gtk::ResponseType::OK);
    dialogue->show();
}
