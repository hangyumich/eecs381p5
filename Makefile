CC = g++
LD = g++

CFLAGS = -c -std=c++14 -pedantic-errors -Wall
LFLAGS = -pedantic-errors -Wall

OBJS = p5_main.o Model.o View.o Controller.o Ship_factory.o Cruiser.o Tanker.o Ship.o Island.o Sim_object.o Track_base.o Geometry.o Navigation.o Utility.o
PROG = p5exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p4_main.o: p5_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p4_main.cpp

Sim_object.o: Sim_object.cpp Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Sim_object.cpp

Ship.o: Ship.cpp Ship.h Island.h Model.h Utility.h Navigation.h
	$(CC) $(CFLAGS) Ship.cpp

Track_base.o: Track_base.cpp Track_base.h Navigation.h
	$(CC) $(CFLAGS) Track_base.cpp

Geometry.o: Geometry.cpp Geometry.h 
	$(CC) $(CFLAGS) Geometry.cpp

Navigation.o: Navigation.cpp Navigation.h Geometry.h
	$(CC) $(CFLAGS) Navigation.cpp

Island.o: Island.cpp Island.h Model.h
	$(CC) $(CFLAGS) Island.cpp

Model.o: Model.cpp Model.h Island.h Ship.h Sim_object.h View.h Ship_factory.h Utility.h
	$(CC) $(CFLAGS) Model.cpp
	
Controller.o: Controller.cpp Controller.h Model.h View.h Ship.h Island.h Geometry.h Ship_factory.h
	$(CC) $(CFLAGS) Controller.cpp

View.o: View.cpp View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) View.cpp

Ship_factory.o: Ship_factory.cpp Ship_factory.h Ship.h Tanker.h Cruiser.h Utility.h
	$(CC) $(CFLAGS) Ship_factory.cpp

Cruiser.o: Cruiser.cpp Cruiser.h Ship.h Island.h Utility.h Geometry.h Track_base.h
	$(CC) $(CFLAGS) Cruiser.cpp

Tanker.o: Tanker.cpp Tanker.h Island.h Utility.h Ship.h Geometry.h Track_base.h
	$(CC) $(CFLAGS) Tanker.cpp

Utility.o: Utility.cpp Utility.h 
	$(CC) $(CFLAGS) Utility.cpp

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe

