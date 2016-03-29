#include "View.h"
#include "Geometry.h"
#include "Utility.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

View::View(): size(25), scale(2.), origin(Point(-10, -10))
{
    cout << "View constructed" << endl;
}

View::~View()
{
    cout << "View destructed" << endl;
}

void View::update_location(const std::string& name, Point location)
{
    map[name] = location;
}

void View::update_remove(const std::string& name)
{
    map.erase(name);
}


void View::set_size(int size_)
{
    if (size_ <= 6)
        throw Error("New map size is too small!");
    if (size_ > 30)
        throw Error("New map size is too big!");
    size = size_;
}

void View::set_scale(double scale_)
{
    if (scale_ <= 0.)
        throw Error("New map scale must be positive!");
    scale = scale_;
}

void View::set_origin(Point origin_)
{
    origin = origin_;
}

void View::set_defaults()
{
    size = 25;
    scale = 2.;
    origin = Point(-10, -10);
}

void View::draw() const
{
    cout.setf(ios::fixed, ios::floatfield); // set fixed floating format
    cout.precision(2);
    vector<vector<string>> map_info(size, vector<string>(size, ". "));
    vector<int> axis;
    vector<string> outsides;
    cout << "Display size: " << size << ", scale: " << scale <<  ", origin: " << origin << endl;
    cout.precision(0);
    for (int i = 0; i < size; i += 3) {
        axis.push_back(origin.x + i * scale);
    }
    for_each(map.begin(), map.end(),
             [&](const pair<const string, Point>& pair){
                 int x = 0, y = 0;
                 if (this->get_subscripts(x, y, pair.second)) {
                     string name = map_info[size - 1 -y][x];
                     map_info[size - 1 -y][x] = (name == ". ") ? pair.first.substr(0, 2) :
                     "* ";
                 } else {
                     outsides.push_back(pair.first);
                 }
             });
    if (outsides.size()) {
        for (int i = 0; i < (int)outsides.size() - 1; ++i)
            cout << outsides[i] << ", ";
        cout << outsides[outsides.size() - 1];
        cout << " outside the map" << endl;
    }
    for (int i = 0; i < (int)map_info.size(); ++i) {
        if ((size-1-i) %3 == 0)
            cout << setw(4) << (axis[axis.size() - 1 - i / 3] + origin.y - origin.x) << ' ';
        else
            cout << setw(7);
        for (auto& str: map_info[i]) {
            cout << str;
        }
        cout << endl;
    }
    for (auto i: axis) {
        cout << setw(6) << i;
    }
    cout << endl;
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
}

// Calculate the cell subscripts corresponding to the supplied location parameter,
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View::get_subscripts(int &ix, int &iy, Point location) const
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to appropriate integer after taking the floor
	// floor function will produce the greatest integer that is 
	// smaller than the argument, even for negative values. 
	// So - 0.05 is floored to -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

