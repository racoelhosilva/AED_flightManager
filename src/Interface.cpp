#include <iostream>
#include "Interface.h"

void Interface::header() {
    std::cout << "┌───────────────────────────────────────────────────────────────────────────────────┐\n"
              << "├──────────────────────────── \033[1;34mFlight Manager (Group 15)\033[0m ────────────────────────────┤\n\n";
}

void Interface::footer() {
    std::cout << "├──────────────────────────── \033[1;34mFlight Manager (Group 15)\033[0m ────────────────────────────┤\n"
              << "└───────────────────────────────────────────────────────────────────────────────────┘\n";
}

void Interface::clear() {
    system("clear");
}

void Interface::outputWait() {
    cin.ignore();
    cin.clear();
    cout << "                            \033[2m< Press \033[0m\033[1mENTER\033[0m\033[2m to Continue >\033[0m";
    cin.ignore();
}

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

void Interface::exitMenu() {
    clear();

    std::cout << "\n                        \033[1mThanks for using our Flight Manager!\033[0m\n\n";

    footer();
    exit(0);
}

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

void Interface::printOptions(const std::vector<std::string> &options) {
    std::cout << "     \033[1m" << options[options.size()-1] << "\033[0m\n";
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        std::cout << " \033[1;36m[" << idx << "]\033[0m " << options[idx] << '\n';
    }
    std::cout << " \033[31m[0]\033[0m " << options[0] << '\n';
}

void Interface::printSelected(const std::string &s) {
    std::cout << "     \033[1m>  \033[33m" << s << "\033[0m selected\n\n";
}

int Interface::readOption(int max) {
    string choice;
    do {
        std::cout << "  Option: ";
        cin.clear();
        std::cin >> choice;
    } while (!validOption(max, choice));
    return stoi(choice);
}

bool Interface::validOption(unsigned long size, const std::string &choice) {
    return choice.size() == 1 && "0" <= choice && choice <= to_string(size-2);
}


std::string Interface::readAirline() {
    string choice;
    do {
        std::cout << "  Airline: ";
        cin.clear();
        std::cin >> choice;
    } while (!manager.validateAirline(choice));
    return choice;
}

std::string Interface::readAirportCode() {
    string choice;
    do {
        std::cout << "  Airport Code: ";
        cin.clear();
        std::cin >> choice;
    } while (!manager.validateAirport(choice));
    return choice;
}

std::string Interface::readCity() {
    string choice;
    do {
        std::cout << "  City: ";
        cin.clear();
        std::cin >> choice;
    } while (!manager.validateCountry(choice));
    return choice;
}

std::string Interface::readCityOptional() {
    string choice;
    std::cout << "  City (Optional): ";
    cin.clear();
    std::cin >> choice;
    return choice;
}

std::string Interface::readCountry() {
    string choice;
    do {
        std::cout << "  Country: ";
        cin.clear();
        std::cin >> choice;
    } while (!manager.validateCountry(choice));
    return choice;
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
        std::cout << "  Number: ";
        cin.clear();
        std::cin >> choice;
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
             "Cities",
             "Countries",
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
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
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
        case 2: {
            string airportCode = readAirportCode();
            manager.listAirlinesAirport(airportCode);
            outputWait();
            break;
        }
        case 3: {
            string country = readCountry();
            manager.listAirlinesCountry(country);
            outputWait();
            break;
        }
        case 4: {
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
            manager.listAirportsCountry(country, city);
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
             "Reachable Airports with N Layovers",
             "Reachable Cities",
             "Reachable Cities with N Layovers",
             "Reachable Countries",
             "Reachable Countries with N Layovers",
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
            break;
        }
        case 0:
            return;
    }
    flightStatisticsMenu();
}