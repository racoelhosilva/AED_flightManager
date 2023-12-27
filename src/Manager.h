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
        return std::hash<std::string>()(airline.getCode());
    }

    bool operator() (const Airline &airline1, const Airline &airline2) const {
        return airline1.getCode() == airline2.getCode();
    }
};

struct AirportHash {
    int operator() (const Airport &airport) const {
        return std::hash<std::string>()(airport.getCode());
    }

    bool operator() (const Airport &airport1, const Airport &airport2) const {
        return airport1 == airport2;
    }
};
class Manager {
    private:
        bool op;
        Graph<Airport> flightNet;
        unordered_set<Airline, AirlineHash, AirlineHash> airlines;
        unordered_set<Airport, AirportHash, AirportHash> airports;

    public:
        Manager() {op = true;}
        bool extractAirports(std::string fname);
        bool extractAirlines(std::string fname);
        bool extractFlights(std::string fname);
        unordered_set<Airport, AirportHash, AirportHash>  getAirports() const {return airports;}
        unordered_set<Airline, AirlineHash, AirlineHash> getAirlines() const {return airlines;}
        Graph<Airport> getFlightNet() const {return flightNet;}
};


#endif //AED2324_PRJ2_G15_MANAGER_H
