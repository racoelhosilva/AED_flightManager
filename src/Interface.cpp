//
// Created by rodrigo on 12/27/23.
//

#include <iostream>
#include "Interface.h"

void Interface::init() {
    this->manager = Manager();
    this->mainMenu();
}

void Interface::mainMenu() {
    std::cout << "It's working!" << std::endl;
}
