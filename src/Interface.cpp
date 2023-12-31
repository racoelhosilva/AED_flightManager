#include <iostream>
#include <climits>
#include "Interface.h"

/**
 * @brief Prints the menu header.
 */
void Interface::header() {
    std::cout << BOLD << "┌─────────────────────────────────────────────────────────────────────────────────────────────────┐\n"
              << "├" << RESET << "─────────────────────────────────── " << BOLD << BLUE << "Flight Manager (Group 15)" << RESET <<" ───────────────────────────────────" << BOLD << "┤\n\n" << RESET;
}

/**
 * @brief Prints the menu footer.
 */
void Interface::footer() {
    std::cout << BOLD << "├" << RESET << "─────────────────────────────────── " << BOLD << BLUE << "Flight Manager (Group 15)" << RESET << " ───────────────────────────────────" << BOLD "┤\n"
              << "└─────────────────────────────────────────────────────────────────────────────────────────────────┘\n" << RESET;
}

/**
 * @brief Clears the screen.
 */
void Interface::clear() {
    system("clear");
}

/**
 * @brief Waits for user input to continue.
 */
void Interface::outputWait() {
    cin.clear();
    cout << "\n\n                                    " << FAINT << "< Press " << RESET << BOLD << "ENTER" << RESET << FAINT << " to Continue >" << RESET;
    cin.ignore();
}

/**
 * @brief Initializes the interface.
 */
void Interface::init() {
    header();
    this->manager = Manager();
    if (loadData()){
        std::cout << YELLOW << BOLD << "        Data Loaded: " << GREEN
                     "\n ✓ " << RESET << "Starting flight manager!\n";
        this->mainMenu();
    }
    else {
        std::cout << RED << BOLD << " ✗ Problem loading data: Exiting!\n" << RESET;
    }
}

/**
 * @brief Prints the exit menu.
 */
void Interface::exitMenu() {
    clear();

    std::cout << "\n                               " << BOLD << UNDERLINE << "Thanks for using our Flight Manager!" << RESET << "\n\n";

    footer();
    exit(0);
}

/**
 * @brief Requests the loading of the data from the manager.
 * @return True if the operation was successful.
 */
bool Interface::loadData() {
    std::cout << "        " << BOLD << YELLOW << "Loading data (this may take a while):\n";
    if (!manager.extractAirports("../data/airports.csv")) {
        return false;
    }
    std::cout << GREEN << BOLD << " ✓ " << RESET << "Airports loaded\n";
    if (!manager.extractAirlines("../data/airlines.csv")) {
        return false;
    }
    std::cout << GREEN << BOLD << " ✓ " << RESET << "Airlines loaded\n";
    if (!manager.extractFlights("../data/flights.csv")){
        return false;
    }
    std::cout << GREEN << BOLD << " ✓ " << RESET << "Flights loaded\n";
    return true;
}

/**
 * @brief Prints available options.
 * @param options - vector of available options.
 */
void Interface::printOptions(const std::vector<std::string> &options) {
    std::cout << "     " << BOLD << options[options.size()-1] << RESET << "\n";
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        std::cout << CYAN << BOLD << " [" << idx << "] " << RESET << options[idx] << '\n';
    }
    std::cout << RED << " [0] " << RESET << options[0] << '\n';
}

/**
 * @brief Prints selected option.
 * @param s - Selected option.
 */
void Interface::printSelected(const std::string &s) {
    std::cout << "     " <<  BOLD << "> " << YELLOW << s << RESET << " selected\n";
}

void Interface::printFilters(){
    if (airlinePreferences.empty() && airlineRestrictions.empty() && airportRestrictions.empty()){
        return;
    }
    std::cout << BOLD << "            Current Filters:" << RESET;
    if (!airlinePreferences.empty()){
        std::cout << GREEN << FAINT << BOLD << "\n        Airline Preferences" << RESET << ": ";
        for (const string &t : airlinePreferences){
            cout << t << "  ";
        }

    }

    if (!airlineRestrictions.empty()){
        std::cout << RED << FAINT << BOLD << "\n        Airline Restrictions" << RESET << ": ";
        for (const string &t : airlineRestrictions){
            cout << t << "  ";
        }
    }

    if (!airportRestrictions.empty()){
        std::cout << RED << FAINT << BOLD << "\n        Airport Restrictions" << RESET << ": ";
        for (const string &t : airportRestrictions){
            cout << t << "  ";
        }
    }
    cout << RESET << '\n';
}

