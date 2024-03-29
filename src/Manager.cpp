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

        Airport airport = Airport(code, name, city, country, Coordinate(lat, lgt));
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
        countryToAirlineCount[country]++;
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

        Vertex<Airport> *a1 = airportCodeToVertex[code1];
        Vertex<Airport> *a2 = airportCodeToVertex[code2];
        double distance = a1->getInfo().getCoordinate().distance(a2->getInfo().getCoordinate());

        flightNet.addEdge(a1, a2, distance, airlineCode);
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
bool Manager::validateCity(const std::string &city, const std::string &country) {
    return countryCityToAirportCount.find({country, city}) != countryCityToAirportCount.end();
}

/**
 * @brief Lists all existent airlines.
 * Complexity: O(n), having n as the number of airlines.
 */
void Manager::listAllAirlines() {
    printAirlineHeader();
    for (Airline airline : airlines) {
        printAirline(airline);
    }
    printAirlineFooter();
    printCount(airlines.size(), "Total Number of Airlines: ");
}

/**
 * @brief Prints the number of existent airlines.
 * Complexity: O(1).
 */
void Manager::numberAirlines() {
    printCount(airlines.size(), "Total Number of Airlines: ");
}

/**
 * @brief Lists the airlines available at a given airport.
 * Complexity: O(n), having n as the number of flights at the given airport.
 * @param airport - Code of the airport to be analyzed.
 */
void Manager::listAirlinesAirport(string airport) {
    Vertex<Airport> *v = airportCodeToVertex[airport];
    unordered_set<string> availableAirlines;
    for (const Edge<Airport> &e : v->getAdj()) {
        availableAirlines.insert(e.getInfo());
    }

    printAirlineHeader();
    for (const string &airlineCode : availableAirlines) {
        printAirline(*airlines.find(Airline(airlineCode)));
    }
    printAirlineFooter();
    printCount(availableAirlines.size(), "Number of Airlines: ");
}

/**
 * @brief Lists the available airlines for a given country.
 * Complexity: O(n), having n as the number of airlines.
 * @param country - Country to be analyzed.
 */
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

/**
 * @brief Prints the information for a given airline.
 * Complexity:  O(1).
 * @param airline - Code of the airline to be analyzed.
 */
void Manager::airlineInfo(string airline) {
    printAirlineHeader();
    printAirline(*airlines.find(Airline(airline)));
    printAirlineFooter();
}

/**
 * @brief Lists all existent airports.
 * Complexity: O(n), having n as the number of airports.
 */
void Manager::listAllAirports() {
    printAirportHeader();
    for (const Airport& airport : airports){
        printAirport(airport);
    }
    printAirportFooter();
    printCount(airports.size(), "Total Number of Airports:");
}

/**
 * @brief Prints the number of existent airports.
 * Complexity: O(1).
 */
void Manager::numberAirports() {
    printCount(airports.size(), "Total Number of Airports:");
}

/**
 * @brief Lists the airports in a given city, in a given country.
 * Complexity: O(n), having n as the number of airports.
 * @param country - Name of the country to be analyzed.
 * @param city - Name of the city to be analyzed.
 */
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

/**
 * @brief Lists the first n airports with the most airlines.
 * Complexity: O(n * m * log(n * m)), having n as the number of airports and m the number of flights in an airport.
 * @param n - number to be analyzed.
 */
void Manager::listAirportsMostAirlines(int n) {
    priority_queue<pair<int, string>> airportAirlineCount;
    for (auto a: flightNet.getVertexSet()) {
        unordered_set<string> airportAirlines;
        for (const Edge<Airport> &e: a->getAdj()) {
            airportAirlines.insert(e.getInfo());
        }
        airportAirlineCount.push({airportAirlines.size(), a->getInfo().getCode()});
    }

    printListHeader("Airports with most Airlines:");
    for (int pos = 1; pos <= min(n, int(airports.size())); pos++) {
        pair<int, string> current = airportAirlineCount.top();
        printOrderedValueAirport(pos, current.first, current.second, airports.find(Airport(current.second))->getName());
        airportAirlineCount.pop();
    }
}

