#ifndef AED2324_PRJ2_G15_MANAGER_H
#define AED2324_PRJ2_G15_MANAGER_H

#include "Airport.h"
#include "Airline.h"
#include "Graph.h"
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

class Manager {
    private:
        bool op;
        std::list<Airport> airports;
        std::list<Airline> airlines;
        Graph<Airport> flightNet;
    public:
        Manager() {op = true;}
        bool extractAirports(std::string fname);
        bool extractAirlines(std::string fname);
        bool extractFlights(std::string fname);
        std::list<Airport> getAirports() const {return airports;}
        std::list<Airline> getAirlines() const {return airlines;}
        Graph<Airport> getFlightNet() const {return flightNet;}
};


#endif //AED2324_PRJ2_G15_MANAGER_H
