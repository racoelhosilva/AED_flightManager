#include <climits>
#include <algorithm>
#include <set>
#include "Manager.h"

/**
 * @brief Extracts the contents of a file into a set of airports and the graph of the flight network.
 * Complexity: O(n), having n as the number of lines in the file.
 * @param fname - Name of the file to be extracted.
 * @return True if the operation was successful.
 */
bool Manager::extractAirports(std::string fname) {
    ifstream input(fname);
    std::string line;

    if (!getline(input, line)) return false;

    std::string code, name, city, country, lats, lgts;
    double lat, lgt;

    getline(input, line);
    do {
        istringstream lineInput(line);
        getline(lineInput, code, ',');
        getline(lineInput, name, ',');
        getline(lineInput, city, ',');
        getline(lineInput, country, ',');
        getline(lineInput, lats, ',');
        getline(lineInput, lgts, '\r');
        lat = stod(lats);
        lgt = stod(lgts);

        Airport airport = Airport(code, name, city, country, lat, lgt);
        airports.insert(airport);
        Vertex<Airport> *vx = flightNet.addVertex(airport);
        airportCodeToVertex[code] = vx;
        airportNameToCode[name] = code;
        cities.insert(city);
        countries.insert(country);
    } while (getline(input, line));
    return true;
}

/**
 * @brief Extracts the contents of a file into a set of Airlines.
 * Complexity: O(n), having n as the number of lines in the file.
 * @param fname - Name of the file to be extracted.
 * @return True if the operation was successful.
 */
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

/**
 * @brief Extracts the contents of a file into the edges of the flight network graph.
 * Complexity: O(n), having n as the number of lines in the file.
 * @param fname - Name of the file to be extracted.
 * @return True if the operation was successful.
 */
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

        Airline airline = *airlines.find(Airline(airlineCode));

        if (airline.getCode() == "noCode") return false;
        flightNet.addEdge(airportCodeToVertex[code1], airportCodeToVertex[code2], airline);
    } while (getline(input, line));
    return true;
}

/**
 * @brief Verifies if there is an airport for a given code.
 * Complexity: O(1).
 * @param code - Code to be assessed.
 * @return True if there exists an airport with the given code.
 */
bool Manager::validateAirport(const std::string &code) {
    return airports.find(Airport(code)) != airports.end();
}

/**
 * @brief Verifies if there is an airline for a given code.
 * Complexity: O(1).
 * @param airline - Code to be assessed.
 * @return True if there exists an airline with the given code.
 */
bool Manager::validateAirline(const std::string &airline) {
    return airlines.find(Airline(airline)) != airlines.end();
}

/**
 * @brief Verifies if there is an airport for a given name.
 * Complexity: O(1).
 * @param name - Name to be assessed.
 * @return True if there exists an airport with the given name.
 */
bool Manager::validateAirportName(const std::string &name) {
    return airportNameToCode.count(name) != 0;
}

/**
 * @brief Verifies if there is a country for a given name.
 * Complexity: O(1).
 * @param country - Name to be assessed.
 * @return True if there exists a country with the given name.
 */
bool Manager::validateCountry(const std::string &country) {
    return countries.find(country) != countries.end();
}

/**
 * @brief Verifies if there is a city for a given name.
 * Complexity: O(1).
 * @param city - Name to be assessed.
 * @return True if there exists a city with the given name.
 */
bool Manager::validateCity(const std::string &city) {
    return cities.find(city) != cities.end();
}

/**
 * @brief Lists all existent airlines.
 * Complexity: O(n), having n as the number of airlines.
 */
void Manager::listAllAirlines() {
    cout << left << setw(6) << "CODE" << '\t'
              << setw(40) << "NAME" << '\t'
              << setw(24) << "CALLSIGN" << '\t'
              << setw(24) << "COUNTRY" << '\n';
    for (Airline airline : airlines) {
        cout << left << setw(6) << airline.getCode() << '\t'
                    << setw(40) << airline.getName() << '\t'
                    << setw(24) << (airline.getCallsign() == "_" ? "" : airline.getCallsign()) << '\t'
                    << setw(24) << airline.getCountry() << '\n';
    }
}