/**
 * @brief Lists the first n airports with the most flights.
 * Complexity: O(n * log(n)), having n as the number of airports.
 * @param n - number to be analyzed.
 */
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

/**
 * @brief Prints the information for a given airport.
 * Complexity:  O(1).
 * @param airport - Code of the airport to be analyzed.
 */
void Manager::airportInfo(const string airport){
    printAirportHeader();
    printAirport(*airports.find(Airport(airport)));
    printAirportFooter();
}

/**
 * @brief Lists the available flights for a given airport.
 * Complexity: O(n), n being the number of flights in an airport.
 * @param airport - Airport to be analyzed.
 */
void Manager::listAirportFlights(string airport) {
    printDepartureHeader();
    double maxDist = 0;
    for (const Edge<Airport> &e : airportCodeToVertex[airport]->getAdj()){
        printDeparture(airport, e.getDest()->getInfo(), e.getInfo(), e.getWeight());
        if (e.getWeight() > maxDist){
            maxDist = e.getWeight();
        }
    }
    printDepartureFooter();
    printCount(airportCodeToVertex[airport]->getAdj().size(), "Number of Flights:");
    printDouble(maxDist, "Longest Flight Distance [km]:");
}

/**
 * @brief Lists the airports reachable from a given airport within a n number of flights.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges visited in the graph respectively.
 * @param airport - Airport of origin.
 * @param n - Distance from the airport of origin in number of flights.
 */
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

/**
 * @brief Lists the cities reachable from a given airport within a n number of flights.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges visited in the graph respectively.
 * @param airport - Airport of origin.
 * @param n - Distance from the airport of origin in number of flights.
 */
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

/**
 * @brief Lists the countries reachable from a given airport within a n number of flights.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges visited within the distance n.
 * @param airport - Airport of origin.
 * @param n - Distance from the airport of origin in number of flights.
 */
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

/**
 * @brief Lists all tbe available flights.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 */
void Manager::listAllFlights() {
    printFlightHeader();
    double maxDist = 0;
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            printFlight(v->getInfo(), e.getDest()->getInfo(), e.getInfo(), e.getWeight());
            count++;
            if (e.getWeight() > maxDist){
                maxDist = e.getWeight();
            }
        }
    }
    printFlightFooter();
    printCount(count, "Total Number of Flights:");
    printDouble(maxDist, "Longest Flight Distance [km]:");
}

/**
 * @brief Prints the number of all available flights.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 */
void Manager::numberFlights(){
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            count++;
        }
    }
    printCount(count, "Total Number of Flights:");
}

/**
 * @brief Lists all tbe available flights from a given airline.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param airline - code of the airline to be analysed.
 */
void Manager::listFlightsAirline(string airline){
    int count = 0;
    double maxDist = 0;
    printFlightHeader();
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e.getInfo() == airline) {
                count++;
                printFlight(v->getInfo(), e.getDest()->getInfo(), e.getInfo(), e.getWeight());
                if (e.getWeight() > maxDist){
                    maxDist = e.getWeight();
                }
            }
        }
    }
    printFlightFooter();
    printCount(count, "Total Number of Flights:");
    printDouble(maxDist, "Longest Flight Distance [km]:");
}

/**
 * @brief Prints the number of all available flights from a given airline.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param airline - code of the airline to be analysed.
 */
void Manager::numberFlightsAirline(string airline){
    int count = 0;
    for (auto v : flightNet.getVertexSet()) {
        for (auto e : v->getAdj()) {
            if (e.getInfo() == airline) {
                count++;
            }
        }
    }
    printCount(count, "Number of Flights:");
}

/**
 * @brief Lists the flights arriving to a given city and/or country.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param country - name of country to be analysed.
 * @param city - name of city to be analysed.
 */
