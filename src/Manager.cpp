#include <set>
#include "Manager.h"


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
        countryToAirportCount[country]++;
        countryCityToAirportCount[make_pair(country, city)] += 1;
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
        countryToAirlineCount[country]++;
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

        Airline airline = *airlines.find(Airline(airlineCode));

        if (airline.getCode() == "noCode") return false;
        flightNet.addEdge(airportCodeToVertex[code1], airportCodeToVertex[code2], airline);
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
void Manager::numberAirlines() {
    cout << airlines.size() << endl;
}
void Manager::listAirlinesAirport(string airport) {
    Airport airport1 = *airports.find(Airport(airport));
    auto v = flightNet.findVertex(airport1);
    set<Airline> availableAirlines;
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
void Manager::listAirportsMostAirlines(int n) {
    priority_queue<pair<int, string>> airportAirlineCount;
    for (auto a : flightNet.getVertexSet()){
        unordered_set<string> airportAirlines;
        for (const Edge<Airport> &e : a->getAdj()){
            airportAirlines.insert(e.getWeight().getCode());
        }
        airportAirlineCount.push({airportAirlines.size(), a->getInfo().getCode()});
    }
    for (int pos = 1; pos <= min(n, int(airports.size())); pos++){
        pair<int, string> current = airportAirlineCount.top();
        cout << pos << ")   " << current.second << " (" << current.first << ")\n";
        airportAirlineCount.pop();
    }
}
void Manager::listAirportsMostFlights(int n){
    priority_queue<pair<int, string>> airportFlightCount;
    for (auto a : flightNet.getVertexSet()){
        airportFlightCount.push({a->getAdj().size(), a->getInfo().getCode()});
    }
    for (int pos = 1; pos <= min(n, int(airports.size())); pos++){
        pair<int, string> current = airportFlightCount.top();
        cout << pos << ")   " << current.second << " (" << current.first << ")\n";
        airportFlightCount.pop();
    }
}
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

void Manager::listCountriesMostAirlines(int n){
    priority_queue<pair<int, string>> countryAirlineCount;
    for (const string &country : countries){
        countryAirlineCount.push({countryToAirlineCount[country], country});
    }
    for (int pos = 1; pos <= min(n, int(countries.size())); pos++){
        pair<int, string> current = countryAirlineCount.top();
        cout << pos << ")   " << current.second << " (" << current.first << ")\n";
        countryAirlineCount.pop();
    }
}
void Manager::listCountriesMostAirports(int n){
    priority_queue<pair<int, string>> countryAirportCount;
    for (const string &country : countries){
        countryAirportCount.push({countryToAirportCount[country], country});
    }
    for (int pos = 1; pos <= min(n, int(countries.size())); pos++){
        pair<int, string> current = countryAirportCount.top();
        cout << pos << ")   " << current.second << " (" << current.first << ")\n";
        countryAirportCount.pop();
    }
}
void Manager::listCitiesMostAirports(int n){
    priority_queue<pair<int, pair<string,string>>> countryCityAirportCount;
    for (const string &country : countries){
        for (const string &city : cities){
            pair<string, string> countryCity = make_pair(country, city);
            countryCityAirportCount.push({countryCityToAirportCount[countryCity], countryCity});
        }
    }
    for (int pos = 1; pos <= min(n, int(cities.size())); pos++){
        pair<int, pair<string,string>> current = countryCityAirportCount.top();
        cout << pos << ")   " << current.second.first << " " << current.second.second << " (" << current.first << ")\n";
        countryCityAirportCount.pop();
    }
}

void Manager::articulationPoints(){
    auto articulationPoints = flightNet.articulationPoints();
    unordered_set<Airport, AirportHash, AirportHash> s;
    for (const Airport &v : articulationPoints){
        s.insert(v);
    }
    for (const Airport &ap : s){
        cout << ap.getCode() << "    " << ap.getName() << '\n';
    }
    cout << s.size() << '\n';
}
void Manager::diameter(){}

std::string Manager::getAirportCode(const std::string &name) {
    return airportNameToCode[name];
}

vector<string> Manager::getAirportsCountryCity(string country, string city) {return vector<string>();}
vector<string> Manager::getAirportsCoordinates(pair<double, double> coords) {return vector<string>();}
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string> *airportFlters, vector<string> *airlineFilters) {}