/**
 * @brief Reads and filters the user chosen option.
 * @param max - Maximum number of options.
 * @return - Number of the chosen option.
 */
int Interface::readOption(int max) {
    string choice;
    do {
        std::cout << FAINT << "  Option" << RESET << ": ";
        cin.clear();
        std::cin >> choice;
        cin.ignore();
    } while (!validOption(max, choice));
    return stoi(choice);
}

/**
 * @brief Validates the user chosen option.
 * @param size - Maximum number of options.
 * @param choice - User chosen option.
 * @return True if the option is valid.
 */
bool Interface::validOption(unsigned long size, const std::string &choice) {
    return choice.size() == 1 && "0" <= choice && choice <= to_string(size-2);
}

/**
 * @brief Reads and requests the manager to validate user inputted airlines.
 * @return - Code of the inputted airline.
 */
std::string Interface::readAirline() {
    string choice;
    do {
        std::cout << FAINT << "  Airline Code" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirline(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted airport codes.
 * @return - Code of the inputted airport.
 */
std::string Interface::readAirportCode() {
    string choice;
    do {
        std::cout << FAINT << "  Airport Code" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirport(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted airport names.
 * @return - Name of the inputted airport.
 */
std::string Interface::readAirportName() {
    string choice;
    do {
        std::cout << FAINT << "  Airport Name" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirportName(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted cities.
 * @return - Name of the inputted city.
 */
std::string Interface::readCity(string country) {
    string choice;
    do {
        std::cout << FAINT << "  City" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCity(choice, country));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted cities.
 * @return - Name of the inputted city.
 */
std::string Interface::readCityOptional() {
    string choice;
    std::cout << FAINT << "  City (Optional)" << RESET << ": ";
    cin.clear();
    getline(cin, choice, '\n');
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted countries.
 * @return - Name of the inputted country.
 */
std::string Interface::readCountry() {
    string choice;
    do {
        std::cout << FAINT << "  Country" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCountry(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted coordinates.
 * @return - Inputted coordinates.
 */
Coordinate Interface::readCoordinates() {
    double latitude;
    do {
        std::cout << FAINT << "  Latitude:" << RESET << ": ";
        cin.clear();
        std::cin >> latitude;
    } while (cin.fail() || latitude < -90 || latitude > 90);
    double longitude;
    do {
        std::cout << FAINT << "  Longitude:" << RESET << ": ";
        cin.clear();
        std::cin >> longitude;
    } while (cin.fail() || latitude < -180 || latitude > 180);
    return {latitude, longitude};
}

bool stringIsNumeric(const string &s){
    for (const char &c : s){
        if (!isdigit(c)){
            return false;
        }
    }
    return true;
}

/**
 * @brief Reads and requests the manager to validate user inputted numbers.
 * @return - Inputted number.
 */
int Interface::readNumber() {
    string choice;
    do {
        std::cout << FAINT << "  Number" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!stringIsNumeric(choice));
    return stoi(choice);
}

/**
 * @brief Prints the main menu.
 */
void Interface::mainMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Quit",
             "Check Statistics",
             "Search Flights",
             "Choose your operation:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            statisticsMenu();
            break;
        case 2:
            flightSourceMenu();
            break;
        case 0:
            exitMenu();
            break;
    }
    mainMenu();
}

/**
 * @brief Prints the flight statistics menu.
 */
void Interface::statisticsMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "Airlines",
             "Airports",
             "Flights",
             "Countries/Cities",
             "Articulation Points",
             "Graph Diameter",
             "Paths with Most Stops",
             "Choose type of Statistics:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            airlineStatisticsMenu();
            break;
        case 2:
            airportStatisticsMenu();
            break;
        case 3:
            flightStatisticsMenu();
            break;
        case 4:
            locationStatisticsMenu();
            break;
        case 5:
            manager.articulationPoints();
            outputWait();
            break;
        case 6:
            manager.diameter();
            outputWait();
            break;
        case 7:
            manager.longestPaths();
            outputWait();
            break;
        case 0:
            return;
    }
    statisticsMenu();
}

/**
 * @brief Prints the airline statistics menu.
 */
void Interface::airlineStatisticsMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "List all Airlines",
             "Number of Airlines",
             "Airlines in Airport",
             "Airlines in Country",
             "Airline Information",
             "Choose type of Airline Statistics:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            manager.listAllAirlines();
            outputWait();
            break;
        case 2:
            manager.numberAirlines();
            outputWait();
            break;
        case 3: {
            string airportCode = readAirportCode();
            manager.listAirlinesAirport(airportCode);
            outputWait();
            break;
        }
        case 4: {
            string country = readCountry();
            manager.listAirlinesCountry(country);
            outputWait();
            break;
        }
        case 5: {
            string airline = readAirline();
            manager.airlineInfo(airline);
            outputWait();
            break;
        }
        case 0:
            return;
    }
    airlineStatisticsMenu();
}

/**
 * @brief Prints the airport statistics menu.
 */
void Interface::airportStatisticsMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "List all Airports",
             "Total Number of Airports",
             "Airports in a Country/City",
             "N Airports with Most Airlines",
             "N Airports with Most Flights",
             "Airport Information",
             "Choose type of Airport Statistics:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            manager.listAllAirports();
            outputWait();
            break;
        case 2:
            manager.numberAirports();
            outputWait();
            break;
        case 3: {
            string country = readCountry();
            string city = readCityOptional();
            manager.listAirportsCountryCity(country, city);
            outputWait();
            break;
        }
        case 4: {
            int n = readNumber();
            manager.listAirportsMostAirlines(n);
            outputWait();
            break;
        }
        case 5: {
            int n = readNumber();
            manager.listAirportsMostFlights(n);
            outputWait();
            break;
        }
        case 6: {
            airportInformationMenu();
            break;
        }
        case 0:
            return;
    }
    airportStatisticsMenu();
}

/**
 * @brief Prints the airport information menu.
 */
void Interface::airportInformationMenu(){
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "Airport Information",
             "Airline List",
             "Flight List",
             "Reachable Airports",
             "Reachable Airports with N Flights",
             "Reachable Cities",
             "Reachable Cities with N Flights",
             "Reachable Countries",
             "Reachable Countries with N Flights",
             "Choose type of Airport Information:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1: {
            string airport = readAirportCode();
            manager.airportInfo(airport);
            outputWait();
            break;
        }
        case 2: {
            string airport = readAirportCode();
            manager.listAirlinesAirport(airport);
            outputWait();
            break;
        }
        case 3: {
            string airport = readAirportCode();
            manager.listAirportFlights(airport);
            outputWait();
            break;
        }
        case 4: {
            string airport = readAirportCode();
            manager.reachableAirports(airport, 1);
            outputWait();
            break;
        }
        case 5: {
            string airport = readAirportCode();
            int n = readNumber();
            manager.reachableAirports(airport, n);
            outputWait();
            break;
        }
        case 6: {
            string airport = readAirportCode();
            manager.reachableCities(airport, 1);
            outputWait();
            break;
        }
        case 7: {
            string airport = readAirportCode();
            int n = readNumber();
            manager.reachableCities(airport, n);
            outputWait();
            break;
        }
        case 8: {
            string airport = readAirportCode();
            manager.reachableCountries(airport, 1);
            outputWait();
            break;
        }
        case 9: {
            string airport = readAirportCode();
            int n = readNumber();
            manager.reachableCountries(airport, n);
            outputWait();
            break;
        }
        case 0:
            return;
    }
    airportInformationMenu();
}

/**
 * @brief Prints the flight statistics menu.
 */
void Interface::flightStatisticsMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "List all Flights",
             "Number of Flights",
             "Flights by Airline",
             "Number of Flights by Airline",
             "Arrivals by Country/City",
             "Number of Arrivals by Country/City",
             "Departures by Country/City",
             "Number of Departures by Country/City",
             "Choose type of Flight Statistics:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            manager.listAllFlights();
            outputWait();
            break;
        case 2:
            manager.numberFlights();
            outputWait();
            break;
        case 3: {
            string airline = readAirline();
            manager.listFlightsAirline(airline);
            outputWait();
            break;
        }
        case 4: {
            string airline = readAirline();
            manager.numberFlightsAirline(airline);
            outputWait();
            break;
        }
        case 5: {
            string country = readCountry();
            string city = readCityOptional();
            manager.listArrivalsCountryCity(country, city);
            outputWait();
            break;
        }
        case 6: {
            string country = readCountry();
            string city = readCityOptional();
            manager.numberArrivalsCountryCity(country, city);
            outputWait();
            break;
        }
        case 7: {
            string country = readCountry();
            string city = readCityOptional();
            manager.listDeparturesCountryCity(country, city);
            outputWait();
            break;
        }
        case 8: {
            string country = readCountry();
            string city = readCityOptional();
            manager.numberDeparturesCountryCity(country, city);
            outputWait();
            break;
        }
        case 0:
            return;
    }
    flightStatisticsMenu();
}

/**
 * @brief Prints location statistics menu.
 */
void Interface::locationStatisticsMenu() {
    clear();
    header();
    std::vector<std::string> options =
            {"Back",
             "Airlines in Country",
             "Countries with most Airlines",
             "Airports in Country",
             "Countries with most Airports",
             "Airports in City",
             "Cities with most Airports",
             "Choose type of Location Statistics:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1: {
            string country = readCountry();
            manager.listAirlinesCountry(country);
            outputWait();
            break;
        }
        case 2: {
            int n = readNumber();
            manager.listCountriesMostAirlines(n);
            outputWait();
            break;
        }
        case 3: {
            string country = readCountry();
            manager.listAirportsCountryCity(country);
            outputWait();
            break;
        }
        case 4: {
            int n = readNumber();
            manager.listCountriesMostAirports(n);
            outputWait();
            break;
        }
        case 5: {
            string country = readCountry();
            string city = readCity(country);
            manager.listAirportsCountryCity(country, city);
            outputWait();
            break;
        }
        case 6: {
            int n = readNumber();
            manager.listCitiesMostAirports(n);
            outputWait();
            break;
        }
        case 0:
            return;
    }
    locationStatisticsMenu();
}

/**
 * @brief Prints the flight source menu.
 */
void Interface::flightSourceMenu() {
    clear();
    header();

    std::vector<std::string> options =
            {"Back",
             "Airport Code",
             "Airport Name",
             "Country/City",
             "Geographical Coordinates",
             "Choose the Source:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);

    sourceCodes.clear();
    switch (choice) {
        case 1:
            sourceCodes.push_back(readAirportCode());
            break;
        case 2:
            sourceCodes.push_back(manager.getAirportCode(readAirportName()));
            break;
        case 3: {
            string country = readCountry();
            string city = readCity(country);
            sourceCodes = manager.getAirportsCountryCity(country, city);
            break;
        }
        case 4:
            sourceCodes = manager.getAirportsCoordinates(readCoordinates());
            break;
        case 0:
            return;
    }
    std::cout << "\n";
    flightDestinationMenu();
    flightSourceMenu();
}

/**
 * @brief Prints the flight destination menu.
 */
void Interface::flightDestinationMenu() {
    std::vector<std::string> options =
            {"Back",
             "Airport Code",
             "Airport Name",
             "Country/City",
             "Geographical Coordinates",
             "Choose the Destination:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);

    destinationCodes.clear();
    switch (choice) {
        case 1:
            destinationCodes.push_back(readAirportCode());
            break;
        case 2:
            destinationCodes.push_back(manager.getAirportCode(readAirportName()));
            break;
        case 3:{
            string country = readCountry();
            string city = readCity(country);
            destinationCodes = manager.getAirportsCountryCity(country, city);
            break;
        }
        case 4:
            destinationCodes = manager.getAirportsCoordinates(readCoordinates());
            break;
        case 0:
            clear();
            header();
            return;
    }
    std::cout << "\n";
    flightFilterMenu();
    flightDestinationMenu();
}

/**
 * @brief Prints the flight filter menu.
 */
void Interface::flightFilterMenu() {
    std::vector<std::string> options =
            {"Back",
             "Process Operation",
             "Add Airline Preference",
             "Add Airline Restriction",
             "Add Airport Restriction",
             "Add City Restriction",
             "Add Country Restriction",
             "Clear Filters",
             "Choose the Filters:"};
    printOptions(options);

    printFilters();

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            manager.bestFlightOption(&sourceCodes, &destinationCodes, &airlinePreferences, &airlineRestrictions, &airportRestrictions);
            outputWait();
            clear();
            header();
            break;
        case 2:
            airlinePreferences.push_back(readAirline());
            break;
        case 3:
            airlineRestrictions.push_back(readAirline());
            break;
        case 4:
            airportRestrictions.push_back(readAirportCode());
            break;
        case 5:{
            string country = readCountry();
            string city = readCity(country);
            vector<string> airports = manager.getAirportsCountryCity(country, city);
            for (const string &airport : airports){
                airportRestrictions.push_back(airport);
            }
            break;
        }
        case 6:{
            string country = readCountry();
            vector<string> airports = manager.getAirportsCountry(country);
            for (const string &airport : airports){
                airportRestrictions.push_back(airport);
            }
            break;
        }
        case 7:
            airlinePreferences.clear();
            airlineRestrictions.clear();
            airportRestrictions.clear();
            break;
        case 0:
            clear();
            header();
            return;
    }
    cout << "\n";
    flightFilterMenu();
}