void Manager::listArrivalsCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    double maxDist = 0;
    printArrivalHeader();
    for (auto w : flightNet.getVertexSet()) {
        for (auto e : w->getAdj()){
            Vertex<Airport> *v = e.getDest();
            if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
                printArrival(w->getInfo(), v->getInfo().getCode(), e.getInfo(), e.getWeight());
                count++;
                if (e.getWeight() > maxDist){
                    maxDist = e.getWeight();
                }
            } else if (v->getInfo().getCountry() == country && city.empty()) {
                printArrival(w->getInfo(), v->getInfo().getCode(), e.getInfo(), e.getWeight());
                count++;
                if (e.getWeight() > maxDist){
                    maxDist = e.getWeight();
                }
            }
        }
    }
    printArrivalFooter();
    printCount(count, "Number of Arrivals:");
    printDouble(maxDist, "Longest Arrival Distance [km]:");
}

/**
 * @brief Prints the number of flights arriving to a given city and/or country.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param country - name of country to be analysed.
 * @param city - name of city to be analysed.
 */
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

/**
 * @brief Lists the flights departing from a given city and/or country.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param country - name of country to be analysed.
 * @param city - name of city to be analysed.
 */
void Manager::listDeparturesCountryCity(string country, string city){
    auto cityIt = cities.find(city);
    if (cityIt == cities.end()) city.erase();
    int count = 0;
    double maxDist = 0;
    printDepartureHeader();
    for (auto v : flightNet.getVertexSet()) {
        if (v->getInfo().getCountry() == country && !city.empty() && v->getInfo().getCity() == city) {
            for (auto e : v->getAdj()) {
                printDeparture(v->getInfo().getCode(), e.getDest()->getInfo(), e.getInfo(), e.getWeight());
                count++;
                if (e.getWeight() > maxDist){
                    maxDist = e.getWeight();
                }
            }
        } else if (v->getInfo().getCountry() == country && city.empty()) {
            for (auto e : v->getAdj()) {
                printDeparture(v->getInfo().getCode(), e.getDest()->getInfo(), e.getInfo(), e.getWeight());
                count++;
                if (e.getWeight() > maxDist){
                    maxDist = e.getWeight();
                }
            }
        }
    }
    printDepartureFooter();
    printCount(count, "Number of Departures:");
    printDouble(maxDist, "Longest Departure Distance [km]:");
}

/**
 * @brief Prints the number of flights departing from a given city and/or country.
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges of the graph respectively.
 * @param country - name of country to be analysed.
 * @param city - name of city to be analysed.
 */
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

/**
 * @brief Lists the countries, sorted in descending order of number of airlines.
 * Complexity: O(n * log(n)), n being the number of countries.
 * @param n - number of countries to be listed.
 */
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

/**
 * @brief Lists the countries, sorted in descending order of number of airports.
 * Complexity: O(n * log(n)), n being the number of countries.
 * @param n - number of countries to be listed.
 */
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

/**
 * @brief Lists the cities, sorted in descending order of number of airports.
 * Complexity: O(n * m * log(n * m)), n being the number of countries and m the number of cities.
 * @param n - number of countries to be listed.
 */
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

/**
 * @brief Lists all the airports that serve as articulation points for the flights graph.
 * Complexity: O(n), n being the number of articulation points.
 */
void Manager::articulationPoints(){
    Graph<Airport> aux;
    unordered_map<string, Vertex<Airport> *> auxNameToVertex;
    for (const Vertex<Airport> *x : flightNet.getVertexSet()){
        auxNameToVertex[x->getInfo().getCode()] = aux.addVertex(x->getInfo());
    }
    for (auto *x : flightNet.getVertexSet()){
        for (const auto &y : x->getAdj()){
            aux.addEdge(auxNameToVertex[x->getInfo().getCode()], auxNameToVertex[y.getDest()->getInfo().getCode()], y.getWeight(), y.getInfo());
            aux.addEdge(auxNameToVertex[y.getDest()->getInfo().getCode()], auxNameToVertex[x->getInfo().getCode()], y.getWeight(), y.getInfo());
        }
    }
    auto articulationPoints = aux.articulationPoints();

    printAirportHeader();
    for (const auto *ap : articulationPoints){
        printAirport(ap->getInfo());
    }
    printAirportFooter();
    printCount(articulationPoints.size(), "Number of Articulation Points:");
}

