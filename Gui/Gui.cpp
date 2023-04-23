/**
 Gui.cpp
 Louis Grange et Daniel Ataide
 Version 1.0
**/

static constexpr unsigned taille_dessin(500); //TODO enlever et mettre dans constantes.h

#include "Gui.h"
#include <vector>
#include <iostream>
using namespace std;

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                    const Frame& frame);

static void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame);

//TODO quelle est frame de référence ?
static Frame default_frame = {-500., 500.,
                              -500., 500.,
                              1., 500, 500};


Drawing::Drawing() {

    set_frame(default_frame);
    set_content_width(taille_dessin);
	set_content_height(taille_dessin);
    set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    //empecher distorsion
    adjustFrame(width, height);
    draw_frame(cr, frame_);
    orthographic_projection(cr,frame_);
    //pointeur sur cr
    graphic_set_context(cr);
    draw_cercle(100,100, 40, ORANGE);
    draw_carre(200,200,50, BLEU_CLAIR);
    //graphic_draw_shape(width, height); TODO draw_shape encore necesAIRE?
};

void Drawing::set_frame(Frame f) { //TODO mettre dans graphicgui?
    if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
    {
        f.asp = f.width/f.height;
        frame_ = f;
    }
    else
        exit(1); //TODO quelle erreur si mauvais affichage?
}

void Drawing::adjustFrame(int width, int height) {
    //Prevents distorsion
    frame_.width  = min(width, height);
    frame_.height = min(width,height);

    //reference framing as a guide to prevent distorsion
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp) //x > y
    { // garde ymax/min, adapte xmax/min
        frame_.yMax = default_frame.yMax ;
        frame_.yMin = default_frame.yMin ;

        double delta(default_frame.xMax - default_frame.xMin);
        double mid((default_frame.xMax + default_frame.xMin)/2);
        // centré point-milieu selon x
        frame_.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
        frame_.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
    }else{ // garde xmax/min, adapte ymax/min
        frame_.xMax = default_frame.xMax ;
        frame_.xMin = default_frame.xMin ;

        double delta(default_frame.yMax - default_frame.yMin);
        double mid((default_frame.yMax + default_frame.yMin)/2);
        // centré point milieu selon y
        frame_.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
        frame_.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
    }
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                    const Frame& frame) {
    // déplace l'origine au centre du carré le plus grand possible
    double centre_carre(min(frame.width,frame.height)/2);
    cr->translate(centre_carre, centre_carre);

    // normalise la largeur et hauteur selon default frame
    // ET inverse la direction de l'axe Y
    double axe_min_diff(0.); //donne xMax-xMin ou yMax-yMin
    double axe_min_sum(0.); // xMin+xMax ou yMin+yMin
    if (frame.xMax > frame.yMax) {
        axe_min_diff = frame.yMax - frame.yMin;
        axe_min_sum =frame.yMin + frame.yMax;
    }else{
        axe_min_diff = frame.xMax - frame.xMin;
        axe_min_sum = frame.xMin + frame.xMax;
    }
    cr->scale(frame.width/(axe_min_diff),
              -frame.height/(axe_min_diff));
    // décalage au centre du cadrage
    cr->translate(-(axe_min_sum)/2, -(axe_min_sum)/2);
}

static void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame)
{
    cr->set_line_width(5.0);
    cr->set_source_rgb(0.5, 0.5, 0.5);
    cr->rectangle(0,0, frame.width, frame.height);
    cr->stroke();
}

//FIN DE DRAWING AREA

Window::Window() : exit_button_("exit"), open_button_("open"),
                   save_button_("save"), start_button_("start"),
                   step_button_("step"),
                   label_maj_("0"), label_pa_("0"),
                   label_rs_("0"), label_rr_("0"),
                   label_ns_("0"), label_np_("0"),
                   label_nd_("0"), label_nr_("0"),
                   drawingArea_() {
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
    boutons.append(exit_button_);
    boutons.append(open_button_);
    boutons.append(save_button_);
    boutons.append(start_button_);
    boutons.append(step_button_);
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
    labels.append(label_nr);
    label_maj.set_halign(Gtk::Align::START);
    label_pa.set_halign(Gtk::Align::START);
    label_rs.set_halign(Gtk::Align::START);
    label_rr.set_halign(Gtk::Align::START);
    label_ns.set_halign(Gtk::Align::START);
    label_np.set_halign(Gtk::Align::START);
    label_nd.set_halign(Gtk::Align::START);
    label_nr.set_halign(Gtk::Align::START);
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
    drawingArea_.set_expand(); //TODO quelles autres boutons faut mettre set_expand??
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
        //simulation.lecture(fichier);
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
    dialogue->set_transient_for(*this);
    dialogue->set_modal(true);
    dialogue->signal_response().connect(sigc::bind(
            sigc::mem_fun(*this, &Window::fichier_selectionne), dialogue));
    dialogue->add_button("Annuler", Gtk::ResponseType::CANCEL);
    dialogue->add_button("Ouvrir", Gtk::ResponseType::OK);
    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("Fichiers .txt");
    filter_cpp->add_mime_type("text/plain");
    dialogue->add_filter(filter_cpp);
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