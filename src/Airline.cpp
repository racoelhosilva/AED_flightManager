#include "Airline.h"

bool Airline::operator==(const Airline &other) const {
    return code == other.code;
}

bool Airline::operator<(const Airline &other) const {
    return code < other.code;
}
