#pragma once

#include <string>
#include <vector>

using namespace std;

struct Location {
    int x = -1;
    int y = -1;
    bool operator==(const Location &rhs);
    friend ostream &operator<<(ostream &out, Location &loc);
};

class Destination {
  public:
    Destination(const string &name, const int &x, const int &y);
    Destination(const string &name, const Location &location);

    Location location();
    bool setLocation(const Location &location);
    bool setLocation(const int &x, const int &y);
    bool setLocation(const vector<int> &vec);

    string name();
    bool setName(const string &name);

    int visited();
    bool setVisited(const int &vis);

    // takes object and turns the location into vector
    vector<int> toVector();

    double calcDistanceMiles(Destination &des2);

    bool isValid();
    bool setIsValid(const bool &valid);

    double distance();
    double setDistance(const double &distance);

    int closestDest(vector<Destination> &list);

    bool operator==(const Destination &rhs);
    bool operator!=(const Destination &rhs);

    friend ostream &operator<<(ostream &out, Destination &dest);

  private:
    Location m_location{-1, -1};
    string m_name = ".";
    bool m_isValid = true;
    int m_visited = -1;
    double m_distance = 0;
};

double fullRoute(vector<Destination> &list);
bool isDuplicate(vector<Destination> &dests, const string &name, const int &x,
                  const int &y);
