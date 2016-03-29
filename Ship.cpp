#include "Ship.h"
#include "Island.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>

using std::cout;
using std::endl;

// initialize, then output constructor message
Ship::Ship(const std::string& name_, Point position_, double fuel_capacity_,
           double maximum_speed_, double fuel_consumption_, int resistance_):
Sim_object(name_), Track_base(position_), fuel_capacity(fuel_capacity_), fuel(fuel_capacity_), maximum_speed(maximum_speed_), fuel_consumption(fuel_consumption_), resistance(resistance_), ship_state(State::stopped)
{
    cout << "Ship " << get_name() << " constructed" << endl;
}

/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/

Ship::~Ship()
{
    cout << "Ship "  << get_name() << " destructed" << endl;
}

//Public function definitions

// Return true if ship can move (it is not dead in the water or in the process or sinking);
bool Ship::can_move() const
{
    return is_afloat() && (ship_state != State::dead_in_the_water);
}

// Return true if ship is moving
bool Ship::is_moving() const
{
    return (ship_state == State::moving_on_course || ship_state == State::moving_to_island ||
            ship_state == State::moving_to_position);
}

// Return true if ship is docked
bool Ship::is_docked() const
{
    return (ship_state == State::docked);
}

// Return true if ship is afloat (not in process of sinking), false if not
bool Ship::is_afloat() const
{
    return !(ship_state == State::sinking || ship_state == State::sunk ||
             ship_state == State::on_the_buttom);
}

// Return true if ship is on the bottom
bool Ship::is_on_the_bottom() const
{
    return (ship_state == State::on_the_buttom);
}

// Return true if the ship is Stopped and the distance to the supplied island
// is less than or equal to 0.1 nm
bool Ship::can_dock(Island* island_ptr) const
{
    if (ship_state != State::stopped)
        return false;
    double destination_distance = cartesian_distance(get_location(), island_ptr->get_location());
    return (destination_distance <= 0.1);
}

/*** Interface to derived classes ***/
// Update the state of the Ship
void Ship::update()
{
    switch (ship_state) {
        case State::sinking:
            ship_state = State::sunk;
            cout << get_name() << " sunk" << endl;
            g_Model_ptr->notify_gone(get_name());
            break;
        case State::sunk:
            ship_state = State::on_the_buttom;
            cout << get_name() << " on the bottom" << endl;
            break;
        case State::on_the_buttom:
            cout << get_name() << " on the bottom" << endl;
            break;
        default:
            break;
    }
    if (!is_afloat())
        return;
    if (resistance < 0) {
        set_speed(0.);
        ship_state = State::sinking;
        cout << get_name() << " sinking" << endl;
        return;
    }
    if (is_moving()) {
        calculate_movement();
        g_Model_ptr->notify_location(get_name(), get_location());
        cout << get_name() << " now at " << get_location() << endl;
        return;
    }
    switch (ship_state) {
        case State::stopped:
            cout << get_name() << " stopped at " << get_position() << endl;
            break;
        case State::docked:
            cout << get_name() << " docked at " << dock_island->get_name() << endl;
            break;
        case State::dead_in_the_water:
            cout << get_name() << " dead in the water at " << get_position() << endl;
            break;
        default:
            break;
    }
}
// output a description of current state to cout
void Ship::describe() const
{
    cout << get_name() << " at " << get_position();
    switch (ship_state) {
        case State::sinking:
            cout << " sinking" << endl;
            break;
        case State::sunk:
            cout << " sunk" << endl;
            break;
        case State::on_the_buttom:
            cout << " on the bottom" << endl;
            break;
        default:
            cout << ", fuel: " << fuel << " tons, resistance: " << resistance << endl;
            break;
    }
    switch (ship_state) {
        case State::moving_to_position:
            cout << "Moving to " << destination_point <<  " on " << Track_base::get_course_speed() << endl;
            break;
        case State::moving_to_island:
            cout << "Moving to " << destination_island->get_name() <<  " on " << Track_base::get_course_speed() << endl;
            break;
        case State::moving_on_course:
            cout << "Moving on " << Track_base::get_course_speed() << endl;
            break;
        case State::docked:
            cout << "Docked at " << dock_island->get_name() << endl;
            break;
        case State::stopped:
            cout << "Stopped" << endl;
            break;
        case State::dead_in_the_water:
            cout << "Dead in the water" << endl;
            break;
        default:
            break;
    }
}

void Ship::broadcast_current_state() const
{
    g_Model_ptr->notify_location(get_name(), get_location());
}

