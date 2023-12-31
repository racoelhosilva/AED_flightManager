//
// Created by rodrigo on 12/29/23.
//

#ifndef AED2324_PRJ2_G15_COORDINATE_H
#define AED2324_PRJ2_G15_COORDINATE_H

/**
 * @brief Class that defines a geographical Coordinate. Each coordinate has a latitude and a longitude.
 */
class Coordinate {
private:
    double latitude;
    double longitude;

public:
    /// Default constructor. Sets latitude and longitude to 0.
    Coordinate() : latitude(0), longitude(0) {};
    /// All parameter constructor.
    Coordinate(double lat, double lon) : latitude(lat), longitude(lon) {};

    double getLatitude() const {
        return latitude;
    }
    void setLatitude(double lat) {
        latitude = lat;
    }
    double getLongitude() const {
        return longitude;
    }
    void setLongitude(double lon) {
        longitude = lon;
    }

    double distance(const Coordinate &c2);

    /// Equality operator. Returns true if both latitude and longitude are equal in both coordinates.
    bool operator==(const Coordinate& other) const {return latitude == other.getLatitude() && longitude == other.getLongitude();}
};



#endif //AED2324_PRJ2_G15_COORDINATE_H
