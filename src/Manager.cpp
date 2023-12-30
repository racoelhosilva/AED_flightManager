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
    printAirlineHeader();
    for (Airline airline : airlines) {
        printAirline(airline);
    }
    printAirlineFooter();
    printCount(airlines.size(), "Total Number of Airlines: ");
}

void Manager::numberAirlines() {
    printCount(airlines.size(), "Total Number of Airlines: ");
}

void Manager::listAirlinesAirport(string airport) {
    Vertex<Airport> *v = airportCodeToVertex[airport];
    unordered_set<Airline, AirlineHash, AirlineHash> availableAirlines;
    for (const Edge<Airport> &e : v->getAdj()) {
        availableAirlines.insert(e.getWeight());
    }

    printAirlineHeader();
    for (const Airline &airline : availableAirlines) {
        printAirline(airline);
    }
    printAirlineFooter();
    printCount(availableAirlines.size(), "Number of Airlines: ");
}

void Manager::listAirlinesCountry(string country) {
    printAirlineHeader();
    int count = 0;
    for (const Airline &airline : airlines) {
        if (airline.getCountry() == country) {
            printAirline(airline);
            count++;
        }
    }
    printAirlineFooter();
    printCount(count, "Number of Airlines: ");
}

void Manager::airlineInfo(string airline) {
    printAirlineHeader();
    printAirline(*airlines.find(Airline(airline)));
    printAirlineFooter();
}

void Manager::listAllAirports() {
    printAirportHeader();
    for (const Airport& airport : airports){
        printAirport(airport);
    }
    printAirportFooter();
    printCount(airports.size(), "Total Number of Airports:");
}
void Manager::numberAirports() {
    printCount(airports.size(), "Total Number of Airports:");
}
void Manager::listAirportsCountryCity(std::string country, std::string city) {
    bool cityExists = cities.find(city) != cities.end();
    printAirportHeader();
    int count = 0;
    for (const Airport &a : airports){
        if (a.getCountry() == country){
            if (cityExists && a.getCity() != city){
                continue;
            }
            else {
                printAirport(a);
                count++;
            }
        }
    }
    printAirportFooter();
    printCount(count, "Number of Airports:");
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
    printListHeader("Airports with most Airlines:");
    for (int pos = 1; pos <= min(n, int(airports.size())); pos++){
        pair<int, string> current = airportAirlineCount.top();
        printOrderedValueAirport(pos, current.first, current.second, airports.find(Airport(current.second))->getName());
        airportAirlineCount.pop();
    }
}
void Manager::listAirportsMostFlights(int n){
    priority_queue<pair<int, string>> airportFlightCount;
    for (auto a : flightNet.getVertexSet()){
        airportFlightCount.push({a->getAdj().size(), a->getInfo().getCode()});
    }
    printListHeader("Airports with most Flights:");
    for (int pos = 1; pos <= min(n, int(airports.size())); pos++){
        pair<int, string> current = airportFlightCount.top();
        printOrderedValueAirport(pos, current.first, current.second, airports.find(Airport(current.second))->getName());
        airportFlightCount.pop();
    }
}

void Manager::airportInfo(const string airport){
    printAirportHeader();
    printAirport(*airports.find(Airport(airport)));
    printAirportFooter();
}

void Manager::listAirportFlights(string airport) {
    printDepartureHeader();
    for (const Edge<Airport> &e : airportCodeToVertex[airport]->getAdj()){
        printDeparture(airport, e.getDest()->getInfo(), e.getWeight().getCode());
    }
    printDepartureFooter();
    printCount(airportCodeToVertex[airport]->getAdj().size(), "Number of Flights:");
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
    printAirportHeader();
    for (const Airport &a : airportsReached){
        printAirport(a);
    }
    printAirportFooter();
    printCount(airportsReached.size(), "Number of Airports Reached:");
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
    printListHeader("Cities:");
    for (const string &city : citiesReached){
        printListValue(city);
    }
    printCount(citiesReached.size(), "Number of Cities Reached:");
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
    printListHeader("Countries:");
    for (const string &country : countriesReached){
        printListValue(country);
    }
    printCount(countriesReached.size(), "Number of Countries Reached:");
}

