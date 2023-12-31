//
// Created by rodrigo on 12/27/23.
//

#ifndef AED2324_PRJ2_G15_INTERFACE_H
#define AED2324_PRJ2_G15_INTERFACE_H

#include "Manager.h"

/**
 * @brief Class that defines the graphical Interface. The interface contains the Manager which processes the user requests.
 */
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
    std::string readAirportName();
    std::string readCity();
    std::string readCityOptional();
    std::string readCountry();
    pair<double, double> readCoordinates();
    int readNumber();

    void mainMenu();

    void statisticsMenu();
    void airlineStatisticsMenu();
    void airportStatisticsMenu();
    void airportInformationMenu();
    void flightStatisticsMenu();
    void locationStatisticsMenu();

    void flightSourceMenu();
    void flightDestinationMenu();
    void flightFilterMenu();

    /// Vector of available source airports for a trip.
    vector<string> sourceCodes;
    /// Vector of available destination airports for a trip.
    vector<string> destinationCodes;
    /// Vector of user preferred airlines for a trip.
    vector<string> airlinePreferences;
    /// Vector of user restricted airlines for a trip.
    vector<string> airlineRestrictions;
    /// Vector of user restricted airports for a trip.
    vector<string> airportRestrictions;
};

bool stringIsNumeric(const string &s);

#endif //AED2324_PRJ2_G15_INTERFACE_H
