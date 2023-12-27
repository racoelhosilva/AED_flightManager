#ifndef AED2324_PRJ2_G15_AIRPORT_H
#define AED2324_PRJ2_G15_AIRPORT_H

#include <string>

class Airport {
    private:
        std::string code;
        std::string name;
        std::string city;
        std::string country;
        double latitude;
        double longitude;
    public:
        Airport(std::string code_, std::string name_, std::string city_, std::string country_, double latitude_, double longitude_) :
            code(code_), name(name_), city(city_), country(country_), latitude(latitude_), longitude(longitude_) {}
        std::string getCode() const {return code;}
        std::string getName() const {return name;}
        std::string getCity() const {return city;}
        std::string getCountry() const {return country;}
        double getLatitiude() const {return latitude;}
        double getLongitude() const {return longitude;}
};


#endif //AED2324_PRJ2_G15_AIRPORT_H