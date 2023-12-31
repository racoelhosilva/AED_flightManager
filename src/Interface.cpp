#include <iostream>
#include <climits>
#include "Interface.h"

/**
 * @brief Prints the menu header.
 */
void Interface::header() {
    std::cout << "┌───────────────────────────────────────────────────────────────────────────────────┐\n"
              << "├──────────────────────────── \033[1;34mFlight Manager (Group 15)\033[0m ────────────────────────────┤\n\n";
}

/**
 * @brief Prints the menu footer.
 */
void Interface::footer() {
    std::cout << "├──────────────────────────── \033[1;34mFlight Manager (Group 15)\033[0m ────────────────────────────┤\n"
              << "└───────────────────────────────────────────────────────────────────────────────────┘\n";
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
    cout << "                            \033[2m< Press \033[0m\033[1mENTER\033[0m\033[2m to Continue >\033[0m";
    cin.ignore();
}

/**
 * @brief Initializes the interface.
 */
void Interface::init() {
    header();
    this->manager = Manager();
    if (loadData()){
        std::cout << "Data Loaded... Starting flight manager!\n";
        this->mainMenu();
    }
    else {
        std::cout << "Problem loading data... Exiting!\n";
    }
}

/**
 * @brief Prints the exit menu.
 */
void Interface::exitMenu() {
    clear();

    std::cout << "\n                        \033[1mThanks for using our Flight Manager!\033[0m\n\n";

    footer();
    exit(0);
}

/**
 * @brief Requests the loading of the data from the manager.
 * @return True if the operation was successful.
 */
bool Interface::loadData() {
    std::cout << "        Loading data (this may take a while):\n";
    if (!manager.extractAirports("../data/airports.csv")) {
        return false;
    }
    std::cout << " - Airports loaded\n";
    if (!manager.extractAirlines("../data/airlines.csv")) {
        return false;
    }
    std::cout << " - Airlines loaded\n";
    if (!manager.extractFlights("../data/flights.csv")){
        return false;
    }
    std::cout << " - Flights loaded\n";
    return true;
}

/**
 * @brief Prints available options.
 * @param options - vector of available options.
 */
void Interface::printOptions(const std::vector<std::string> &options) {
    std::cout << "     \033[1m" << options[options.size()-1] << "\033[0m\n";
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        std::cout << " \033[1;36m[" << idx << "]\033[0m " << options[idx] << '\n';
    }
    std::cout << " \033[31m[0]\033[0m " << options[0] << '\n';
}

/**
 * @brief Prints selected option.
 * @param s - Selected option.
 */
void Interface::printSelected(const std::string &s) {
    std::cout << "     \033[1m>  \033[33m" << s << "\033[0m selected\n\n";
}

/**
 * @brief Reads and filters the user chosen option.
 * @param max - Maximum number of options.
 * @return - Number of the chosen option.
 */
int Interface::readOption(int max) {
    string choice;
    do {
        std::cout << "  Option: ";
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
        std::cout << "  Airline Code: ";
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
        std::cout << "  Airport Code: ";
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
        std::cout << "  Airport Name: ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirportName(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted cities.
 * @return - Name of the inputted city.
 */
std::string Interface::readCity() {
    string choice;
    do {
        std::cout << "  City: ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCity(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted cities.
 * @return - Name of the inputted city.
 */
std::string Interface::readCityOptional() {
    string choice;
    std::cout << "  City (Optional): ";
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
        std::cout << "  Country: ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCountry(choice));
    return choice;
}

/**
 * @brief Reads and requests the manager to validate user inputted coordinates.
 * @return - Inputted coordinates.
 */
pair<double, double> Interface::readCoordinates() {
    double latitude;
    do {
        std::cout << "  Latitude: ";
        cin.clear();
        std::cin >> latitude;
    } while (cin.fail() || latitude < -90 || latitude > 90);
    double longitude;
    do {
        std::cout << "  Longitude: ";
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
        std::cout << "  Number: ";
        cin.clear();
        std::cin >> choice;
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
             "Flights by Country/City",
             "Number of Flights by Country/City",
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
            manager.listFlightsCountryCity(country, city);
            outputWait();
            break;
        }
        case 6: {
            string country = readCountry();
            string city = readCityOptional();
            manager.numberFlightsCountryCity(country, city);
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
            string city = readCity();
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
        case 3:
            sourceCodes = manager.getAirportsCountryCity(readCountry(), readCity());
            break;
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
        case 3:
            destinationCodes = manager.getAirportsCountryCity(readCountry(), readCity());
            break;
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
             "Clear Filters",
             "Choose the Filters:"};
    printOptions(options);

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
        case 5:
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