/**
 * @brief Prints the number of existent airlines.
 * Complexity: O(1).
 */
void Manager::numberAirlines() {
    cout << airlines.size() << endl;
}

/**
 * @brief Lists the airlines available at a given airport.
 * Complexity: O(n), having n as the number of airports.
 * @param airport - Code of the airport to be analyzed.
 */
void Manager::listAirlinesAirport(string airport) {
    Airport airport1 = *airports.find(Airport(airport));
    auto v = flightNet.findVertex(airport1);
    unordered_set<Airline, AirlineHash> availableAirlines;
    for (auto e : v->getAdj()) {
        availableAirlines.insert(e.getWeight());
    }

    cout << left << setw(6) << "CODE" << '\t'
         << setw(40) << "NAME" << '\t'
         << setw(24) << "CALLSIGN" << '\t'
         << setw(24) << "COUNTRY" << '\n';
    for (Airline airline : availableAirlines) {
        cout << left << setw(6) << airline.getCode() << '\t'
             << setw(40) << airline.getName() << '\t'
             << setw(24) << (airline.getCallsign() == "_" ? "" : airline.getCallsign()) << '\t'
             << setw(24) << airline.getCountry() << '\n';
    }
}

/**
 * @brief Lists the available airlines for a given country.
 * Complexity: O(n), having n as the number of airlines.
 * @param country - Country to be analyzed.
 */
void Manager::listAirlinesCountry(string country) {
    cout << left << setw(6) << "CODE" << '\t'
         << setw(40) << "NAME" << '\t'
         << setw(24) << "CALLSIGN" << '\t'
         << setw(24) << "COUNTRY" << '\n';
    for (Airline airline : airlines) {
        if (airline.getCountry() == country) {
            cout << left << setw(6) << airline.getCode() << '\t'
                 << setw(40) << airline.getName() << '\t'
                 << setw(24) << (airline.getCallsign() == "_" ? "" : airline.getCallsign()) << '\t'
                 << setw(24) << airline.getCountry() << '\n';
        }
    }
}

/**
 * @brief Prints the information for a given airline.
 * Complexity:  O(1).
 * @param airline - Code of the airline to be analyzed.
 */
void Manager::airlineInfo(string airline) {
    Airline airline1 = *airlines.find(Airline(airline));
    cout << left << setw(6) << "CODE" << '\t'
         << setw(40) << "NAME" << '\t'
         << setw(24) << "CALLSIGN" << '\t'
         << setw(24) << "COUNTRY" << '\n';
    cout << left << setw(6) << airline1.getCode() << '\t'
        << setw(40) << airline1.getName() << '\t'
        << setw(24) << (airline1.getCallsign() == "_" ? "" : airline1.getCallsign()) << '\t'
        << setw(24) << airline1.getCountry() << '\n';
}

/**
 * @brief Lists all existent airports.
 * Complexity: O(n), having n as the number of airports.
 */
void Manager::listAllAirports() {
    for (const Airport& a : airports){
        cout << a.getCode() << "  " << a.getName() << "  " << a.getCity() << " (" << a.getCountry() << ")\n";
    }
}

/**
 * @brief Prints the number of existent airports.
 * Complexity: O(1).
 */
void Manager::numberAirports() {
    cout << airports.size() << '\n';
}

/**
 * @brief Lists the airports in a given city, in a given country.
 * Complexity: O(n), having n as the number of airports.
 * @param country - Name of the country to be analyzed.
 * @param city - Name of the city to be analyzed.
 */
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

/**
 * @brief Lists the first n airports with the most airlines.
 * Complexity: O(n * m), having n as the number of airports and m the number of flights in an airport.
 * @param n - number to be analyzed.
 */
void Manager::listAirportsMostAirlines(int n) {}

/**
 * @brief Lists the first n airports with the most flights.
 * Complexity: O(n), having n as the number of airports.
 * @param n - number to be analyzed.
 */
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

