#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Ship_factory.h"
#include <iostream>

using namespace std;

int main()
{
    Model model;
//    cout << endl;
//    cout << g_Model_ptr->is_name_in_use("Exxxxxx") << endl;
//    cout << g_Model_ptr->is_ship_present("Exxon") << endl;
//    cout << g_Model_ptr->is_island_present("Exxon") << endl;
//    (g_Model_ptr->get_ship_ptr("Valdez"))->set_load_destination(model.get_island_ptr("Exxon"));
//    model.get_ship_ptr("Valdez")->set_unload_destination(model.get_island_ptr("Bermuda"));
//    g_Model_ptr->update();
//    g_Model_ptr->update();
//    g_Model_ptr->get_ship_ptr("Ajax")->attack(model.get_ship_ptr("Valdez"));
//    g_Model_ptr->update();
//    g_Model_ptr->update();
//    g_Model_ptr->update();
//    g_Model_ptr->update();
//    cout << model.get_time() << endl;
//    model.add_ship(create_ship("attacker", "Cruiser", Point(50,50)));
    View view;
    model.attach(&view);
    view.set_origin(Point(-20, 10));
    view.set_scale(15);
    view.set_size(10);
    view.draw();
    cout << endl;
    return 0;
    
}