/**
 * @brief Prints the diameter of the flights graph.
 * Complexity: O(1).
 */
void Manager::diameter(){
    printCount(flightNet.diameter(), "Diameter of the graph:");
}

/**
 * @brief Lists the longest flights (paths) possible in the flights graph.
 * Complexity: O(n * m), n being the number of origin airports and m the number of flights of each airport.
 */
void Manager::longestPaths(){
    int distance;
    vector<pair<Vertex<Airport>*, vector<Vertex<Airport> *>>> result = flightNet.longestPaths(distance);
    for (const auto &x : result){
        printListHeader(x.first->getInfo().getCode());
        for (const auto &y : x.second){
            printListValue(y->getInfo().getCode());
        }
    }
    printCount(distance, "Longest Path Distance:");
}

/**
 * @brief Searches for the code of an airport with a specific name..
 * Complexity: O(1).
 * @param name - Name of airport.
 * @return Code of the airport.
 */
std::string Manager::getAirportCode(const std::string &name) {
    return airportNameToCode[name];
}

/**
 * @brief Searches for all airports in a given city and/or country.
 * Complexity: O(n), n being the number of airports in the graph.
 * @param country - Name of country.
 * @param city - Name of city.
 * @return Vector of codes of all airports in the given city and/or country.
 */
vector<string> Manager::getAirportsCountryCity(string country, string city) {
    vector<string> res;
    for (const Airport &airport : airports) {
        if (airport.getCity() == city && airport.getCountry() == country)
            res.push_back(airport.getCode());
    }
    return res;
}

/**
 * @brief Searches for all airports in a given country.
 * Complexity: O(n), n being the number of airports in the graph.
 * @param country - Name of country.
 * @return Vector of codes of all airports in the given country.
 */
vector<string> Manager::getAirportsCountry(string country) {
    vector<string> res;
    for (const Airport &airport : airports) {
        if (airport.getCountry() == country)
            res.push_back(airport.getCode());
    }
    return res;
}

/**
 * @brief Searches for the closest airports to a given point.
 * Complexity: O(n), n being the number of airports in the graph.
 * @param coords - Coordinates for the point.
 * @return Vector of codes of all airports closest to the given coordinates.
 */
