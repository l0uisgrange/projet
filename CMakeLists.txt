cmake_minimum_required(VERSION 3.25)
project(projet)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(projet
        Projet.cpp
        Constantes.h
        Simulation/Simulation.h
        Shape/Shape.h
        Robot/Robot.h
        Particule/Particule.h
        Message/Message.h
        Simulation/Simulation.cpp
        Message/Message.cpp
        Particule/Particule.cpp
        Robot/Robot.cpp
        Shape/Shape.cpp Gui/Gui.h Gui/Gui.cpp Constantes.cpp Graphic/Graphic.cpp Graphic/Graphic.h Graphic/GraphicPartielle.h)

target_link_libraries(projet PRIVATE PkgConfig::GTK4)
