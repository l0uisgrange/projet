/**
 Projet.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

#include "Simulation/Simulation.h"
#include <iostream>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
using namespace std;

class ExampleWindow : public Gtk::Window
{
public:
	ExampleWindow();
};

ExampleWindow::ExampleWindow()
{
	set_default_size(820, 350);
	set_title("EPFL Logo");
}


int main(int argc, char *argv[]) {
    S2d position;
    Spatial spatial(position, 0, 0, 0, 0, 0, 0);
    Simulation simulation(0, spatial);
    if(argc < 1) {
        exit(0);
    }
    simulation.lecture(argv[1]);
    auto app = Gtk::Application::create();
	return app->make_window_and_run<ExampleWindow>(1, argv);
}