vector<string> Manager::getAirportsCoordinates(Coordinate coords) {
    vector<string> res;
    double distance = INT_MAX;
    for (const Airport &airport : airports) {
        Coordinate airport_coord(airport.getLatitiude(), airport.getLongitude());
        double distance_to_airport = coords.distance(airport_coord);
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

/**
 * @brief Prints the best flight option, considering a number of preferences.
 * Complexity: O(n * m * (|V| + |E|), n being the size of sources, m the size of destinations, |V| the number of vertices and |E| the number of edges in the graph.
 * @param sources - vector of names and/or codes of the airports of departure.
 * @param destinations - vector of names and/or codes of the airports of arrival.
 * @param airlinePreferences - Vector of codes of preferred airlines.
 * @param airlineRestrictions - Vector of codes of airlines to avoid.
 * @param airportRestrictions - Vector of codes of airports to avoid.
 */
void Manager::bestFlightOption(vector<string> *sources, vector<string> *destinations, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions){
    // Removing values from the source that are restricted
    auto It = sources->begin();
    while (It != sources->end()) {
        if (find(airportRestrictions->begin(), airportRestrictions->end(), *It) != airportRestrictions->end() || find(airportRestrictions->begin(), airportRestrictions->end(), airportNameToCode[*It]) != airportRestrictions->end())
            It = sources->erase(It);
        else {
            It++;
        }
    }
    // Removing values from the destination that are restricted
    It = destinations->begin();
    while (It != destinations->end()) {
        if (find(airportRestrictions->begin(), airportRestrictions->end(), *It) != airportRestrictions->end() || find(airportRestrictions->begin(), airportRestrictions->end(), airportNameToCode[*It]) != airportRestrictions->end())
            It = destinations->erase(It);
        else {
            It++;
        }
    }
    // Resetting all the vertexes
    for (Vertex<Airport> *w : flightNet.getVertexSet()){
        w->setVisited(false);
        w->parents.clear();
        w->setVisitIndex(-1);
    }

    // BFS for all (source,destination) pairs, stacking and resetting the result
    int minDist = INT_MAX;
    for (string& src : *sources){
        for (string& dest : *destinations){
            int current = minDistanceBFS(src, dest, airlinePreferences, airlineRestrictions, airportRestrictions);
            if (current < minDist){
                minDist = current;
            }
        }
    }

    // If there are no paths, end here (this accounts for empty src and dest)
    if (minDist == INT_MAX){
        printCount(0, "Total Number of Different Paths:");
        return;
    }

    // Reconstruct the paths for each destination
    paths.clear();
    for (auto x : *destinations){
        path.clear();
        reconstructPaths(airportCodeToVertex[x], minDist);
    }

    // Output the results
    int airportCount = printPaths(airlinePreferences, airlineRestrictions);
    printCount(airportCount - 1, "Minimum path distance (Flights):");
    printCount(paths.size(), "Total number of different paths:");

}

/**
 * @brief Reconstructs paths from a destination vertex back to the source in the flights graph.
 * Complexity: O(n * m), n being the number of flights of the destination vertex and m its minimum distance to the source vertex.
 * @param dest - Vertex of destination.
 * @param minDist - Minimum distance to source vertex.
 */
void Manager::reconstructPaths(Vertex<Airport> *dest, int minDist){
    if (minDist < 0){
        return;
    }
    if (dest->parents.empty()){
        path.push_back(dest->getInfo().getCode());
        if (path.size() >= 2){
            paths.push_back(path);
        }
        path.pop_back();
    }
    else {
        for (auto x : dest->parents){
            path.push_back(dest->getInfo().getCode());
            reconstructPaths(airportCodeToVertex[x], minDist - 1);
            path.pop_back();
        }
    }
}

/**
 * @brief Conducts a breadth-first search of the graph from a source vertex..
 * Complexity: O(|V| + |E|), |V| and |E| being the number of vertices and edges in the graph respectively..
 * @param src - Code of departure airport.
 * @param dest - Code of arrival airport.
 * @param airlinePreferences - Vector of codes of preferred airlines.
 * @param airlineRestrictions - Vector of codes of airlines to avoid.
 * @param airportRestrictions - Vector of codes of airports to avoid.
 * @return The minimum distance for a flight between two airports in the graph.
 */
// Calculates the minimum distance needed according to the restrictions
int Manager::minDistanceBFS(string &src, string &dest, vector<string>* airlinePreferences, vector<string> *airlineRestrictions, vector<string> *airportRestrictions) {

    queue<Vertex<Airport> *> remaining;
    Vertex<Airport> *source = airportCodeToVertex[src];
    source->setVisited(true);
    source->setVisitIndex(0);
    source->parents.clear();
    remaining.push(source);

    int distance = INT_MAX;

    while (!remaining.empty()){
        Vertex<Airport> *current = remaining.front();
        remaining.pop();

        if (current->getVisitIndex() > distance){
            continue;
        }

        for (const Edge<Airport> &e : current->getAdj()){
            Vertex<Airport> *next = e.getDest();

            bool isValidAirlinePref = airlinePreferences->empty() || find(airlinePreferences->begin(), airlinePreferences->end(), e.getInfo()) != airlinePreferences->end();
            bool isValidAirlineRest = find(airlineRestrictions->begin(), airlineRestrictions->end(), e.getInfo()) == airlineRestrictions->end();
            bool isValidAirport = find(airportRestrictions->begin(), airportRestrictions->end(), next->getInfo().getCode()) == airportRestrictions->end();

            if (isValidAirport && isValidAirlinePref && isValidAirlineRest){
                if (!next->isVisited()){
                    remaining.push(next);
                    next->setVisited(true);
                    next->setVisitIndex(current->getVisitIndex() + 1);
                    next->parents.insert(current->getInfo().getCode());
                }
                else if (next->isVisited()){
                    if (next->getVisitIndex() > current->getVisitIndex() + 1){
                        next->parents.clear();
                        next->parents.insert(current->getInfo().getCode());
                        next->setVisitIndex(current->getVisitIndex() + 1);
                    }
                    else if (next->getVisitIndex() == current->getVisitIndex() + 1){
                        next->parents.insert(current->getInfo().getCode());
                    }
                }
                if (next->getInfo().getCode() == dest){
                    distance = min(distance, next->getVisitIndex());
                    if (next->getVisitIndex() > distance){
                        next->parents.clear();
                        next->parents.insert(current->getInfo().getCode());
                    }
                }
            }
        }
    }
    return distance;
}

/************************************/
/**
 * @brief Formats and prints a specific (integer) value of a given item.
 * Complexity: O(1).
 * @param number - Value.
 * @param text - Item.
 */
void Manager::printCount(int number, std::string text) {
    cout << "\n     " << BOLD << text << " " << MAGENTA << number << RESET;
}

/**
 * @brief Formats and prints a specific (floating) value of a given item.
 * Complexity: O(1).
 * @param number - Value.
 * @param text - Item.
 */
void Manager::printDouble(double number, std::string text) {
    cout << "\n     " << BOLD << text << " " << MAGENTA << number << RESET;
}

/**
 * @brief Formats and prints the header for a list.
 * Complexity: O(1).
 * @param text - Name of list.
 */
void Manager::printListHeader(string text){
    cout << "\n       " << BOLD << text << RESET << '\n';
}

/**
 * @brief Formats and prints a specific value from a list.
 * Complexity: O(1).
 * @param text - Value.
 */
void Manager::printListValue(string text){
    cout << "    " << BOLD << MAGENTA << "- " << RESET << text << '\n';
}

/**
 * @brief Formats and prints the value of a given something in an ordered list format.
 * Complexity: O(1).
 * @param pos - Position of the item.
 * @param count - Value.
 * @param text - Item.
 */
void Manager::printOrderedValue(int pos, int count, string text){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << left << setw(30) << text
         << BOLD << CYAN << count << RESET << '\n';
}

/**
 * @brief Formats and prints an value for a given airport in an ordered list format.
 * Complexity: O(1).
 * @param pos - Position of the item.
 * @param count - Value.
 * @param text1 - Airport code.
 * @param text2 - Airport name.
 */
void Manager::printOrderedValueAirport(int pos, int count, string text1, string text2){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << text1 << "  (" << left << setw(36) << text2.append(")")
         << BOLD << CYAN << count << RESET << '\n';
}

/**
 * @brief Formats and prints a value for a given city in an ordered list format.
 * Complexity: O(1).
 * @param pos - Position of the value.
 * @param count - Value
 * @param text1 - Country name.
 * @param text2 - City name.
 */
void Manager::printOrderedValueCity(int pos, int count, string text1, string text2){
    cout << "    " << BOLD << MAGENTA << right << setw(3) << pos << ")  " << RESET
         << left << setw(20) << text1 << "  (" << left << setw(36) << text2.append(")")
         << BOLD << CYAN << count << RESET << '\n';
}

/**
 * @brief Formats and prints the header for an airline table.
 * Complexity: O(1).
 */
void Manager::printAirlineHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " CODE " << " "
         << setw(43) << " NAME" << " "
         << setw(27) << " CALLSIGN" << " "
         << setw(39) << " COUNTRY" << " " << '\n' << RESET;
}

/**
 * @brief Formats and prints a line in an airline table.
 * Complexity: O(1).
 * @param airline - Airline.
 */
void Manager::printAirline(const Airline &airline){
    cout << left << "│"
         << " " << setw(5)  << airline.getCode() << "│"
         << " " << setw(42) << airline.getName() << "│"
         << " " << setw(26) << (airline.getCallsign() == "_" ? "" : airline.getCallsign()) << "│"
         << " " << setw(38) << airline.getCountry() << "│" << '\n';
}

/**
 * @brief Formats the footer for an airline table.
 * Complexity: O(1).
 */
void Manager::printAirlineFooter() {
    cout << left << "└──────┴───────────────────────────────────────────┴───────────────────────────┴───────────────────────────────────────┘";
}

/**
 * @brief Formats and prints the header for an airport table.
 * Complexity: O(1).
 */
void Manager::printAirportHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " CODE " << " "
         << setw(56) << " NAME" << " "
         << setw(33) << " COUNTRY" << " "
         << setw(31) << " CITY" << " " << '\n' << RESET;
}

