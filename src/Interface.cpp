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
    return choice.size() == 1 && "0" <= choice && choice < to_string(size-1);
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

std::string Interface::readCountry() {
    string choice;
    do {
        std::cout << "  Country: ";
        cin.clear();
        std::cin >> choice;
    } while (!manager.validateCountry(choice));
    return choice;
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
            mainMenu();
            break;
    }
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
            break;
        case 2: {
            string airportCode = readAirportCode();
            manager.listAirlinesAirport(airportCode);
            break;
        }
        case 3: {
            string country = readCountry();
            manager.listAirlinesCountry(country);
            break;
        }
        case 4: {
            string airline = readAirline();
            manager.airlineInfo(airline);
            break;
        }
        case 0:
            mainMenu();
            break;
    }
    airlineStatisticsMenu();
}
