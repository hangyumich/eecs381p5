#include "Island.h"

int main() {
    Island * land1 = new Island("Exxon", Point(10, 10), 1000, 200);
    land1->update();
    land1->describe();
    land1->accept_fuel(49.2);
    land1->provide_fuel(1343);
    delete land1;
    return 0;
}