void Manager::listAllFlights() {
    printFlightHeader();
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            printFlight(v->getInfo(), e.getDest()->getInfo(), e.getWeight().getCode());
            count++;
        }
    }
    printFlightFooter();
    printCount(count, "Total Number of Flights:");
}
void Manager::numberFlights(){
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            count++;
        }
    }
    printCount(count, "Total Number of Flights:");
}
void Manager::listFlightsAirline(string airline){
    int count = 0;
    printFlightHeader();
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e.getWeight().getCode() == airline) {
                count++;
                printFlight(v->getInfo(), e.getDest()->getInfo(), e.getWeight().getCode());
            }
        }
    }
    printFlightFooter();
    printCount(count, "Total Number of Flights:");
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
    printCount(count, "Number of Flights:");
}
void Manager::listArrivalsCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    printArrivalHeader();
    for (auto w : flightNet.getVertexSet()) {
        for (auto e : w->getAdj()){
            Vertex<Airport> *v = e.getDest();
            if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
                printArrival(w->getInfo(), v->getInfo().getCode(), e.getWeight().getCode());
                count++;
            } else if (v->getInfo().getCountry() == country && city.empty()) {
                printArrival(w->getInfo(), v->getInfo().getCode(), e.getWeight().getCode());
                count++;
            }
        }
    }
    printArrivalFooter();
    printCount(count, "Number of Arrivals:");
}
void Manager::numberArrivalsCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    for (auto w : flightNet.getVertexSet()) {
        for (auto e : w->getAdj()){
            Vertex<Airport> *v = e.getDest();
            if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
                count++;
            } else if (v->getInfo().getCountry() == country && city.empty()) {
                count++;
            }
        }
    }
    printCount(count, "Number of Arrivals:");
}
void Manager::listDeparturesCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    printDepartureHeader();
    for (auto v : flightNet.getVertexSet()) {
        if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
            for (auto e : v->getAdj()) {
                printDeparture(v->getInfo().getCode(), e.getDest()->getInfo(), e.getWeight().getCode());
                count++;
            }
        } else if (v->getInfo().getCountry() == country && city.empty()) {
            for (auto e : v->getAdj()) {
                printDeparture(v->getInfo().getCode(), e.getDest()->getInfo(), e.getWeight().getCode());
                count++;
            }
        }
    }
    printDepartureFooter();
    printCount(count, "Number of Departures:");
}
void Manager::numberDeparturesCountryCity(std::string country, std::string city) {
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
    printCount(count, "Number of Departures:");
}

void Manager::listCountriesMostAirlines(int n){
    priority_queue<pair<int, string>> countryAirlineCount;
    for (const string &country : countries){
        countryAirlineCount.push({countryToAirlineCount[country], country});
    }
    printListHeader("Countries with most Airlines:");
    for (int pos = 1; pos <= min(n, int(countries.size())); pos++){
        pair<int, string> current = countryAirlineCount.top();
        printOrderedValue(pos, current.first, current.second);
        countryAirlineCount.pop();
    }
}
void Manager::listCountriesMostAirports(int n){
    priority_queue<pair<int, string>> countryAirportCount;
    for (const string &country : countries){
        countryAirportCount.push({countryToAirportCount[country], country});
    }
    printListHeader("Countries with most Airports:");
    for (int pos = 1; pos <= min(n, int(countries.size())); pos++){
        pair<int, string> current = countryAirportCount.top();
        printOrderedValue(pos, current.first, current.second);
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
    printListHeader("Cities with most Airports:");
    for (int pos = 1; pos <= min(n, int(cities.size())); pos++){
        pair<int, pair<string,string>> current = countryCityAirportCount.top();
        printOrderedValueCity(pos, current.first, current.second.first, current.second.second);
        countryCityAirportCount.pop();
    }
}

void Manager::articulationPoints(){
    auto articulationPoints = flightNet.articulationPoints();
    unordered_set<Airport, AirportHash, AirportHash> s;
    for (const Airport &v : articulationPoints){
        s.insert(v);
    }
    printAirportHeader();
    for (const Airport &ap : s){
        printAirport(ap);
    }
    printAirportFooter();
    printCount(s.size(), "Number of Articulation Points:");
}
void Manager::diameter(){}

std::string Manager::getAirportCode(const std::string &name) {
    return airportNameToCode[name];
}

vector<string> Manager::getAirportsCountryCity(string country, string city) {return vector<string>();}
vector<string> Manager::getAirportsCoordinates(pair<double, double> coords) {return vector<string>();}
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string> *airportFlters, vector<string> *airlineFilters) {}

void Manager::printCount(int number, std::string text) {
    cout << "\n     " << BOLD << text << " " << MAGENTA << number << RESET;
}

