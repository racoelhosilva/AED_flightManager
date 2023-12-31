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
#include <unordered_set>
#include <unordered_map>

// Text Formatting
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define FAINT       "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define INVERT      "\033[7m"
#define HIDDEN      "\033[8m"

// Text Colors (Foreground)
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Background Colors
#define BGBLACK     "\033[40m"
#define BGRED       "\033[41m"
#define BGGREEN     "\033[42m"
#define BGYELLOW    "\033[43m"
#define BGBLUE      "\033[44m"
#define BGMAGENTA   "\033[45m"
#define BGCYAN      "\033[46m"
#define BGWHITE     "\033[47m"

// Cursor Control
#define HOME        "\033[H"
#define CLEAR       "\033[2J"
#define CLEARLINE   "\033[K"
#define UP          "\033[A"
#define DOWN        "\033[B"
#define RIGHT       "\033[C"
#define LEFT        "\033[D"


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
        return airport1.getCode() == airport2.getCode();
    }
};

template <class T1, class T2>
struct hash<std::pair<T1, T2>> {
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        // Combine the hash values using bitwise XOR
        return hash1 ^ hash2;
    }
};

class Manager {
private:
    Graph<Airport> flightNet;
    unordered_set<Airline, AirlineHash, AirlineHash> airlines;
    unordered_set<Airport, AirportHash, AirportHash> airports;
    unordered_set<std::string> cities;
    unordered_set<std::string> countries;

    unordered_map<string, string> airportNameToCode;
    unordered_map<string, Vertex<Airport> *> airportCodeToVertex;
    unordered_map<string, int> countryToAirportCount;
    unordered_map<string, int> countryToAirlineCount;
    unordered_map<pair<string, string>, int> countryCityToAirportCount;

public:
    bool extractAirports(std::string fname);
    bool extractAirlines(std::string fname);
    bool extractFlights(std::string fname);
    unordered_set<Airport, AirportHash, AirportHash>  getAirports() const {return airports;}
    unordered_set<Airline, AirlineHash, AirlineHash> getAirlines() const {return airlines;}
    Graph<Airport> getFlightNet() const {return flightNet;}

    bool validateAirline(const std::string &airline);
    bool validateAirport(const std::string &code);
    bool validateAirportName(const std::string &name);
    bool validateCountry(const std::string &country);
    bool validateCity(const string &city, const string &country);

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
    void listArrivalsCountryCity(string country, string city = "");
    void numberArrivalsCountryCity(string country, string city = "");
    void listDeparturesCountryCity(string country, string city = "");
    void numberDeparturesCountryCity(string country, string city = "");

    void listCountriesMostAirlines(int n);
    void listCountriesMostAirports(int n);
    void listCitiesMostAirports(int n);

    void articulationPoints();
    void diameter();
    void longestPaths();

    std::string getAirportCode(const std::string &name);
    vector<string> getAirportsCountryCity(string country, string city);
    vector<string> getAirportsCountry(string country);
    vector<string> getAirportsCoordinates(Coordinate coords);

    void bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions);
    int minDistanceBFS(string &src, string &dest, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions);
    void printPath(Vertex<Airport> *v);
    vector<vector<pair<string,string>>> allPaths;

    void bestFlightOptions(vector<string> *sources, vector<string> *destinations, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions);
    int minDistanceBFS2(string &src, string &dest, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions);
    void reconstructPaths(Vertex<Airport> *dest, int minDist);
    vector<string> path;
    vector<vector<string>> paths;

private:
    void printCount(int number, string text);
    void printDouble(double number, string text);
    void printListHeader(string text);
    void printListValue(string text);
    void printOrderedValue(int pos, int count, string text);
    void printOrderedValueAirport(int pos, int count, string text1, string text2);
    void printOrderedValueCity(int pos, int count, string text1, string text2);
    void printAirlineHeader();
    void printAirline(const Airline &airline);
    void printAirlineFooter();
    void printAirportHeader();
    void printAirport(const Airport &airport);
    void printAirportFooter();
    void printDepartureHeader();
    void printDeparture(string source, const Airport &dest, string airline, double distance);
    void printDepartureFooter();
    void printArrivalHeader();
    void printArrival(const Airport &source, string dest, string airline, double distance);
    void printArrivalFooter();
    void printFlightHeader();
    void printFlight(const Airport &source, const Airport &dest, string airline, double distance);
    void printFlightFooter();

    void printAllPaths();

    void printPaths(vector<string>* airlinePreferences, vector<string> *airlineRestrictions);
    void printFlightAirlines(const string &currentAirport, const string &nextAirport, vector<string>* airlinePreferences, vector<string> *airlineRestrictions);


};


#endif //AED2324_PRJ2_G15_MANAGER_H
