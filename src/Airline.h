#ifndef AED2324_PRJ2_G15_AIRLINE_H
#define AED2324_PRJ2_G15_AIRLINE_H

#include <string>

class Airline {
    private:
        std::string code;
        std::string name;
        std::string callsign;
        std::string country;
    public:
        Airline() {code = "noCode";}
        Airline(std::string code_) : code(code_) {};
        Airline(std::string code_, std::string name_, std::string callsign_, std::string country_) : code(code_), name(name_), callsign(callsign_), country(country_) {}
        std::string getCode() const {return code;}
        std::string getName() const {return name;}
        std::string getCallsign() const {return callsign;}
        std::string getCountry() const {return country;}
};


#endif //AED2324_PRJ2_G15_AIRLINE_H