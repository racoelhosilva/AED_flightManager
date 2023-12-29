#ifndef AED2324_PRJ2_G15_MANAGER_H
#define AED2324_PRJ2_G15_MANAGER_H

#include "Airport.h"
#include "Airline.h"
#include "Graph.h"
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

class Manager {
    private:
        std::list<Airport> airports;
        std::list<Airline> airlines;
        Graph<Airport> flightNet;
    public:
        bool extractAirports(std::string fname);
        bool extractAirlines(std::string fname);
        bool extractFlights(std::string fname);
        std::list<Airport> getAirports() const {return airports;}
        std::list<Airline> getAirlines() const {return airlines;}
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
