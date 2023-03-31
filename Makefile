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
	Shape/Shape.cpp
OFILES = Projet.o Constantes.o Simulation/Simulation.o Particule/Particule.o \
	Robot/Robot.o Message/Message.o Shape/Shape.o

all: $(OUT)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo "*** MODULES OBJETS ET EXECUTABLES EFFACES ***"
	@/bin/rm -f *.o */*.o $(OUT)

Projet.o: Projet.cpp Simulation/Simulation.cpp Simulation/Simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

Simulation.o: Simulation/Simulation.cpp Simulation/Simulation.h \
  Simulation/../Particule/Particule.h \
  Simulation/../Particule/../Shape/Shape.h \
  Simulation/../Particule/../Constantes.h \
  Simulation/../Particule/../Message/Message.h \
  Simulation/../Robot/Robot.h Simulation/../Robot/../Message/Message.h \
  Simulation/../Robot/../Shape/Shape.h \
  Simulation/../Robot/../Particule/Particule.h

Particule.o: Particule/Particule.cpp Particule/Particule.h \
  Particule/../Shape/Shape.h Particule/../Constantes.h \
  Particule/../Message/Message.h

Robot.o: Robot/Robot.cpp Robot/Robot.h Robot/../Message/Message.h \
  Robot/../Shape/Shape.h Robot/../Particule/Particule.h \
  Robot/../Particule/../Shape/Shape.h Robot/../Particule/../Constantes.h \
  Robot/../Particule/../Message/Message.h Robot/../Constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

Message.o: Message/Message.cpp Message/Message.h

Shape.o: Shape/Shape.cpp Shape/Shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
