/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

/* 
*** This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
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
    
    void set_defaults();
    void read_size();
    void set_zoom();
    void set_pan();
    void show_views();
    void model_status();
    void model_go();
    void model_create();
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