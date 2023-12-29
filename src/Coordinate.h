//
// Created by rodrigo on 12/29/23.
//

#ifndef AED2324_PRJ2_G15_COORDINATE_H
#define AED2324_PRJ2_G15_COORDINATE_H


class Coordinate {
private:
    double latitude;
    double longitude;

public:
    Coordinate() : latitude(0), longitude(0) {};
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
    bool operator==(const Coordinate& other) const {return latitude == other.getLatitude() && longitude == other.getLongitude();}
};



#endif //AED2324_PRJ2_G15_COORDINATE_H