/*** Command functions ***/
// Start moving to a destination position at a speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!")
void Ship::set_destination_position_and_speed(Point destination_position, double speed)
{
    if (!can_move())
        throw Error("Ship cannot move!");
    if (speed > maximum_speed)
        throw Error("Ship cannot go that fast!");
    Compass_vector des_vec(get_location(), destination_position);
    Track_base::set_course_speed(Course_speed(des_vec.direction, speed));
    ship_state = State::moving_to_position;
    destination_point = destination_position;
    cout <<  get_name() << " will sail on " << get_course_speed() <<  " to " << destination_point << endl;
}
// Start moving to a destination Island at a speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!")
void Ship::set_destination_island_and_speed(Island* destination_island_, double speed)
{
    if (!can_move())
        throw Error("Ship cannot move!");
    if (speed > maximum_speed)
        throw Error("Ship cannot go that fast!");
    Compass_vector des_vec(get_location(), destination_island_->get_location());
    Track_base::set_course_speed(Course_speed(des_vec.direction, speed));
    ship_state = State::moving_to_island;
    destination_island = destination_island_;
    destination_point = destination_island->get_location();
    cout <<  get_name() << " will sail on " << get_course_speed() <<  " to " << destination_island->get_name() << endl;
}
// Start moving on a course and speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!");
void Ship::set_course_and_speed(double course, double speed)
{
    if (!can_move())
        throw Error("Ship cannot move!");
    if (speed > maximum_speed)
        throw Error("Ship cannot go that fast!");
    Track_base::set_course_speed(Course_speed(course, speed));
    ship_state = State::moving_on_course;
    cout <<  get_name() << " will sail on " << get_course_speed() << endl;
}
// Stop moving
// may throw Error("Ship cannot move!");
void Ship::stop()
{
    if (!can_move())
        throw Error("Ship cannot move!");
    set_speed(0.);
    cout << get_name() <<  " stopping at " << get_position() << endl;
    ship_state = State::stopped;
}
// dock at an Island - set our position = Island's position, go into Docked state
// may throw Error("Can't dock!");
void Ship::dock(Island * island_ptr)
{
    if (ship_state != State::stopped || cartesian_distance(get_location(), island_ptr->get_location()) > 0.1)
        throw Error("Can't dock!");
    set_position(island_ptr->get_location());
    g_Model_ptr->notify_location(get_name(), get_location());
    ship_state = State::docked;
    dock_island = island_ptr;
    cout << get_name() <<  " docked at " << island_ptr->get_name() << endl;
}
// Refuel - must already be docked at an island; fill takes as much as possible
// may throw Error("Must be docked!");
void Ship::refuel()
{
    if (!is_docked()) {
        throw Error("Must be docked!");
    }
    double fuel_needed = fuel_capacity - fuel;
    if (fuel_needed < 0.005) {
        fuel = fuel_capacity;
    } else {
        double fuel_provided = dock_island->provide_fuel(fuel_needed);
        fuel += fuel_provided;
        cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
    }
}

/*** Fat interface command functions ***/
// These functions throw an Error exception for this class
// will always throw Error("Cannot load at a destination!");
void Ship::set_load_destination(Island *)
{
    throw Error("Cannot load at a destination!");
}

// will always throw Error("Cannot unload at a destination!");
void Ship::set_unload_destination(Island *)
{
    throw Error("Cannot unload at a destination!");
}

// will always throw Error("Cannot attack!");
void Ship::attack(Ship * in_target_ptr)
{
    throw Error("Cannot attack!");
}

// will always throw Error("Cannot attack!");
void Ship::stop_attack()
{
    throw Error("Cannot attack!");
}

// interactions with other objects
// receive a hit from an attacker
void Ship::receive_hit(int hit_force, Ship* attacker_ptr)
{
    resistance -= hit_force;
    cout << get_name() << " hit with " << hit_force << ", resistance now " << resistance << endl;
}

//Protected function definitions

// future projects may need additional protected member functions

double Ship::get_maximum_speed() const
{
    return maximum_speed;
}

// return pointer to the Island currently docked at, or nullptr if not docked
Island* Ship::get_docked_Island() const
{
    if (is_docked()) {
        return dock_island;
    }
    return nullptr;
}

// return pointer to current destination Island, nullptr if not set
Island* Ship::get_destination_Island() const
{
    if (ship_state == State::moving_to_island) {
        return destination_island;
    }
    return nullptr;
}



/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
moving_to_position, moving_to_island, or moving_on_course.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or enum class names, or state names if you wish (e.g. movement_state).
*/
void Ship::calculate_movement()
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination_point);
	// get full step distance we can move on this time step
	double full_distance = get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
	// how far and how long can we sail in this time period based on the fuel state?
	double distance_possible, time_possible;
	if(full_fuel_required <= fuel) {
		distance_possible = full_distance;
		time_possible = time;
		}
	else {
		distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
		time_possible = (distance_possible / full_distance) * time;
		}
	
	// are we are moving to a destination, and is the destination within the distance possible?
	if((ship_state == State::moving_to_position || ship_state== State::moving_to_island) && destination_distance <= distance_possible) {
		// yes, make our new position the destination
        Track_base::set_position(destination_point);
		// we travel the destination distance, using that much fuel
		double fuel_required = destination_distance * fuel_consumption;
		fuel -= fuel_required;
		set_speed(0.);
		ship_state = State::stopped;
		}
	else {
		// go as far as we can, stay in the same movement state
		// simply move for the amount of time possible
		Track_base::update_position(time_possible);
		// have we used up our fuel?
		if(full_fuel_required >= fuel) {
			fuel = 0.0;
            set_speed(0.);
			ship_state = State::dead_in_the_water;
			}
		else {
			fuel -= full_fuel_required;
			}
		}
}

