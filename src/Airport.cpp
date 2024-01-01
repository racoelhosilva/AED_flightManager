#include "Airport.h"

bool Airport::operator==(const Airport &other) const {
    return code == other.code && name == other.name && city == other.city && country == other.country && latitude == other.latitude && longitude == other.longitude;
}
