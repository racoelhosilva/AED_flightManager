#ifndef AED2324_PRJ2_G15_AIRLINE_H
#define AED2324_PRJ2_G15_AIRLINE_H

#include <string>

/**
     * @brief Class that defines an Airline. Each airline has a unique code, a name, a callsign (optional) and a country.
*/

class Airline {
    private:
        std::string code;
        std::string name;
        std::string callsign;
        std::string country;
    public:
        /// Default constructor. Sets airline code to "noCode".
        Airline() {code = "noCode";}
        /// Code constructor
        Airline(std::string code_) : code(code_) {};
        /// All parameter constructor
        Airline(std::string code_, std::string name_, std::string callsign_, std::string country_) : code(code_), name(name_), callsign(callsign_), country(country_) {}

        std::string getCode() const {return code;}
        std::string getName() const {return name;}
        std::string getCallsign() const {return callsign;}
        std::string getCountry() const {return country;}

        /// Equality operator. Returns true if airports have the same code.
        bool operator==(const Airline& other) const;
        /// Comparison operator. Compares airport codes alphabetically.
        bool operator<(const Airline& other) const;
};


#endif //AED2324_PRJ2_G15_AIRLINE_H