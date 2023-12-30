#include <iostream>
#include "Interface.h"

void Interface::header() {
    std::cout << BOLD << "┌─────────────────────────────────────────────────────────────────────────────────────────────────┐\n"
              << "├" << RESET << "─────────────────────────────────── " << BOLD << BLUE << "Flight Manager (Group 15)" << RESET <<" ───────────────────────────────────" << BOLD << "┤\n\n" << RESET;
}

void Interface::footer() {
    std::cout << BOLD << "├" << RESET << "─────────────────────────────────── " << BOLD << BLUE << "Flight Manager (Group 15)" << RESET << " ───────────────────────────────────" << BOLD "┤\n"
              << "└─────────────────────────────────────────────────────────────────────────────────────────────────┘\n" << RESET;
}

void Interface::clear() {
    system("clear");
}

void Interface::outputWait() {
    cin.clear();
    cout << "\n\n                                    " << FAINT << "< Press " << RESET << BOLD << "ENTER" << RESET << FAINT << " to Continue >" << RESET;
    cin.ignore();
}

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

void Interface::exitMenu() {
    clear();

    std::cout << "\n                               " << BOLD << UNDERLINE << "Thanks for using our Flight Manager!" << RESET << "\n\n";

    footer();
    exit(0);
}

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

void Interface::printOptions(const std::vector<std::string> &options) {
    std::cout << "     " << BOLD << options[options.size()-1] << RESET << "\n";
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        std::cout << CYAN << BOLD << " [" << idx << "] " << RESET << options[idx] << '\n';
    }
    std::cout << RED << " [0] " << RESET << options[0] << '\n';
}

void Interface::printSelected(const std::string &s) {
    std::cout << "     " <<  BOLD << "> " << YELLOW << s << RESET << " selected\n";
}

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

bool Interface::validOption(unsigned long size, const std::string &choice) {
    return choice.size() == 1 && "0" <= choice && choice <= to_string(size-2);
}


std::string Interface::readAirline() {
    string choice;
    do {
        std::cout << FAINT << "  Airline Code" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirline(choice));
    return choice;
}

std::string Interface::readAirportCode() {
    string choice;
    do {
        std::cout << FAINT << "  Airport Code" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirport(choice));
    return choice;
}

std::string Interface::readAirportName() {
    string choice;
    do {
        std::cout << FAINT << "  Airport Name" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateAirportName(choice));
    return choice;
}

std::string Interface::readCity(string country) {
    string choice;
    do {
        std::cout << FAINT << "  City" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCity(choice, country));
    return choice;
}

std::string Interface::readCityOptional() {
    string choice;
    std::cout << FAINT << "  City (Optional)" << RESET << ": ";
    cin.clear();
    getline(cin, choice, '\n');
    return choice;
}

std::string Interface::readCountry() {
    string choice;
    do {
        std::cout << FAINT << "  Country" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!manager.validateCountry(choice));
    return choice;
}

pair<double, double> Interface::readCoordinates() {
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

int Interface::readNumber() {
    string choice;
    do {
        std::cout << FAINT << "  Number" << RESET << ": ";
        cin.clear();
        getline(cin, choice, '\n');
    } while (!stringIsNumeric(choice));
    return stoi(choice);
}

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

void Interface::flightFilterMenu() {
    std::vector<std::string> options =
            {"Back",
             "Process Operation",
             "Airline Filters",
             "Airport Filters",
             "Clear Filters",
             "Choose the Filters:"};
    printOptions(options);

    int choice = readOption(int(options.size()));

    printSelected(options[choice]);
    switch (choice) {
        case 1:
            manager.bestFlightOption(&sourceCodes, &destinationCodes, &airportFilters, &airlineFilters);
            outputWait();
            clear();
            header();
            break;
        case 2:
            airlineFilters.push_back(readAirline());
            break;
        case 3:
            airportFilters.push_back(readAirportCode());
            break;
        case 4:
            airportFilters.clear();
            airlineFilters.clear();
            break;
        case 0:
            clear();
            header();
            return;
    }
    cout << "\n";
    flightFilterMenu();
}