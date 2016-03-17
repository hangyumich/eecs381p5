#include "Island.h"
#include "Model.h"
#include <iostream>

Island::Island (const std::string& name_, Point position_, double fuel_, double production_rate_):
Sim_object(name_), position(position_), fuel(fuel_), production_rate(production_rate_)
{
    std::cout << "Island " << Sim_object::get_name() <<  " constructed" << std::endl;
}

Island::~Island()
{
    std::cout << "Island " << Sim_object::get_name() << " destructed" << std::endl;
}

// if production_rate > 0, compute production_rate * unit time, and add to amount, and print an update message
void Island::update()
{
    if (production_rate > 0) {
        fuel += production_rate;
        std::cout << "Island " << Sim_object::get_name() << " now has " << fuel << " tons" << std::endl;
    }
}

// output information about the current state
void Island::describe() const
{
    std::cout << "\nIsland " << get_name() << " at position " << position << std::endl;
    std::cout << "Fuel available: " << fuel << " tons" << std::endl;
}

// ask model to notify views of current state
void Island::broadcast_current_state() const
{
    g_Model_ptr->notify_location(Sim_object::get_name(), position);
}

// Return whichever is less, the request or the amount left,
// update the amount on hand accordingly, and output the amount supplied.
double Island::provide_fuel(double request)
{
    double provide = (fuel < request) ? fuel : request;
    fuel -= provide;
    std::cout << "Island " << Sim_object::get_name() << " supplied " << provide << " tons of fuel" << std::endl;
    return provide;
}

// Add the amount to the amount on hand, and output the total as the amount the Island now has.
void Island::accept_fuel(double amount)
{
    fuel += amount;
    std::cout << "Island " << Sim_object::get_name() << " now has " << fuel << " tons" << std::endl;

}
