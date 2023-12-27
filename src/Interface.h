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
    static void outputWait();

    void init();
    bool loadData();

    static void printOptions(const std::vector<std::string> &options);
    bool validOption(unsigned long size, const std::string &choice);
    void printSelected(const string &s);

    void mainMenu();



};


#endif //AED2324_PRJ2_G15_INTERFACE_H