/**
 * @brief Formats and prints a line in an airport table.
 * Complexity: O(1).
 * @param airline - Airport.
 */
void Manager::printAirport(const Airport &airport) {
    cout << left << "│"
         << " " << setw(5)  << airport.getCode() << "│"
         << " " << setw(55) << airport.getName() << "│"
         << " " << setw(32) << airport.getCountry() << "│"
         << " " << setw(30) << airport.getCity() << "│" << '\n';
}

/**
 * @brief Formats the footer for an airport table.
 * Complexity: O(1).
 */
void Manager::printAirportFooter() {
    cout << left << "└──────┴────────────────────────────────────────────────────────┴─────────────────────────────────┴───────────────────────────────┘";
}

/**
 * @brief Formats and prints the header for a departing flights table.
 * Complexity: O(1).
 */
void Manager::printDepartureHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(6) << " DEST" << " "
         << setw(55) << " DESTINATION NAME" << " "
         << setw(8) << " AIRLINE" << " "
         << setw(12) << "   DISTANCE" << " " << '\n' << RESET;
}

/**
 * @brief Formats and prints a line in a departing flights table.
 * Complexity: O(1).
 * @param source - Code of the airport of departure.
 * @param dest - Airport of arrival.
 * @param airline - Airline of the flight.
 * @param distance - Flight distance.
 */
