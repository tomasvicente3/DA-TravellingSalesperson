#include "coordinates.h"

Coordinates::Coordinates(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

/**
 * Calculates the haversine distance between this and another point
 * Time Complexity: O(1)
 * @param point - Coordinates of second point
 * @return Haversine distance between two points
 */
double Coordinates::distanceTo(Coordinates point) const {
    double lat1 = this->latitude, lat2 = point.latitude;
    double lon1 = this->longitude, lon2 = point.longitude;

    if ((lat1 == 0 && lon1 == 0) || (lat2 == 0 && lon2 == 0)) return -1;

    // distance between latitudes
    // and longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

double Coordinates::getLatitude() const {
    return latitude;
}

void Coordinates::setLatitude(double latitude) {
    Coordinates::latitude = latitude;
}

double Coordinates::getLongitude() const {
    return longitude;
}

void Coordinates::setLongitude(double longitude) {
    Coordinates::longitude = longitude;
}

bool Coordinates::operator==(const Coordinates &rhs) const {
    return latitude == rhs.latitude &&
           longitude == rhs.longitude;
}

bool Coordinates::operator!=(const Coordinates &rhs) const {
    return !(rhs == *this);
}
