#include "Cruiser.h"
#include "Ship.h"
#include "Utility.h"
#include <iostream>

using std::cout;
using std::endl;

// initialize, then output constructor message
Cruiser::Cruiser(const std::string& name_, Point position_)
:Ship(name_, position_, 1000., 20, 10, 6), firepower(3), range(15), cruiser_state(not_attacking), target(nullptr)
{
    cout << "Cruiser " << get_name() << " constructed" << endl;
}

// output destructor message
Cruiser::~Cruiser()
{
    cout << "Cruiser " << get_name() << " destructed" << endl;
}

// perform Cruiser-specific behavior
void Cruiser::update()
{
    Ship::update();
    if (cruiser_state == not_attacking)
        return;
    if (!is_afloat() || !target->is_afloat()) {
        stop_attack();
        return;
    }
    cout << get_name() << " is attacking" << endl;
    double distance_to_target = cartesian_distance(get_location(), target->get_location());
    if (distance_to_target <= range) {
        cout << get_name() << " fires" << endl;
        target->receive_hit(firepower, this);
        return;
    }
    cout << get_name() << " target is out of range" << endl;
    stop_attack();
}

void Cruiser::describe() const
{
    cout << "\nCruiser ";
    Ship::describe();
    if (cruiser_state == CruiserState::attacking) {
        cout << "Attacking " << target->get_name() << endl;
    }
}

// respond to an attack
void Cruiser::receive_hit(int hit_force, Ship* attacker_ptr)
{
    Ship::receive_hit(hit_force, attacker_ptr);
    if (cruiser_state == not_attacking || attacker_ptr != target) {
        attack(attacker_ptr);
    }
}

// start an attack on a target ship
void Cruiser::attack(Ship* target_ptr_)
{
    if (!is_afloat()) {
        throw Error("Cannot attack!");
    }
    if (target_ptr_ == this) {
        throw Error("Cannot attack itself!");
    }
    if (cruiser_state == CruiserState::attacking && target_ptr_ == target) {
        throw Error("Already attacking this target!");
    }
    if ((cruiser_state == CruiserState::attacking && target_ptr_ != target) || cruiser_state == not_attacking) {
        target = target_ptr_;
        cruiser_state = CruiserState::attacking;
        cout << get_name() << " will attack " << target->get_name() << endl;
    }
}
void Cruiser::stop_attack()
{
    if (cruiser_state == CruiserState::not_attacking) {
        throw Error("Was not attacking!");
    }
    cout << get_name() << " stopping attack" << endl;
    cruiser_state = CruiserState::not_attacking;
    target = nullptr;
}