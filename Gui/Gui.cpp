/**
 Gui.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

static constexpr unsigned taille_dessin(500);

#include "Gui.h"
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

static Frame default_frame = {-dmax, dmax,
                              -dmax, dmax,
                              1., taille_dessin, taille_dessin};

Drawing::Drawing(Simulation &sim) : sim_(sim) {
    set_frame(default_frame);
    set_content_width(taille_dessin);
	set_content_height(taille_dessin);
    set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (sim_.get_dessiner()) {
        adjust_frame(width, height);
        draw_frame(cr, frame_);
        orthographic_projection(cr,frame_);
        graphic_set_context(cr);
        sim_.draw_simulation();
    } // TODO faut rien afficher dans les labels!
};

void Drawing::set_frame(Frame f) {
    if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0)) {
        f.asp = (double)f.width/f.height;
        frame_ = f;
    } else {
        exit(1);
    }
}

void Drawing::adjust_frame(int width, int height) {
    frame_.width  = min(width, height);
    frame_.height = min(width,height);
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp) {
        frame_.yMax = default_frame.yMax ;
        frame_.yMin = default_frame.yMin ;
        double delta(default_frame.xMax - default_frame.xMin);
        double mid((default_frame.xMax + default_frame.xMin)/2);
        frame_.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
        frame_.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
    } else {
        frame_.xMax = default_frame.xMax ;
        frame_.xMin = default_frame.xMin ;
        double delta(default_frame.yMax - default_frame.yMin);
        double mid((default_frame.yMax + default_frame.yMin)/2);
        frame_.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
        frame_.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
    }
}

void Drawing::set_sim(Simulation &sim) {
    sim_ = sim;
}

Window::Window(Simulation &sim) : exit_button_("exit"), open_button_("open"),
                   save_button_("save"), start_button_("start"),
                   step_button_("step"), label_maj_("0"), label_pa_("0"),
                   label_rs_("0"), label_rr_("0"), label_ns_("0"), label_np_("0"),
                   label_nd_("0"), label_nr_("0"), drawingArea_(sim), minuteur_(false) {
	set_default_size(taille_dessin, taille_dessin);
	set_title("Mission Propre En Ordre");
    Gtk::Box fenetre(Gtk::Orientation::HORIZONTAL, 0);
    Gtk::Box menu(Gtk::Orientation::VERTICAL, 0);
    Gtk::Box texte(Gtk::Orientation::HORIZONTAL, 50);
    Gtk::Box labels(Gtk::Orientation::VERTICAL, 7);
    Gtk::Box stats(Gtk::Orientation::VERTICAL, 7);
    Gtk::Separator separator1;
    Gtk::Separator separator2;
    Gtk::Box boutons(Gtk::Orientation::VERTICAL, 5);
    boutons.set_margin(10);
    texte.set_margin(10);
    drawingArea_.set_margin(10);
    boutons.append(exit_button_);
    boutons.append(open_button_);
    boutons.append(save_button_);
    boutons.append(start_button_);
    boutons.append(step_button_);
    exit_button_.signal_clicked().connect(sigc::mem_fun(*this,
                                &Window::exit_button_clicked));
    open_button_.signal_clicked().connect(sigc::mem_fun(*this,
                                &Window::open_button_clicked));
    save_button_.signal_clicked().connect(sigc::mem_fun(*this,
                                &Window::save_button_clicked));
    start_button_.signal_clicked().connect(sigc::mem_fun(*this,
                                &Window::start_button_clicked));
    step_button_.signal_clicked().connect(sigc::mem_fun(*this,
                                &Window::step_button_clicked));
    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
            sigc::mem_fun(*this, &Window::touche_clavier), false);
    add_controller(controller);
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
    labels.append(label_nr);
    label_maj.set_halign(Gtk::Align::START);
    label_pa.set_halign(Gtk::Align::START);
    label_rs.set_halign(Gtk::Align::START);
    label_rr.set_halign(Gtk::Align::START);
    label_ns.set_halign(Gtk::Align::START);
    label_np.set_halign(Gtk::Align::START);
    label_nd.set_halign(Gtk::Align::START);
    label_nr.set_halign(Gtk::Align::START);
    label_maj_.set_halign(Gtk::Align::END);
    label_pa_.set_halign(Gtk::Align::END);
    label_rs_.set_halign(Gtk::Align::END);
    label_rr_.set_halign(Gtk::Align::END);
    label_ns_.set_halign(Gtk::Align::END);
    label_np_.set_halign(Gtk::Align::END);
    label_nd_.set_halign(Gtk::Align::END);
    label_nr_.set_halign(Gtk::Align::END);
    stats.append(label_maj_);
    stats.append(label_pa_);
    stats.append(label_rs_);
    stats.append(label_rr_);
    stats.append(label_ns_);
    stats.append(label_np_);
    stats.append(label_nd_);
    stats.append(label_nr_);
    texte.append(labels);
    texte.append(stats);
    menu.append(boutons);
    menu.append(separator1);
    menu.append(texte);
    fenetre.append(menu);
    fenetre.append(separator2);
    fenetre.append(drawingArea_);
    set_child(fenetre);
    drawingArea_.set_expand();
    actualiser_stats();
}

void Window::actualiser_stats() {
    label_maj_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_update()));
    label_pa_.set_label(to_string(drawingArea_.get_sim().get_particules().size()));
    label_rs_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_nbRs()));
    label_rr_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_nbRr()));
    label_ns_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_nbNs()));
    label_np_.set_label(to_string(drawingArea_.get_sim().get_nbNp()));
    label_nd_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_nbNd()));
    label_nr_.set_label(to_string(drawingArea_.get_sim().get_spatial().get_nbNr()));
}

void Window::fichier_selectionne(int reponse, Gtk::FileChooserDialog* dialogue) {
    if(reponse == 1) {
        auto fichier = dialogue->get_file()->get_path();
        dialogue->hide();
        ifstream variable(fichier);
        Simulation sim2(0);
        sim2.lecture(variable);
        drawingArea_.set_sim(sim2);
        actualiser_stats();
        drawingArea_.queue_draw();
    } else if (reponse == 2) {
        ofstream fw(dialogue->get_file()->get_path(), std::ofstream::out);
        fw.is_open();
        fw << drawingArea_.get_sim().get_nbP() << endl;
        for (auto P : drawingArea_.get_sim().get_particules()) {
            fw << "\t" << P.get_forme().centre.x << " " << P.get_forme().centre.y;
            fw << " " << P.get_forme().cote << endl;
        }
        fw << endl;
        fw << drawingArea_.get_sim().get_spatial().get_info() << endl;
        for (auto R : drawingArea_.get_sim().get_reparateurs()) {
            fw << "\t" << R.get_forme().centre.x << " ";
            fw << R.get_forme().centre.y << endl;
        }
        fw << endl;
        for (auto N : drawingArea_.get_sim().get_neutraliseurs()) {
            fw << "\t" << N.get_info() << endl;
        }
        fw.close();
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
    dialogue->set_transient_for(*this);
    dialogue->set_modal(true);
    dialogue->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &Window::fichier_selectionne), dialogue));
    dialogue->add_button("Annuler", Gtk::ResponseType::CANCEL);
    dialogue->add_button("Ouvrir", 1);
    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("Fichiers .txt");
    filter_cpp->add_mime_type("text/plain");
    dialogue->add_filter(filter_cpp);
    dialogue->show();
}

void Window::save_button_clicked() {
    auto dialogue = new Gtk::FileChooserDialog("Sélectionner un fichier",
                                               Gtk::FileChooser::Action::SAVE);
    dialogue->set_transient_for(*this);
    dialogue->set_modal(true);
    dialogue->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &Window::fichier_selectionne), dialogue));
    dialogue->add_button("Annuler", Gtk::ResponseType::CANCEL);
    dialogue->add_button("Sauvegarder", 2);
    dialogue->show();

}

void Window::start_button_clicked() {
    if(minuteur_) {
        minuteur_ = false;
        start_button_.set_label("start");
    } else {
        minuteur_ = true;
        sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
                                                              &Window::on_timeout));
        auto conn = Glib::signal_timeout().connect(my_slot, int(delta_t*1000));
        start_button_.set_label("stop");
    }
}

void Window::step_button_clicked() {
    drawingArea_.get_sim().update();
    actualiser_stats();
    drawingArea_.queue_draw();
}

bool Window::touche_clavier(guint keyval, guint keycode, Gdk::ModifierType state) {
    switch(gdk_keyval_to_unicode(keyval)) {
        case 's':
            start_button_clicked();
            return true;
            break;
        case '1':
            step_button_clicked();
    }
    return false;
}

bool Window::on_timeout() {
    if(minuteur_) {
        drawingArea_.get_sim().update();
        actualiser_stats();
        drawingArea_.queue_draw();
        return true;
    } else {
        return false;
    }
}
