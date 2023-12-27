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
    static void footer();
    static void clear();
    static void outputWait();

    void init();
    void exitMenu();
    bool loadData();

    static void printOptions(const std::vector<std::string> &options);
    bool validOption(unsigned long size, const std::string &choice);
    int readOption(int i);
    void printSelected(const string &s);
    std::string readAirline();
    std::string readAirportCode();
    std::string readCity();
    std::string readCityOptional();
    std::string readCountry();
    std::string readCoordinates();
    int readNumber();

    void mainMenu();

    void statisticsMenu();
    void airlineStatisticsMenu();
    void airportStatisticsMenu();
    void airportInformationMenu();
    void flightStatisticsMenu();
    void locationStatisticsMenu();


};

bool stringIsNumeric(const string &s);

#endif //AED2324_PRJ2_G15_INTERFACE_H
