#include <iostream>
#include "Interface.h"

void Interface::header() {
    std::cout << "┌───────────────────────────────────────────────────────────────────────────────────┐\n"
              << "├──────────────────────────── \033[1;34mFlight Manager (Group 15)\033[0m ────────────────────────────┤\n\n";
}

void Interface::clear() {
    system("clear");
}

void Interface::inputWait() {
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

bool Interface::loadData() {
    std::cout << "\tLoading data (may take a while):\n";
    if (!manager.extractAirports("../data/airports.csv")) {
        return false;
    }
    std::cout << "- Airports loaded\n";
    if (!manager.extractAirlines("../data/airlines.csv")) {
        return false;
    }
    std::cout << "- Airlines loaded\n";
    if (!manager.extractFlights("../data/flights.csv")){
        return false;
    }
    std::cout << "- Flights loaded\n";
    return true;
}

void Interface::mainMenu() {
    clear();
    header();
    std::cout << "MAIN MENU STUFF" << std::endl;
    inputWait();
}
