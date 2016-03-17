#include "Tanker.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>

using std::cout;
using std::endl;

/*Initial values:
fuel capacity and initial amount 100 tons, maximum speed 10., fuel consumption 2.tons/nm,
resistance 0, cargo capacity 1000 tons, initial cargo is 0 tons.*/
Tanker::Tanker(const std::string& name_, Point position_):
Ship(name_, position_, 100., 10., 2., 0), cargo_capacity(1000), cargo(0), load_destination(nullptr), unload_destination(nullptr), tanker_state(no_cargo_dest)
{
    cout << "Tanker " << get_name() << " constructed" << endl;
}

// output destructor message
Tanker::~Tanker()
{
    cout << "Tanker " <<get_name() << " destructed" << endl;
}

// This class overrides these Ship functions so that it can check if this Tanker has assigned cargo destinations.
// if so, throw an Error("Tanker has cargo destinations!"); otherwise, simply call the Ship functions.
void Tanker::set_destination_position_and_speed(Point destination_point, double speed)
{
    check_no_cargo_destination();
    Ship::set_destination_position_and_speed(destination_point, speed);
}
void Tanker::set_destination_island_and_speed(Island* destination_island, double speed)
{
    check_no_cargo_destination();
    Ship::set_destination_island_and_speed(destination_island, speed);
}
void Tanker::set_course_and_speed(double course, double speed)
{
    check_no_cargo_destination();
    Ship::set_course_and_speed(course, speed);
}

// Set the loading and unloading Island destinations
// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
// if both destinations are now set, start the cargo cycle
void Tanker::set_load_destination(Island* load_dest_)
{
    check_no_cargo_destination();
    load_destination = load_dest_;
    if (load_destination == unload_destination) {
        throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will load at " << load_dest_->get_name() << endl;
    into_cycle();
}
void Tanker::set_unload_destination(Island* unload_dest_){
    check_no_cargo_destination();
    unload_destination = unload_dest_;
    if (load_destination == unload_destination) {
        throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will unload at " << unload_dest_->get_name() << endl;
    into_cycle();
}

// when told to stop, clear the cargo destinations and stop
void Tanker::stop(){
    Ship::stop();
    load_destination = nullptr;
    unload_destination = nullptr;
    tanker_state = TankerState::no_cargo_dest;
    cout << get_name() << " now has no cargo destinations" << endl;
}

// perform Tanker-specific behavior
void Tanker::update()
{
    Ship::update();
    if (!can_move()) {
        tanker_state = TankerState::no_cargo_dest;
        load_destination = nullptr;
        unload_destination = nullptr;
        cout << get_name() <<  " now has no cargo destinations" << endl;
        return;
    }
    if (tanker_state == TankerState::no_cargo_dest)
        return;
    if (tanker_state == TankerState::moving_to_loading) {
        if (!is_moving() && can_dock(load_destination)) {
            dock(load_destination);
            tanker_state = TankerState::loading;
            return;
        }
    }
    if (tanker_state == TankerState::moving_to_unloading) {
        if (!is_moving() && can_dock(unload_destination)) {
            dock(unload_destination);
            tanker_state = TankerState::unloading;
            return;
        }
    }
    if (tanker_state == TankerState::loading) {
        Ship::refuel();
        double needed_for_cargo = cargo_capacity - cargo;
        if (needed_for_cargo < 0.005) {
            cargo = cargo_capacity;
            Ship::set_destination_island_and_speed(unload_destination, get_maximum_speed());
            tanker_state = TankerState::moving_to_unloading;
            return;
        } else {
            double cargo_provided = load_destination->provide_fuel(needed_for_cargo);
            cargo += cargo_provided;
            cout << get_name() <<   " now has " << cargo << " of cargo" << endl;
            return;
        }
    }
    if (tanker_state == TankerState::unloading) {
        if (cargo == 0.) {
            Ship::set_destination_island_and_speed(load_destination, get_maximum_speed());
            tanker_state = TankerState::moving_to_loading;
            return;
        } else {
            unload_destination->accept_fuel(cargo);
            cargo = 0.;
            return;
        }
    }
}

void Tanker::describe() const
{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo  << " tons";
    switch (tanker_state) {
        case no_cargo_dest:
            cout << ", no cargo destinations" << endl;
            break;
        case loading:
            cout << ", loading" << endl;
            break;
        case unloading:
            cout << ", unloading" << endl;
            break;
        case moving_to_loading:
            cout << ", moving to loading destination" << endl;
            break;
        case moving_to_unloading:
            cout << ", moving to unloading destination" << endl;
            break;
        default:
            break;
    }
}

void Tanker::check_no_cargo_destination()
{
    if (tanker_state != no_cargo_dest)
        throw Error("Tanker has cargo destinations!");
}

void Tanker::into_cycle() {
    if (!load_destination || !unload_destination)
        return;
    if (is_docked()) {
        if (get_docked_Island() == load_destination) {
            tanker_state = TankerState::loading;
            return;
        }
        if (get_docked_Island() == unload_destination) {
            tanker_state = TankerState::unloading;
            return;
        }
    }
    if (!is_moving()) {
        if (cargo == 0 && can_dock(load_destination)) {
            dock(load_destination);
            tanker_state = TankerState::loading;
            return;
        }
        if (cargo > 0 && can_dock(unload_destination)) {
            dock(unload_destination);
            tanker_state = TankerState::unloading;
            return;
        }
    }
    if (cargo == 0) {
        Ship::set_destination_island_and_speed(load_destination, get_maximum_speed());
        tanker_state = TankerState::moving_to_loading;
        return;
    }
    if (cargo > 0) {
        Ship::set_destination_island_and_speed(unload_destination, get_maximum_speed());
        tanker_state = TankerState::moving_to_unloading;
        return;
    }
}