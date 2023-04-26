/**
 Projet.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Simulation/Simulation.h"
#include "Gui/Gui.h"
using namespace std;

int main(int argc, char *argv[]) {
    Simulation simulation(0);
    if(argc > 1) {
        simulation.lecture(argv[1]);
    }
    auto app = Gtk::Application::create();
	return app->make_window_and_run<Window>(1, argv, simulation);
}