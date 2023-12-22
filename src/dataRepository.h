#ifndef TRAVELLINGSALESMAN_DATAREPOSITORY_H
#define TRAVELLINGSALESMAN_DATAREPOSITORY_H

#include <algorithm>
#include <iostream>
#include <optional>
#include <memory>
#include "vertex.h"
#include "coordinates.h"

class DataRepository {

private:
    VertexPointerTable vertices;
    double sumLatitude = 0;
    double sumLongitude = 0;

public:
    DataRepository();

    const VertexPointerTable &getVertices() const;

    void setVertices(const VertexPointerTable &vertices);

    std::shared_ptr<Vertex> findVertex(const unsigned int &id);

    double getAverageLatitude();

    double getAverageLongitude();

    Vertex &getFurthestVertex();

    std::shared_ptr<Vertex> addVertexEntry(unsigned int id, double latitude, double longitude);

    void clearData();
};


#endif //TRAVELLINGSALESMAN_DATAREPOSITORY_H
