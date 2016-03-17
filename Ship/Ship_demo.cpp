#include "Ship.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>

using namespace std;
int main()
{
    Ship new_ship("ship1", Point(100, 101), 1000., 99., 3, 100);
    new_ship.describe();
    new_ship.update();
    new_ship.set_destination_position_and_speed(Point(1, 0), 30.);
    new_ship.describe();
    new_ship.update();
    new_ship.describe();
    new_ship.update();
    new_ship.describe();
    new_ship.update();
    new_ship.describe();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    Island island("land", Point(1,0), 100, 100);
    new_ship.dock(&island);
    new_ship.describe();
    new_ship.refuel();
    new_ship.stop();
    try {
        new_ship.refuel();
    } catch (Error& e) {
        cout << e.what() << endl;
    }
    new_ship.set_course_and_speed(100, 90);
    new_ship.update();
    new_ship.describe();
    try {
        new_ship.dock(&island);
    } catch (Error& e) {
        cout << e.what() << endl;
    }
    new_ship.set_destination_island_and_speed(&island, 90);
    new_ship.update();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    new_ship.update();
    
    return 0;
}