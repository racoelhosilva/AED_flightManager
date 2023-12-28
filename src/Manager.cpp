#include "Manager.h"


bool Manager::extractAirports(std::string fname) {
    ifstream input(fname);
    std::string line;

    if (!getline(input, line)) return false;

    std::string code, name, city, country;
    double lat, lgt;

    getline(input, line);
    do {
        istringstream lineInput(line);
        getline(lineInput, code, ',');
        getline(lineInput, name, ',');
        getline(lineInput, city, ',');
        getline(lineInput, country, ',');
        lineInput >> lat >> new char >> lgt >> new char;

        Airport airport = Airport(code, name, city, country, lat, lgt);
        airports.insert(airport);
        flightNet.addVertex(airport);
    } while (getline(input, line));
    return true;
}

bool Manager::extractAirlines(std::string fname) {
    ifstream input(fname);
    std::string line;

    if (!getline(input, line)) return false;

    std::string code, name, callsign, country;

    getline(input, line);
    do {
        istringstream lineInput(line);
        getline(lineInput, code, ',');
        getline(lineInput, name, ',');
        getline(lineInput, callsign, ',');
        getline(lineInput, country, '\r');


        Airline airline = Airline(code, name, callsign, country);
        airlines.insert(airline);
    } while (getline(input, line));
    return true;
}

bool Manager::extractFlights(std::string fname) {
    ifstream input(fname);
    std::string line;

    if (!getline(input, line)) return false;

    std::string code1, code2, airlineCode;

    getline(input, line);
    do {
        istringstream lineInput(line);
        getline(lineInput, code1, ',');
        getline(lineInput, code2, ',');
        getline(lineInput, airlineCode, '\r');Airport airport1, airport2;
        for (const Airport& a : airports) {
            if (a.getCode() == code1) {
                airport1 = a;
            } else if (a.getCode() == code2) {
                airport2 = a;
            } else if (airport1.getCode() != "noCode" && airport2.getCode() != "noCode") break;
        }
        if (airport1.getCode() == "noCode" || airport2.getCode() == "noCode") return false;
        Airline airline;
        for (const Airline& a : airlines) {
            if (a.getCode() == airlineCode) {
                airline = a;
                break;
            }
        }
        if (airline.getCode() == "noCode") return false;
        flightNet.addEdge(airport1, airport2, airline);
    } while (getline(input, line));
    return true;
}

bool Manager::validateAirport(const std::string &code) {return true;}
bool Manager::validateCountry(const std::string &country) {return true;}
bool Manager::validateAirline(const std::string &airline) {return true;}
bool Manager::validateCity(const std::string &airline) {return true;}


void Manager::listAllAirlines() {}
void Manager::numberAirlines() {}
void Manager::listAirlinesAirport(string airport) {}
void Manager::listAirlinesCountry(string country) {}
void Manager::airlineInfo(string airline) {}

void Manager::listAllAirports() {}
void Manager::numberAirports() {}
void Manager::listAirportsCountryCity(std::string country, std::string city) {}
void Manager::listAirportsMostAirlines(int n){}
void Manager::listAirportsMostFlights(int n){}
void Manager::airportInfo(string airport){}
void Manager::listAirportFlights(string airport){}
void Manager::reachableAirports(string airport, int n){}
void Manager::reachableCities(string airport, int n){}
void Manager::reachableCountries(string airport, int n){}

void Manager::listAllFlights() {}
void Manager::numberFlights(){}
void Manager::listFlightsAirline(string airline){}
void Manager::numberFlightsAirline(string airline){}
void Manager::listFlightsCountryCity(string country, string city){}
void Manager::numberFlightsCountryCity(string country, string city){}

void Manager::listCountriesMostAirlines(int n){}
void Manager::listCountriesMostAirports(int n){}
void Manager::listCitiesMostAirports(int n){}

void Manager::articulationPoints(){}
void Manager::diameter(){}