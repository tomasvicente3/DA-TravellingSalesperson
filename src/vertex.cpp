#include "vertex.h"

Vertex::Vertex(const unsigned int &id, Coordinates c) : id(id), coordinates(c) {}

unsigned int Vertex::getId() const {
    return this->id;
}

bool Vertex::isVisited() const {
    return this->visited;
}

unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

std::shared_ptr<Vertex> Vertex::getPath() const {
    return this->path;
}

void Vertex::setId(const unsigned int &id) {
    this->id = id;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}


void Vertex::setPath(std::shared_ptr<Vertex> path) {
    this->path = path;
}

double Vertex::haversineDistance(const std::shared_ptr<Vertex> &other) const{
    return coordinates.distanceTo(other->getCoordinates());
}

const Coordinates &Vertex::getCoordinates() const {
    return coordinates;
}

void Vertex::setCoordinates(const Coordinates &newCoordinates) {
    Vertex::coordinates = newCoordinates;
}

double Vertex::getDist() const {
    return this->dist;
}

void Vertex::setDist(const double newDist) {
    this->dist = newDist;
}

bool Vertex::operator<(const Vertex &rhs) const {
    if (dist != rhs.getDist()){
        return dist < rhs.getDist();
    }
    return id < rhs.getId();
}
