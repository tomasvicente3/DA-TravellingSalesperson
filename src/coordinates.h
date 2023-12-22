//
// Created by rita on 30-05-2023.
//

#ifndef TRAVELLINGSALESMAN_COORDINATES_H
#define TRAVELLINGSALESMAN_COORDINATES_H

#include <cmath>

class Coordinates {
private:
    double latitude;
    double longitude;
public:
    Coordinates(double latitude, double longitude);

    [[nodiscard]] double getLatitude() const;

    void setLatitude(double latitude);

    [[nodiscard]] double getLongitude() const;

    void setLongitude(double longitude);

    [[nodiscard]] double distanceTo(Coordinates c) const;

    bool operator==(const Coordinates &rhs) const;

    bool operator!=(const Coordinates &rhs) const;
};


#endif //TRAVELLINGSALESMAN_COORDINATES_H