void Manager::printListHeader(string text){
    cout << "\n       " << BOLD << text << RESET << '\n';
}

void Manager::printListValue(string text){
    cout << "    " << BOLD << MAGENTA << "- " << RESET << text << '\n';
}

void Manager::printOrderedValue(int pos, int count, string text){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << left << setw(30) << text
         << BOLD << CYAN << count << RESET << '\n';
}

void Manager::printOrderedValueAirport(int pos, int count, string text1, string text2){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << text1 << "  (" << left << setw(36) << text2.append(")")
         << BOLD << CYAN << count << RESET << '\n';
}

void Manager::printOrderedValueCity(int pos, int count, string text1, string text2){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << left << setw(20) << text1 << "  (" << left << setw(36) << text2.append(")")
         << BOLD << CYAN << count << RESET << '\n';
}

void Manager::printAirlineHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " CODE " << " "
         << setw(43) << " NAME" << " "
         << setw(27) << " CALLSIGN" << " "
         << setw(39) << " COUNTRY" << " " << '\n' << RESET;
}

void Manager::printAirline(const Airline &airline){
    cout << left << "│"
         << " " << setw(5)  << airline.getCode() << "│"
         << " " << setw(42) << airline.getName() << "│"
         << " " << setw(26) << (airline.getCallsign() == "_" ? "" : airline.getCallsign()) << "│"
         << " " << setw(38) << airline.getCountry() << "│" << '\n';
}

void Manager::printAirlineFooter() {
    cout << left << "└──────┴───────────────────────────────────────────┴───────────────────────────┴───────────────────────────────────────┘";
}

void Manager::printAirportHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " CODE " << " "
         << setw(56) << " NAME" << " "
         << setw(33) << " COUNTRY" << " "
         << setw(31) << " CITY" << " " << '\n' << RESET;
}

void Manager::printAirport(const Airport &airport) {
    cout << left << "│"
         << " " << setw(5)  << airport.getCode() << "│"
         << " " << setw(55) << airport.getName() << "│"
         << " " << setw(32) << airport.getCountry() << "│"
         << " " << setw(30) << airport.getCity() << "│" << '\n';
}

void Manager::printAirportFooter() {
    cout << left << "└──────┴────────────────────────────────────────────────────────┴─────────────────────────────────┴───────────────────────────────┘";
}

void Manager::printDepartureHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(6) << " DEST" << " "
         << setw(55) << " DESTINATION NAME" << " "
         << setw(8) << " AIRLINE" << " " << '\n' << RESET;
}

void Manager::printDeparture(string source, const Airport &dest, string airline){
    cout << left << "│"
         << " " << setw(5)  << source << "│"
         << " " << setw(5) << dest.getCode() << "│"
         << " " << setw(55) << dest.getName() << "│"
         << " " << setw(6) << airline << "│" << '\n';
}

void Manager::printDepartureFooter() {
    cout << left << "└──────┴──────┴────────────────────────────────────────────────────────┴───────┘";
}

void Manager::printArrivalHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(55) << " SOURCE NAME" << " "
         << setw(6) << " DEST" << " "
         << setw(8) << " AIRLINE" << " " << '\n' << RESET;
}

void Manager::printArrival(const Airport &source, string dest, string airline){
    cout << left << "│"
         << " " << setw(5)  << source.getCode() << "│"
         << " " << setw(55) << source.getName() << "│"
         << " " << setw(5) << dest << "│"
         << " " << setw(6) << airline << "│" << '\n';
}

void Manager::printArrivalFooter() {
    cout << left << "└──────┴────────────────────────────────────────────────────────┴──────┴───────┘";
}

void Manager::printFlightHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(55) << " SOURCE NAME" << " "
         << setw(6) << " DEST" << " "
         << setw(55) << " DESTINATION NAME" << " "
         << setw(8) << " AIRLINE" << " " << '\n' << RESET;
}
void Manager::printFlight(const Airport &source, const Airport &dest, string airline){
    cout << left << "│"
         << " " << setw(5)  << source.getCode() << "│"
         << " " << setw(55) << source.getName() << "│"
         << " " << setw(5) << dest.getCode() << "│"
         << " " << setw(55) << dest.getName() << "│"
         << " " << setw(6) << airline << "│" << '\n';
}
void Manager::printFlightFooter(){
    cout << left << "└──────┴────────────────────────────────────────────────────────┴──────┴────────────────────────────────────────────────────────┴───────┘";
}