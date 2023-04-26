#
# Makefile
# Louis Grange et Daniel Ataide
# Version 1.0
#

OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = Projet.cpp Simulation/Simulation.cpp \
	Particule/Particule.cpp Robot/Robot.cpp Message/Message.cpp \
	Shape/Shape.cpp Gui/Gui.cpp Graphic/Graphic.cpp
OFILES = Projet.o Simulation/Simulation.o Particule/Particule.o \
	Robot/Robot.o Message/Message.o Shape/Shape.o Gui/Gui.o Graphic/Graphic.o

all: $(OUT)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "*** MODULES OBJETS ET EXECUTABLES EFFACES ***"
	@/bin/rm -f *.o */*.o $(OUT)

Simulation/Simulation.o: Simulation/Simulation.cpp Simulation/Simulation.h \
  Robot/Robot.h \
  Constantes.h \
  Particule/Particule.h \
  Message/Message.h \
  Shape/Shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Particule/Particule.o: Particule/Particule.cpp Particule/Particule.h \
  Constantes.h \
  Message/Message.h \
  Shape/Shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Robot/Robot.o: Robot/Robot.cpp Robot/Robot.h \
  Message/Message.h \
  Particule/Particule.h \
  Shape/Shape.h \
  Constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Graphic/Graphic.o: Graphic/Graphic.cpp Graphic/Graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Message/Message.o: Message/Message.cpp Message/Message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Shape/Shape.o: Shape/Shape.cpp Shape/Shape.h \
  Graphic/Graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Gui/Gui.o: Gui/Gui.cpp Gui/Gui.h \
  Graphic/Graphic.h \
  Simulation/Simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Projet.o: Projet.cpp \
  Simulation/Simulation.h \
  Gui/Gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)