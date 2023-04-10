/**
 Gui.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Gui.h"
#include <iostream>
using namespace std;

Window::Window() : box_1(Gtk::Orientation::VERTICAL, 10),
                   exit_button("exit"), open_button("open"),
                   save_button("save"), start_button("start"),
                   stop_button("stop") {
	set_default_size(820, 350);
	set_title("Propre-en-ordre");
    // Marges
    exit_button.set_margin(2);
    open_button.set_margin(2);
    save_button.set_margin(2);
    start_button.set_margin(2);
    stop_button.set_margin(2);
    box_1.set_margin(10);
    // Ajout à la boîte
    box_1.append(exit_button);
    box_1.append(open_button);
    box_1.append(save_button);
    box_1.append(start_button);
    box_1.append(stop_button);

    set_child(box_1);
}