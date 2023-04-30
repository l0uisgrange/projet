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
CXXFILES = Projet.cpp Simulation.cpp \
	Particule.cpp Robot.cpp Message.cpp \
	Shape.cpp Gui.cpp Graphic.cpp
OFILES = Projet.o Simulation.o Particule.o \
	Robot.o Message.o Shape.o Gui.o Graphic.o

all: $(OUT)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "*** MODULES OBJETS ET EXECUTABLES EFFACES ***"
	@/bin/rm -f *.o */*.o $(OUT)

Simulation.o: Simulation.cpp Simulation.h \
  Robot.h \
  Constantes.h \
  Particule.h \
  Message.h \
  Shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Particule.o: Particule.cpp Particule.h \
  Constantes.h \
  Message.h \
  Shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Robot.o: Robot.cpp Robot.h \
  Message.h \
  Particule.h \
  Shape.h \
  Constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Graphic.o: Graphic.cpp Graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Message.o: Message.cpp Message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Shape.o: Shape.cpp Shape.h \
  Graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Gui.o: Gui.cpp Gui.h \
  Graphic.h \
  Simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
Projet.o: Projet.cpp \
  Simulation.h \
  Gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)