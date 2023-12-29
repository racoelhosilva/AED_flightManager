//
// Created by rodrigo on 12/29/23.
//

#include <cmath>
#include "Coordinate.h"

double Coordinate::distance(const Coordinate &c2){
    double dLat = (c2.getLatitude() - getLatitude()) * M_PI / 180.0;
    double dLon = (c2.getLongitude() - getLongitude()) * M_PI / 180.0;

    // convert to radians
    double lat1 = (getLatitude()) * M_PI / 180.0;
    double lat2 = (c2.getLatitude()) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double earthRadius = 6371;
    double c = 2 * asin(sqrt(a));
    return earthRadius * c;
}