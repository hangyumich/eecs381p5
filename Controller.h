/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H


class View;
class Ship;

class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:
	// Controller keeps its own pointer to the View because it has to manage the View.
	// Future versions will need to manage more than one view.
	View* view_ptr;
    
    // reset attacked views to default
    void set_defaults();
    // set the size of view
    void read_size();
    // set the scale of view
    void set_zoom();
    // set the origin point
    void set_pan();
    // draw views
    void show_views();
    // call objects describe in alphabetical order
    void model_status();
    // model update
    void model_go();
    // create a ship
    void model_create();
    
    // corresponding ship functions
    void set_ship_course(Ship*);
    void set_ship_position(Ship*);
    void set_ship_destination(Ship*);
    void set_load_destination(Ship*);
    void set_unload_destination(Ship*);
    void set_dock_island(Ship*);
    void attack(Ship*);
    void refuel(Ship*);
    void stop(Ship*);
    void stop_attack(Ship*);
};

#endif