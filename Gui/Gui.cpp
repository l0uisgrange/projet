/**
 Gui.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Gui.h"
#include <vector>
#include <iostream>
using namespace std;

Drawing::Drawing() {
    set_content_width(400);
	set_content_height(400);
    set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->rectangle(0, 0, width, height);
    cr->fill();
};

Window::Window() : box(Gtk::Orientation::HORIZONTAL, 20),
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
	set_default_size(820, 400);
	set_title("Mission Propre En Ordre");
    // Marges
    exit_button.set_margin(2);
    open_button.set_margin(2);
    save_button.set_margin(2);
    start_button.set_margin(2);
    stop_button.set_margin(2);
    box_actions.set_margin(10);
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
        Gtk::Box box_ligne = Gtk::Box(Gtk::Orientation::HORIZONTAL, 40);
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
    box.append(drawingArea_);

    set_child(box);
}