void Manager::printDeparture(string source, const Airport &dest, string airline, double distance){
    cout << left << "│"
         << " " << setw(5)  << source << "│"
         << " " << setw(5) << dest.getCode() << "│"
         << " " << setw(55) << dest.getName() << "│"
         << " " << setw(6) << airline << "│"
         << " " << right << setw(10) << distance << " │" << '\n';
}

/**
 * @brief Formats the footer for a departing flights table.
 * Complexity: O(1).
 */
void Manager::printDepartureFooter() {
    cout << left << "└──────┴──────┴────────────────────────────────────────────────────────┴───────┴────────────┘";
}

/**
 * @brief Formats and prints the header for an arriving flights table.
 * Complexity: O(1).
 */
void Manager::printArrivalHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(55) << " SOURCE NAME" << " "
         << setw(6) << " DEST" << " "
         << setw(8) << " AIRLINE" << " "
         << setw(12) << "   DISTANCE" << " " << '\n' << RESET;
}

/**
 * @brief Formats and prints a line in an arriving flights table.
 * Complexity: O(1).
 * @param source - Airport of departure.
 * @param dest - Code of airport of arrival.
 * @param airline - Airline of the flight.
 * @param distance - Flight distance.
 */
void Manager::printArrival(const Airport &source, string dest, string airline, double distance){
    cout << left << "│"
         << " " << setw(5)  << source.getCode() << "│"
         << " " << setw(55) << source.getName() << "│"
         << " " << setw(5) << dest << "│"
         << " " << setw(6) << airline << "│"
         << " " << right << setw(10) << distance << " │" << '\n';
}

/**
 * @brief Formats the footer for an arriving flights table.
 * Complexity: O(1).
 */
void Manager::printArrivalFooter() {
    cout << left << "└──────┴────────────────────────────────────────────────────────┴──────┴───────┴────────────┘";
}

/**
 * @brief Formats and prints the header for a flights table.
 * Complexity: O(1).
 */
