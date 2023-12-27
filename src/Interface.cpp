#include <iostream>
#include "Interface.h"

void Interface::init() {
    this->manager = Manager();
    this->mainMenu();
}

void Interface::mainMenu() {
    manager.extractAirports("../data/airports.csv");
    manager.extractAirlines("../data/airlines.csv");
    manager.extractFlights("../data/flights.csv");
    std::cout << "It's working!" << std::endl;
}
