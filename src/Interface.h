//
// Created by rodrigo on 12/27/23.
//

#ifndef AED2324_PRJ2_G15_INTERFACE_H
#define AED2324_PRJ2_G15_INTERFACE_H


#include "Manager.h"

class Interface {
private:
    Manager manager;

public:
    static void header();
    static void clear();
    static void inputWait();

    void init();
    bool loadData();

    void mainMenu();


};


#endif //AED2324_PRJ2_G15_INTERFACE_H
