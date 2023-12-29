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

void Manager::listAllAirports() {
    for (const Airport& a : airports){
        cout << a.getCode() << "  " << a.getName() << "  " << a.getCity() << " (" << a.getCountry() << ")\n";
    }
}
void Manager::numberAirports() {
    cout << airports.size() << '\n';
}
void Manager::listAirportsCountryCity(std::string country, std::string city) {
    bool cityExists = cities.find(city) != cities.end();
    for (const Airport &a : airports){
        if (a.getCountry() == country){
            if (cityExists && a.getCity() != city){
                continue;
            }
            else {
                cout << a.getCode() << "  " << a.getName() << "  " << a.getCity() << " (" << a.getCountry() << ")\n";
            }
        }
    }
}
void Manager::listAirportsMostAirlines(int n) {}
void Manager::listAirportsMostFlights(int n){}
void Manager::airportInfo(string airport){
    auto a = airports.find(Airport(airport));
    cout << a->getCode() << "  " << a->getName() << "  " << a->getCity() << " (" << a->getCountry() << ")\n";
}
void Manager::listAirportFlights(string airport) {
    for (const Edge<Airport> &e : flightNet.findVertex(*airports.find(Airport(airport)))->getAdj()){
        cout << airport << " --- (" << e.getWeight().getCode() << ") --> "<< e.getDest()->getInfo().getCode() << '\n';
    }
}
void Manager::reachableAirports(string airport, int n){
    for (auto a : flightNet.getVertexSet()){
        a->setVisited(false);
    }

    vector<Airport> airportsReached;
    queue<pair<Airport, int>> remaining;

    remaining.push({*airports.find(Airport(airport)), n});

    while (!remaining.empty()) {
        Airport a = remaining.front().first;
        int layoversRemaining = remaining.front().second - 1;
        remaining.pop();
        airportsReached.push_back(a);

        if (layoversRemaining >= 0){
            for (const Edge<Airport> &e : flightNet.findVertex(a)->getAdj()){
                if (!e.getDest()->isVisited()){
                    remaining.push({e.getDest()->getInfo(), layoversRemaining});
                    e.getDest()->setVisited(true);
                }
            }
        }
    }
    for (auto a : airportsReached){
        cout << a.getCode() << "  " << a.getName() << "  " << a.getCity() << " (" << a.getCountry() << ")\n";
    }
    cout << airportsReached.size() << '\n';
}
void Manager::reachableCities(string airport, int n){
    for (auto a : flightNet.getVertexSet()){
        a->setVisited(false);
    }

    unordered_set<string> citiesReached;
    queue<pair<Airport, int>> remaining;

    remaining.push({*airports.find(Airport(airport)), n});

    while (!remaining.empty()) {
        Airport a = remaining.front().first;
        int layoversRemaining = remaining.front().second - 1;
        remaining.pop();
        citiesReached.insert(a.getCity());

        if (layoversRemaining >= 0){
            for (const Edge<Airport> &e : flightNet.findVertex(a)->getAdj()){
                if (!e.getDest()->isVisited()){
                    if (citiesReached.count(e.getDest()->getInfo().getCity()) == 0){
                        remaining.push({e.getDest()->getInfo(), layoversRemaining});
                    }
                    e.getDest()->setVisited(true);
                }
            }
        }
    }
    for (auto c : citiesReached){
        cout << c << "\n";
    }
    cout << citiesReached.size() << '\n';
}
void Manager::reachableCountries(string airport, int n){
    for (auto a : flightNet.getVertexSet()){
        a->setVisited(false);
    }

    unordered_set<string> countriesReached;
    queue<pair<Airport, int>> remaining;

    remaining.push({*airports.find(Airport(airport)), n});

    while (!remaining.empty()) {
        Airport a = remaining.front().first;
        int layoversRemaining = remaining.front().second - 1;
        remaining.pop();
        countriesReached.insert(a.getCountry());

        if (layoversRemaining >= 0){
            for (const Edge<Airport> &e : flightNet.findVertex(a)->getAdj()){
                if (!e.getDest()->isVisited()){
                    if (countriesReached.count(e.getDest()->getInfo().getCountry()) == 0){
                        remaining.push({e.getDest()->getInfo(), layoversRemaining});
                    }
                    e.getDest()->setVisited(true);
                }
            }
        }
    }
    for (auto c : countriesReached){
        cout << c << "\n";
    }
    cout << countriesReached.size() << '\n';
}

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

vector<string> Manager::getAirportsCountryCity(string country, string city) {return vector<string>();}
vector<string> Manager::getAirportsCoordinates(pair<double, double> coords) {return vector<string>();}
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string> *airportFlters, vector<string> *airlineFilters) {}