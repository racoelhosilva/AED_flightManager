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
        Graph<Airport> flightNet;
        unordered_set<Airline, AirlineHash, AirlineHash> airlines;
        unordered_set<Airport, AirportHash, AirportHash> airports;

    public:
        bool extractAirports(std::string fname);
        bool extractAirlines(std::string fname);
        bool extractFlights(std::string fname);
        unordered_set<Airport, AirportHash, AirportHash>  getAirports() const {return airports;}
        unordered_set<Airline, AirlineHash, AirlineHash> getAirlines() const {return airlines;}
        Graph<Airport> getFlightNet() const {return flightNet;}

        bool validateAirline(const std::string &airline);
        bool validateAirport(const std::string &code);
        bool validateCountry(const std::string &country);
        bool validateCity(const string &airline);

        void listAllAirlines();
        void numberAirlines();
        void listAirlinesAirport(string airport);
        void listAirlinesCountry(string country);
        void airlineInfo(string airline);

        void listAllAirports();
        void numberAirports();
        void listAirportsCountryCity(string country, string city = "");
        void listAirportsMostAirlines(int n);
        void listAirportsMostFlights(int n);
        void airportInfo(string airport);
        void listAirportFlights(string airport);
        void reachableAirports(string airport, int n=1);
        void reachableCities(string airport, int n=1);
        void reachableCountries(string airport, int n=1);

        void listAllFlights();
        void numberFlights();
        void listFlightsAirline(string airline);
        void numberFlightsAirline(string airline);
        void listFlightsCountryCity(string country, string city = "");
        void numberFlightsCountryCity(string country, string city = "");

        void listCountriesMostAirlines(int n);
        void listCountriesMostAirports(int n);
        void listCitiesMostAirports(int n);

        void articulationPoints();
        void diameter();

};


#endif //AED2324_PRJ2_G15_MANAGER_H