void Manager::listAllFlights() {
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            cout << v->getInfo().getCode() << ' ' << e.getDest()->getInfo().getCode() << ' ' << e.getWeight().getCode() << endl;
        }
    }
}
void Manager::numberFlights(){
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            count++;
        }
    }
    cout << count << endl;
}
void Manager::listFlightsAirline(string airline){
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e.getWeight().getCode() == airline) {
                cout << v->getInfo().getCode() << ' ' << e.getDest()->getInfo().getCode() << ' ' << e.getWeight().getCode() << endl;
            }
        }
    }
}
void Manager::numberFlightsAirline(string airline){
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e.getWeight().getCode() == airline) {
                count++;
            }
        }
    }
    cout << count << endl;
}
void Manager::listFlightsCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    for (auto v : flightNet.getVertexSet()) {
        if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
            for (auto e : v->getAdj()) {
                cout << v->getInfo().getCode() << ' ' << e.getDest()->getInfo().getCode() << ' ' << e.getWeight().getCode() << endl;
            }
        } else if (v->getInfo().getCountry() == country && city.empty()) {
            for (auto e : v->getAdj()) {
                cout << v->getInfo().getCode() << ' ' << e.getDest()->getInfo().getCode() << ' ' << e.getWeight().getCode() << endl;
            }
        }
    }
}
void Manager::numberFlightsCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
            for (auto e : v->getAdj()) {
                count++;
            }
        } else if (v->getInfo().getCountry() == country && city.empty()) {
            for (auto e : v->getAdj()) {
                count++;
            }
        }
    }
    cout << count << endl;
}

void Manager::listCountriesMostAirlines(int n){}
void Manager::listCountriesMostAirports(int n){}
void Manager::listCitiesMostAirports(int n){}

void Manager::articulationPoints(){}
void Manager::diameter(){
    cout << "Calculating diameter (this might take a while)" << endl;
    int diameter = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto w : flightNet.getVertexSet()) {
            w->setVisited(false);
            w->setProcessing(false);
        }
        queue<Vertex<Airport>*> q = {};
        queue<Vertex<Airport>*> next = {};
        q.push(v);
        int distance = 0;
        v->setVisited(true);
        do {
            while (!q.empty()) {
                auto u = q.front();
                q.pop();
                u->setVisited(true);
                u->setProcessing(false);
                for (auto e : u->getAdj()) {
                    if (e.getDest()->isVisited() == false && e.getDest()->isProcessing() == false) {
                        e.getDest()->setProcessing(true);
                        next.push(e.getDest());
                    }
                }
            }
            if (!next.empty()) distance++;
            q = next;
            next = {};
        } while (!q.empty());
        if (distance > diameter) diameter = distance;
    }
    cout << diameter << endl;
}

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
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions) {
    auto It = sources->begin();
    while (It != sources->end()) {
        if (find(airportRestrictions->begin(), airportRestrictions->end(), *It) != airportRestrictions->end() || find(airportRestrictions->begin(), airportRestrictions->end(), airportNameToCode[*It]) != airportRestrictions->end())
            It = sources->erase(It);
        else {
            It++;
        }
    }
    It = destinations->begin();
    while (It != destinations->end()) {
        if (find(airportRestrictions->begin(), airportRestrictions->end(), *It) != airportRestrictions->end() || find(airportRestrictions->begin(), airportRestrictions->end(), airportNameToCode[*It]) != airportRestrictions->end())
            It = destinations->erase(It);
        else {
            It++;
        }
    }

    int minDist = INT_MAX;
    vector<pair<string, string>> minStops;

    for (const string &src : *sources) {
        Airport source = *airports.find(Airport(src));
        for (const string &dest : *destinations) {
            Airport destination = *airports.find(Airport(dest));
            minStopsBetweenAirports(source, destination, airlinePreferences, airlineRestrictions, airportRestrictions);
            if (flightNet.findVertex(destination)->getSteps() < minDist) {
                minStops.clear();
                minDist = flightNet.findVertex(destination)->getSteps();
                minStops.push_back({source.getCode(), destination.getCode()});
            }
            else if (flightNet.findVertex(destination)->getSteps() == minDist) {
                minStops.push_back({source.getCode(), destination.getCode()});
            }
        }
    }

    if (minDist < 1)  {
        cout << "The flight is not possible!" << endl;
        return;
    }

    for (pair<string, string> trip : minStops) {
        cout << trip.first << " -------> " << trip.second << endl;
        queue<Vertex<Airport>*> processing;
        Vertex<Airport>* destination = flightNet.findVertex(*airports.find(Airport(trip.second)));
        cout << destination->getInfo().getCode() << " <-- ";
        for (auto flight : destination->getPrevious()) {
            if (flight.first->getInfo().getCode() != trip.first)
                processing.push(flight.first);
            cout << flight.first->getInfo().getCode() << " / " << flight.second.getWeight().getCode() << " | ";
        }
        cout << endl;
        while (!processing.empty()) {
            Vertex<Airport>* front = processing.front();
            cout << front->getInfo().getCode() << " <-- ";
            for (auto flight : front->getPrevious()) {
                if (flight.first->getInfo().getCode() != trip.first)
                    processing.push(flight.first);
                cout << flight.first->getInfo().getCode() << " / " << flight.second.getWeight().getCode() << " | ";
            }
            cout << endl;
            processing.pop();
        }
    }
    cout << endl;
}