void Manager::printFlightHeader(){
    cout << '\n' << INVERT << BOLD << left  << " "
         << setw(6) << " SRC " << " "
         << setw(55) << " SOURCE NAME " << "  "
         << setw(6) << " DEST" << " "
         << setw(55) << " DESTINATION NAME" << " "
         << setw(8) << " AIRLINE" << " "
         << setw(12) << "   DISTANCE" << " " << '\n' << RESET;
}

/**
 * @brief Formats and prints a line in a flights table.
 * Complexity: O(1).
 * @param source - Airport of departure.
 * @param dest - Airport of arrival.
 * @param airline - Airline of the flight.
 * @param distance - Flight distance.
 */
void Manager::printFlight(const Airport &source, const Airport &dest, string airline, double distance){
    cout << left << "│"
         << " " << setw(5)  << source.getCode() << "│"
         << " " << setw(55) << source.getName() << "│"
         << " " << setw(5) << dest.getCode() << "│"
         << " " << setw(55) << dest.getName() << "│"
         << " " << setw(6) << airline << "│"
         << " " << right << setw(10) << distance << " │" << '\n';
}

/**
 * @brief Formats the footer for a flights table.
 * Complexity: O(1).
 */
void Manager::printFlightFooter(){
    cout << left << "└──────┴────────────────────────────────────────────────────────┴──────┴────────────────────────────────────────────────────────┴───────┴────────────┘";
}

/**
 * @brief Formats and prints a path on the flights graph.
 * Complexity: O(n * m), n being the number of paths available and m the number of flights in a given path.
 * @param airlinePreferences - Vector of codes of preferred airlines.
 * @param airlineRestrictions - Vector of code of airlines to avoid.
 * @return The minimum size for a path of the graph, respecting the given preferences.
 */
int Manager::printPaths(vector<string>* airlinePreferences, vector<string> *airlineRestrictions){
    int min = INT_MAX;
    for (int pathIdx = 0; pathIdx < paths.size(); pathIdx++){
        vector<string> &path = paths[pathIdx];
        std::reverse(path.begin(), path.end());
        cout << FAINT << " Path " << RESET << right << setw(3) << (pathIdx+1) << ":   ";
        for (int flightIdx = 0; flightIdx < path.size() - 1; flightIdx++){
            string currentAirport = path[flightIdx];
            string nextAirport = path[flightIdx+1];
            cout << BOLD << YELLOW << currentAirport << RESET;
            printFlightAirlines(currentAirport, nextAirport, airlinePreferences, airlineRestrictions);
        }
        cout << BOLD << YELLOW << path[path.size()-1] << RESET;
        cout << '\n';
        if (path.size() < min){
            min = path.size();
        }
    }
    return min;
}

/**
 * @brief Formats and prints the airlines for a specific flight when printing a path.
 * Complexity: O(n), n being the number of flights departing the airport of departure.
 * @param currentAirport - Code of the airport of departure.
 * @param nextAirport - Code of the airport of arrival.
 * @param airlinePreferences - Vector of codes of preferred airlines.
 * @param airlineRestrictions - Vector of codes of airlines to avoid.
 */
void Manager::printFlightAirlines(const string &currentAirport, const string &nextAirport, vector<string>* airlinePreferences, vector<string> *airlineRestrictions){
    cout << " [ ";
    for (const Edge<Airport> &e : airportCodeToVertex[currentAirport]->getAdj()){
        if (e.getDest()->getInfo().getCode() == nextAirport){
            bool isValidAirlinePref = airlinePreferences->empty() || find(airlinePreferences->begin(), airlinePreferences->end(), e.getInfo()) != airlinePreferences->end();
            bool isValidAirlineRest = find(airlineRestrictions->begin(), airlineRestrictions->end(), e.getInfo()) == airlineRestrictions->end();
            if (isValidAirlinePref && isValidAirlineRest){
                cout << BOLD << GREEN << FAINT << e.getInfo() << RESET << " ";
            }
        }
    }
    cout << "] ";
}