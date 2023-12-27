#ifndef AED2324_PRJ2_G15_MANAGER_H
#define AED2324_PRJ2_G15_MANAGER_H

#include "Airport.h"
#include "Airline.h"
#include "Graph.h"
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

struct AirlineHash {
    int operator() (const Airline &airline) const {
        return std::hash<std::string>()(airline.getCode()) ;
    }

    bool operator() (const Airline &airline1, const Airline &airline2) const {
        return airline1.getCode() == airline2.getCode();
    }
};

class Manager {
    private:
        bool op;
        std::list<Airport> airports;
        Graph<Airport> flightNet;
        unordered_set<Airline, AirlineHash, AirlineHash> airlines;

    public:
        Manager() {op = true;}
        bool extractAirports(std::string fname);
        bool extractAirlines(std::string fname);
        bool extractFlights(std::string fname);
        std::list<Airport> getAirports() const {return airports;}
        unordered_set<Airline, AirlineHash, AirlineHash> getAirlines() const {return airlines;}
        Graph<Airport> getFlightNet() const {return flightNet;}
};


#endif //AED2324_PRJ2_G15_MANAGER_H
