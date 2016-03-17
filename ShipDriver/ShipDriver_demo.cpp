#include "Island.h"
#include "Utility.h"
#include "Ship_factory.h"
#include "Ship.h"
#include <iostream>

using namespace std;
int main()
{
    Ship* tanker = create_ship("tanker", "Tanker", Point(5, 5));
    Island island1("load", Point(0,0), 1000, 1000);
    Island island2("unload", Point(10, 0), 1000, 0);
    tanker->set_load_destination(&island1);
    tanker->set_unload_destination(&island2);
    int i = 0;
    Ship* attacker = create_ship("attacker", "Cruiser", Point(5, 0));
    attacker->attack(tanker);
    Ship* attacker2 = create_ship("stealer", "Cruiser", Point(5, 5));
    attacker2->attack(attacker);
    cout << endl;
    while (i++  < 20) {
        tanker->update();
        island1.update();
        island2.update();
        attacker->update();
        attacker2->update();
        cout << endl;
    }
}