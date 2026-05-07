#include "destination.h"
#include "files.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Equality operator overload for Location objects
bool Location::operator==(const Location &rhs) {
    return (this->x == rhs.x && this->y == rhs.y);
}

ostream &operator<<(ostream &out, Location &loc) {
    out << '(' << to_string(loc.x) << ", "
        << to_string(loc.y) << ')';
    return out;
}

// Destination Contstructor, taking (string) name, (int) x, and (int) y
Destination::Destination(const string &name, const int &x, const int &y) {
    Destination::setName(name);
    Destination::setLocation(x, y);
}

// Overloaded Destination Constructor, taking (string) name, and (Location)
// location
Destination::Destination(const string &name, const Location &location) {
    Destination::setName(name);
    Destination::setLocation(location);
}

// Updates this Destination's Location via an argument Location, and returns
// true if successful. Returns false and sets this Destination's x or y to -1 if
// argument Location's x or y is over 1000.
bool Destination::setLocation(const Location &location) {
    this->m_location = location;
    if (location.x < 0 || location.x > 1000) {
        this->m_location.x = -1;
        this->setIsValid(false);
    }
    if (location.y < 0 || location.y > 1000) {
        this->m_location.y = -1;
        this->setIsValid(false);
    }
    if (!this->isValid())
        return false;

    return true;
}

// Overload, updates this Destination's Location via (int) x and (int )y, and
// returns true if successful. Returns false and sets this Destination's x or y
// to -1 if argument x or y is over 1000.
bool Destination::setLocation(const int &x, const int &y) {
    this->m_location = {x, y};
    if (x < 0 || x > 1000) {
        this->m_location.x = -1;
        this->setIsValid(false);
    }
    if (y < 0 || y > 1000) {
        this->m_location.y = -1;
        this->setIsValid(false);
    }
    if (!this->isValid())
        return false;

    return true;
}

// Overload, updates this Destination's Location via a 2-length vector<int>, and
// returns true if successful. Returns false and sets this Destination's x or y
// to -1 if argument x or y is over 1000. Early returns false if argument vector
// is not 2 in length.
bool Destination::setLocation(const vector<int> &vec) {
    if (vec.size() != 2) {
        return false;
    }

    const int x = vec[0];
    const int y = vec[1];

    this->m_location = {x, y};
    if (x < 0 || x > 1000) {
        this->m_location.x = -1;
        this->setIsValid(false);
    }
    if (y < 0 || y > 1000) {
        this->m_location.y = -1;
        this->setIsValid(false);
    }
    if (!this->isValid())
        return false;

    return true;
}

// Returns this Destination's Location
Location Destination::location() { return this->m_location; }

// Updates this Destination's visited flag.
// Early returns false if argument int is negative.
bool Destination::setVisited(const int &vis) {
    if (vis < 0)
        return false;
    this->m_visited = vis;
    return true;
}

// Returns this Destination's visited flag
int Destination::visited() { return this->m_visited; }

// Updates this Destination's valid bool
bool Destination::setIsValid(const bool &valid) {
    this->m_isValid = valid;
    return true;
}

// Returns this Destination's valid bool
bool Destination::isValid() { return this->m_isValid; }

// Returns this Destination's name string
string Destination::name() { return this->m_name; }

// Updates this Destination's name string
bool Destination::setName(const string &name) {
    this->m_name = name;
    return true;
}

// Returns the distance between this Destination and an argument Destination
double Destination::calcDistanceMiles(Destination &dest) {
    // Unintended but more accurate solution
    //return (sqrt(pow((m_location.x - dest.location().x), 2) + pow((m_location.y - dest.location().y), 2)));
    return abs(m_location.x - dest.location().x) + abs(m_location.y - dest.location().y);
}

// Returns this Destination's Location coordinates as a vector<int>
vector<int> Destination::toVector() {
    return (vector<int>{m_location.x, m_location.y});
}

// Equality operator overload for Destination objects
bool Destination::operator==(const Destination &rhs) {
    return this->m_name == rhs.m_name && this->m_location == rhs.m_location;
}

// Inequality operator overload for Destination objects
bool Destination::operator!=(const Destination &rhs) { return !(*this == rhs); }

ostream &operator<<(ostream &out, Destination &dest) {
    out << dest.name() << File::g_VALUE_DELIMITER
        << to_string(dest.location().x) << File::g_VALUE_DELIMITER
        << to_string(dest.location().y) << File::g_DESTINATION_DELIMITER;
    return out;
}

double Destination::distance() { return this->m_distance; }
double Destination::setDistance(const double &distance) {
    if (distance > -1) {
        this->m_distance = distance;
    }
    return this->m_distance;
}

// Starting from this Distance, loops through an argument vector
// Destinations and returns the vector index of the closest Destination
int Destination::closestDest(vector<Destination> &list) {
    int closestVisited = -1;
    double smallestDistance = INFINITY;
    for (size_t i = 0; i < list.size(); i += 1) {

        // self check
        if (*this == list[i])
            continue;

        // valid / visited before check
        if (list[i].visited() != -1)
            continue;

        // passed, now we can check the distance
        double testDistance = this->calcDistanceMiles(list[i]);

        // check to see if smaller than
        if (testDistance < smallestDistance) {
            closestVisited = i;
            smallestDistance = testDistance;
        }
    }
    return closestVisited;
}

// Starting from the first index of an argument vector<Destination>, loops
// through a vector<Destination> and calculates the shortest route between all
// Destinations in argument vector
double fullRoute(vector<Destination> &list) {
    int currentPointIndex = 0;
    double totalDistance = 0;

    for (size_t i = 0; i < list.size(); i += 1) {
        // set the visited #
        list[currentPointIndex].setVisited(i);

        // get next point to check (closest to current point)
        int nextPointIndex = list[currentPointIndex].closestDest(list);

        // if no destination then go back home
        if (nextPointIndex == -1)
            nextPointIndex = 0;

        // add to total distance and set the m_distance var of the current
        // destination
        totalDistance += list[currentPointIndex].setDistance(
            list[currentPointIndex].calcDistanceMiles(list[nextPointIndex]));

        // setup next point
        currentPointIndex = nextPointIndex;
    }

    return totalDistance;
}

bool isDuplicate(vector<Destination> &dests, const string &name, const int &x,
                  const int &y) {
    for (Destination &dest : dests) {
        if (x == dest.location().x && y == dest.location().y) {
            cout << " \033[31mDuplicate coordinates found! Location: (" << x << ", " << y << ')' << '\n'
                 << " \033[31mUsing original \"" << dest.name() << "\", skipping \"" << name << '"' << "\033[0m" 
                 << '\n';
            return true;
        }
    }
    return false;
}
