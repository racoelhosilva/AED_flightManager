#include <climits>
#include <algorithm>
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
        airportNameToCode[name] = code;
        cities.insert(city);
        countries.insert(country);
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
        countries.insert(country);
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
        getline(lineInput, airlineCode, '\r');
        Airport airport1, airport2;
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

bool Manager::validateAirport(const std::string &code) {
    return airports.find(Airport(code)) != airports.end();
}
bool Manager::validateAirline(const std::string &airline) {
    return airlines.find(Airline(airline)) != airlines.end();
}
bool Manager::validateAirportName(const std::string &name) {
    return airportNameToCode.count(name) != 0;
}
bool Manager::validateCountry(const std::string &country) {
    return countries.find(country) != countries.end();
}
bool Manager::validateCity(const std::string &city) {
    return cities.find(city) != cities.end();
}

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

std::string Manager::getAirportCode(const std::string &name) {
    return airportNameToCode[name];
}

vector<string> Manager::getAirportsCountryCity(string country, string city) {
    vector<string> res;
    for (const Airport &airport : airports) {
        if (airport.getCity() == city && airport.getCountry() == country)
            res.push_back(airport.getCode());
    }
    return res;
}

vector<string> Manager::getAirportsCoordinates(pair<double, double> coords) {
    vector<string> res;
    Coordinate location(coords.first, coords.second);
    double distance = INT_MAX;
    for (const Airport &airport : airports) {
        Coordinate airport_coord(airport.getLatitiude(), airport.getLongitude());
        double distance_to_airport = location.distance(airport_coord);
        if (distance_to_airport < distance) {
            res.clear();
            res.push_back(airport.getCode());
            distance = distance_to_airport;
        }
        else if (distance_to_airport == distance) {
            res.push_back(airport.getCode());
        }
    }
    return res;
}
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string> *airportFilters, vector<string> *airlineFilters) {
    auto It = sources->begin();
    while (It != sources->end()) {
        if (find(airportFilters->begin(), airportFilters->end(), *It) != airportFilters->end() || find(airportFilters->begin(), airportFilters->end(), airportNameToCode[*It]) != airportFilters->end())
            It = sources->erase(It);
        else {
            It++;
        }
    }
    It = destinations->begin();
    while (It != destinations->end()) {
        if (find(airportFilters->begin(), airportFilters->end(), *It) != airportFilters->end() || find(airportFilters->begin(), airportFilters->end(), airportNameToCode[*It]) != airportFilters->end())
            It = destinations->erase(It);
        else {
            It++;
        }
    }

    int minDist = INT_MAX;
    vector<string> minStops;

    for (const string &src : *sources) {
        Airport source = *airports.find(Airport(src));
        for (const string &dest : *destinations) {
            Airport destination = *airports.find(Airport(dest));
            vector<string> stops = minStopsBetweenAirports(source, destination, airportFilters, airlineFilters);
            if (stops.size() < minDist) {
                minDist = stops.size();
                minStops = stops;
            }
        }
    }

    if (minStops.size() < 2)  {
        cout << "The flight is not possible!" << endl;
        return;
    }
    cout << minStops[0];
    for (int i = 1; i < minStops.size(); i++) {
        cout << " <----- " << minStops[i];
    }
    cout << endl;
}

vector<string> Manager::minStopsBetweenAirports(const Airport &source, const Airport &destination, vector<string> *airportFilters, vector<string>* airlineFilters) {
    vector<string> res;

    for (Vertex<Airport>* airport : flightNet.getVertexSet()) {
        airport->setVisited(false);
        airport->setPrevious(nullptr);
        airport->setSteps(INT_MAX);
    }

    Vertex<Airport>* src = flightNet.findVertex(source);
    Vertex<Airport>* dest = flightNet.findVertex(destination);
    src->setVisited(true);
    src->setSteps(0);
    minStopsBFS(src, dest, airportFilters, airlineFilters);

    while (dest != src) {
        res.push_back(dest->getInfo().getCode());
        dest = dest->getPrevious();
    }
    res.push_back(src->getInfo().getCode());
    return res;
}

void Manager::minStopsBFS(Vertex<Airport>* source, Vertex<Airport>* dest, vector<string> *airportFilters, vector<string>* airlineFilters) {
    source->setVisited(true);
    for (const Edge<Airport> &flight : source->getAdj()) {
        if (find(airlineFilters->begin(), airlineFilters->end(), flight.getWeight().getCode()) == airlineFilters->end() && !flight.getDest()->isVisited()
        && find(airportFilters->begin(), airportFilters->end(), flight.getDest()->getInfo().getCode()) == airportFilters->end()) {
            if (flight.getDest()->getSteps() > source->getSteps() + 1) {
                flight.getDest()->setSteps(source->getSteps() + 1);
                flight.getDest()->setPrevious(source);
            }
        }
        if (flight.getDest() == dest) return;
    }
    for (const Edge<Airport> &flight : source->getAdj()) {
        if (!flight.getDest()->isVisited() && find(airportFilters->begin(), airportFilters->end(), flight.getDest()->getInfo().getCode()) == airportFilters->end())
            minStopsBFS(flight.getDest(), dest, airportFilters, airlineFilters);
    }
}
