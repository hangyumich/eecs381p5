#include "Model.h"
#include "Island.h"
#include "Ship.h"
#include "Sim_object.h"
#include "View.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility> //for std::pair

using std::cout;
using std::endl;

typedef std::map<std::string, Sim_object*>::value_type value_type;

Model* g_Model_ptr;

/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */
Model::Model(): time(0)
{
    g_Model_ptr = this;
    Island* island1 = new Island("Exxon", Point(10, 10), 1000, 200);
    Island* island2 = new Island("Shell", Point(0, 30), 1000, 200);
    Island* island3 = new Island("Bermuda", Point(20, 20));
    Ship* ship1 = create_ship("Ajax", "Cruiser", Point (15, 15));
    Ship* ship2 = create_ship("Xerxes", "Cruiser", Point (25, 25));
    Ship* ship3 = create_ship("Valdez", "Tanker", Point (30, 30));
    insert_island(island1);
    insert_island(island2);
    insert_island(island3);
    insert_ship(ship1);
    insert_ship(ship2);
    insert_ship(ship3);
    cout << "Model constructed" << endl;
}

Model::~Model()
{
    std::for_each(sim_map.begin(), sim_map.end(),
                  [](const value_type& pair){delete pair.second;});
    cout << "Model destructed" << endl;
}

// is name already in use for either ship or island?
// either the identical name, or identical in first two characters counts as in-use
bool Model::is_name_in_use(const std::string& name) const
{
    return (sim_map.find(name) != sim_map.end()) ||
    (name_set.find(name.substr(0, 2)) != name_set.end());
}

// is there such an island?
bool Model::is_island_present(const std::string& name) const
{
    return island_map.find(name) != island_map.end();
}

// will throw Error("Island not found!") if no island of that name
Island* Model::get_island_ptr(const std::string& name) const
{
    if (island_map.find(name) == island_map.end()) {
        throw Error("Island not found!");
    }
    return island_map.at(name);
}

// is there such an ship?
bool Model::is_ship_present(const std::string& name) const
{
    return ship_map.find(name) != ship_map.end();
}

// add a new ship to the list, and update the view
void Model::add_ship(Ship* ship_ptr)
{
    insert_ship(ship_ptr);
    ship_ptr->broadcast_current_state();
}

// will throw Error("Ship not found!") if no ship of that name
Ship* Model::get_ship_ptr(const std::string& name) const
{
    if (ship_map.find(name) == ship_map.end()) {
        throw Error("Ship not found!");
    }
    return ship_map.at(name);
}

// tell all objects to describe themselves
void Model::describe() const
{
    std::for_each(sim_map.begin(), sim_map.end(),
                  [](const value_type& pair){pair.second->describe();});
}

// increment the time, and tell all objects to update themselves
void Model::update()
{
    time ++;
    std::vector<Ship*> bottom_ships;
    std::for_each(sim_map.begin(), sim_map.end(),
                  [](const value_type& pair){pair.second->update();});
    std::for_each(ship_map.begin(), ship_map.end(),
                  [&](const std::pair<const std::string, Ship*>& pair){
                      if (pair.second->is_on_the_bottom()) {
                          delete pair.second;
                          sim_map.erase(pair.first);
                          ship_map.erase(pair.first);
                      }});
}


/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(View* view_ptr)
{
    views.insert(view_ptr);
    std::for_each(sim_map.begin(), sim_map.end(),
                  [](value_type& pair){pair.second->broadcast_current_state();});
}

// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(View* view_ptr)
{
    views.erase(view_ptr);
}

// notify the views about an object's location
void Model::notify_location(const std::string& name, Point location)
{
    std::for_each(views.begin(), views.end() ,
                  [&](View* view_ptr){view_ptr->update_location(name, location);});
}

// notify the views that an object is now gone
void Model::notify_gone(const std::string& name)
{
    std::for_each(views.begin(), views.end() ,
                  [&](View* view_ptr){view_ptr->update_remove(name);});
}


void Model::insert_ship(Ship* ship_ptr)
{
    sim_map[ship_ptr->get_name()] = ship_ptr;
    ship_map[ship_ptr->get_name()] = ship_ptr;
    name_set.insert(ship_ptr->get_name().substr(0, 2));
}

void Model::insert_island(Island* island_ptr)
{
    sim_map[island_ptr->get_name()] = island_ptr;
    island_map[island_ptr->get_name()] = island_ptr;
    name_set.insert(island_ptr->get_name().substr(0, 2));
}
