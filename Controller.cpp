#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <limits>
using namespace std;

typedef void (Controller::*ControFuncPtr)();
typedef void (Controller::*ShipFuncPtr)(Ship*);

int get_int()
{
    int value;
    cin >> value;
    if (!cin.good()) {
        cin.clear();
        throw Error("Expected an integer!");
    }
    return value;
}

double get_double()
{
    double value;
    cin >> value;
    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }
    return value;
}

double get_speed()
{
    double speed = get_double();
    if (speed < 0) {
        throw Error("Negative speed entered!");
    }
    return speed;
}

Controller::Controller(): view_ptr(nullptr)
{
    cout << "Controller constructed" << endl;
}

Controller::~Controller()
{
    cout << "Controller destructed" << endl;
}

void Controller::run()
{
    map<string, ControFuncPtr>functions {
        {"default", reinterpret_cast<ControFuncPtr>(&Controller::set_defaults)},
        {"size", reinterpret_cast<ControFuncPtr>(&Controller::read_size)},
        {"zoom", reinterpret_cast<ControFuncPtr>(&Controller::set_zoom)},
        {"pan", reinterpret_cast<ControFuncPtr>(&Controller::set_pan)},
        {"show", reinterpret_cast<ControFuncPtr>(&Controller::show_views)},
        {"status", reinterpret_cast<ControFuncPtr>(&Controller::model_status)},
        {"go", reinterpret_cast<ControFuncPtr>(&Controller::model_go)},
        {"create", reinterpret_cast<ControFuncPtr>(&Controller::model_create)}
    };
    map<string, ShipFuncPtr> ship_functions {
        {"course", reinterpret_cast<ShipFuncPtr>(&Controller::set_ship_course)},
        {"position", reinterpret_cast<ShipFuncPtr>(&Controller::set_ship_position)},
        {"destination", reinterpret_cast<ShipFuncPtr>(&Controller::set_ship_destination)},
        {"load_at", reinterpret_cast<ShipFuncPtr>(&Controller::set_load_destination)},
        {"unload_at", reinterpret_cast<ShipFuncPtr>(&Controller::set_unload_destination)},
        {"dock_at", reinterpret_cast<ShipFuncPtr>(&Controller::set_dock_island)},
        {"attack", reinterpret_cast<ShipFuncPtr>(&Controller::attack)},
        {"refuel", reinterpret_cast<ShipFuncPtr>(&Controller::refuel)},
        {"stop", reinterpret_cast<ShipFuncPtr>(&Controller::stop)},
        {"stop_attack", reinterpret_cast<ShipFuncPtr>(&Controller::stop_attack)}
    };
    
    view_ptr = new View;
    g_Model_ptr->attach(view_ptr);
    while (1) {
        try {
            cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
            string first_cmd;
            cin >> first_cmd;
            if (first_cmd == "quit") {
                g_Model_ptr->detach(view_ptr);
                delete view_ptr;
                cout << "Done" << endl;
                break;
            }
            if (g_Model_ptr->is_ship_present(first_cmd)) {
                string ship_cmd;
                cin >> ship_cmd;
                if (ship_functions.find(ship_cmd) != ship_functions.end()) {
                    (this->*ship_functions[ship_cmd])(g_Model_ptr->get_ship_ptr(first_cmd));
                } else {
                    throw Error("Unrecognized command!");
                }
            } else {
                if (functions.find(first_cmd) != functions.end()) {
                    (this->*functions[first_cmd])();
                } else {
                    throw Error("Unrecognized command!");
                }
            }
        } catch (Error& e) {
            cout << e.what() << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } catch (std::exception& e) {
            cout << e.what();
            g_Model_ptr->detach(view_ptr);
            delete view_ptr;
            break;
        }
        
    }
    
}

void Controller::set_defaults()
{
    view_ptr->set_defaults();
}

void Controller::read_size()
{
    int size = get_int();
    view_ptr->set_size(size);
}


void Controller::set_zoom()
{
    double scale = get_double();
    view_ptr->set_scale(scale);
}


void Controller::set_pan()
{
    double x ,y;
    x = get_double();
    y = get_double();
    view_ptr->set_origin(Point(x, y));
}

void Controller::show_views()
{
    view_ptr->draw();
}

void Controller::model_status()
{
    g_Model_ptr->describe();
}

void Controller::model_go()
{
    g_Model_ptr->update();
}
void Controller::model_create()
{
    string name;
    cin >> name;
    if ((int)name.size() < 2) {
        throw Error("Name is too short!");
    }
    if (g_Model_ptr->is_name_in_use(name)) {
        throw Error("Name is invalid!");
    }
    string type;
    cin >> type;
    double x, y;
    x = get_double();
    y = get_double();
    Ship* new_ship = create_ship(name, type, Point(x, y));
    g_Model_ptr->add_ship(new_ship);
}
void Controller::set_ship_course(Ship* ship_ptr)
{
    double heading, speed;
    heading = get_double();
    if (heading < 0. || heading >=  360.0) {
        throw Error("Invalid heading entered!");
    }
    speed = get_speed();
    ship_ptr->set_course_and_speed(heading, speed);
    
}
void Controller::set_ship_position(Ship* ship_ptr)
{
    double x, y, speed;
    x = get_double();
    y = get_double();
    speed = get_speed();
    ship_ptr->set_destination_position_and_speed(Point(x, y), speed);
}

void Controller::set_ship_destination(Ship* ship_ptr)
{
    string name;
    cin >> name;
    double speed = get_speed();
    ship_ptr->set_destination_island_and_speed(g_Model_ptr->get_island_ptr(name), speed);
}

void Controller::set_load_destination(Ship* ship_ptr)
{
    string name;
    cin >> name;
    ship_ptr->set_load_destination(g_Model_ptr->get_island_ptr(name));
    
}
void Controller::set_unload_destination(Ship* ship_ptr)
{
    string name;
    cin >> name;
    ship_ptr->set_unload_destination(g_Model_ptr->get_island_ptr(name));
}

void Controller::set_dock_island(Ship* ship_ptr)
{
    string name;
    cin >> name;
    ship_ptr->dock(g_Model_ptr->get_island_ptr(name));
}

void Controller::attack(Ship* ship_ptr)
{
    string name;
    cin >> name;
    ship_ptr->attack(g_Model_ptr->get_ship_ptr(name));
}

void Controller::refuel(Ship* ship_ptr)
{
    ship_ptr->refuel();
}

void Controller::stop(Ship* ship_ptr)
{
    ship_ptr->stop();
}
void Controller::stop_attack(Ship* ship_ptr)
{
    ship_ptr->stop_attack();
}