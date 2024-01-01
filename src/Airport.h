#ifndef AED2324_PRJ2_G15_AIRPORT_H
#define AED2324_PRJ2_G15_AIRPORT_H

#include <string>
#include "Coordinate.h"

/**
 * @brief Class that defines an Airport. Each airport contains a unique code, a name, a city a country, and its coordinates.
 */
class Airport {
    private:
        std::string code;
        std::string name;
        std::string city;
        std::string country;
        double latitude;
        double longitude;
        Coordinate coordinates;
    public:
        /// Default constructor. Sets airport code to "noCode".
        Airport() {code = "noCode";}
        /// Code constructor
        Airport(std::string code_) : code(code_) {}
        /// All parameter constructor
        Airport(std::string code_, std::string name_, std::string city_, std::string country_, Coordinate coords) :
            code(code_), name(name_), city(city_), country(country_), coordinates(coords) {}

        std::string getCode() const {return code;}
        std::string getName() const {return name;}
        std::string getCity() const {return city;}
        std::string getCountry() const {return country;}
        Coordinate getCoordinate() const {return coordinates;}
        double getLatitiude() const {return coordinates.getLatitude();}
        double getLongitude() const {return coordinates.getLongitude();}

        /// Equality operator. Returns true if all of the airports' elements are equal.
        bool operator==(const Airport& other) const;
};


#endif //AED2324_PRJ2_G15_AIRPORT_H