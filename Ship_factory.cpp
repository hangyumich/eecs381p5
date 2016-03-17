#include "Ship_factory.h"
#include "Ship.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Utility.h"


Ship* create_ship(const std::string& name, const std::string& type, Point initial_position)
{
    Ship* ship_ptr = nullptr;
    if (type == "Tanker") {
        ship_ptr = new Tanker(name, initial_position);
        return ship_ptr;
    }
    if (type == "Cruiser") {
        ship_ptr = new Cruiser(name, initial_position);
        return ship_ptr;
    }
    throw Error("Trying to create ship of unknown type!");
}