void Manager::minStopsBetweenAirports(const Airport &source, const Airport &destination, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions) {
    vector<pair<string, string>> res;

    for (Vertex<Airport>* airport : flightNet.getVertexSet()) {
        airport->setVisited(false);
        airport->clearPrevious();
        airport->setSteps(INT_MAX);
    }

    Vertex<Airport>* src = flightNet.findVertex(source);
    Vertex<Airport>* dest = flightNet.findVertex(destination);
    src->setVisited(true);
    src->setSteps(0);
    minStopsBFS(src, dest, airlinePreferences, airlineRestrictions, airportRestrictions);
}

void Manager::minStopsBFS(Vertex<Airport>* source, Vertex<Airport>* dest, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions) {
    source->setVisited(true);
    bool flag = false;
    for (const Edge<Airport> &flight : source->getAdj()) {
        if (find(airlineRestrictions->begin(), airlineRestrictions->end(), flight.getWeight().getCode()) == airlineRestrictions->end() && !flight.getDest()->isVisited()
        && find(airportRestrictions->begin(), airportRestrictions->end(), flight.getDest()->getInfo().getCode()) == airportRestrictions->end()) {
            if (flight.getDest()->getSteps() >= source->getSteps() + 1) {
                flight.getDest()->setSteps(source->getSteps() + 1);
                flight.getDest()->addPrevious({source, flight});
            }
        }
        if (flight.getDest() == dest) flag = true;
    }

    if (flag) return;

    for (const Edge<Airport> &flight : source->getAdj()) {
        if (!flight.getDest()->isVisited() && find(airportRestrictions->begin(), airportRestrictions->end(), flight.getDest()->getInfo().getCode()) == airportRestrictions->end() && find(airlinePreferences->begin(), airlinePreferences->end(), flight.getWeight().getCode()) != airlinePreferences->end() && find(airlineRestrictions->begin(), airlineRestrictions->end(), flight.getWeight().getCode()) == airlineRestrictions->end())
            minStopsBFS(flight.getDest(), dest, airlinePreferences, airlineRestrictions, airportRestrictions);
    }
    for (const Edge<Airport> &flight : source->getAdj()) {
        if (!flight.getDest()->isVisited() && find(airportRestrictions->begin(), airportRestrictions->end(), flight.getDest()->getInfo().getCode()) == airportRestrictions->end() && find(airlineRestrictions->begin(), airlineRestrictions->end(), flight.getWeight().getCode()) == airlineRestrictions->end())
            minStopsBFS(flight.getDest(), dest, airlinePreferences, airlineRestrictions, airportRestrictions);
    }
}
