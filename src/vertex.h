#ifndef TRAVELLINGSALESMAN_VERTEX_H
#define TRAVELLINGSALESMAN_VERTEX_H

#include <utility>
#include <iostream>
#include <queue>
#include <unordered_set>
#include "constants.h"
#include "coordinates.h"
#include "MutablePriorityQueue.h"

class Vertex {
  public:
    explicit Vertex(const unsigned int &id, Coordinates c = {0, 0});

    [[nodiscard]] unsigned int getId() const;

    [[nodiscard]] bool isVisited() const;

    [[nodiscard]] unsigned int getIndegree() const;

    [[nodiscard]] std::shared_ptr<Vertex> getPath() const;

    [[nodiscard]] double getDist() const;

    void setDist(const double newDist);

    void setId(const unsigned int &id);

    void setVisited(bool visited);

    void setIndegree(unsigned int indegree);


    void setPath(std::shared_ptr<Vertex> path);

    [[nodiscard]] const Coordinates &getCoordinates() const;

    void setCoordinates(const Coordinates &coordinates);

    double haversineDistance(const std::shared_ptr<Vertex> &other) const;

    bool operator<(const Vertex &rhs) const;

    friend class MutablePriorityQueue<Vertex>;
  protected:
    unsigned int id;                // identifier
    Coordinates coordinates;

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    unsigned int indegree = 0; // used by topsort
    double dist;

    std::shared_ptr<Vertex> path = nullptr; //Pointer to the preceding vertex
    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS
};

struct VertexPointerHash {
    std::size_t operator()(const std::shared_ptr<Vertex> &vertex) const {
        return std::hash<unsigned int>()(vertex->getId());
    }
};

struct VertexPointerEquals {
    bool operator()(const std::shared_ptr<Vertex> &vertex1, const std::shared_ptr<Vertex> &vertex2) const {
        return vertex1->getId() == vertex2->getId();
    }
};

typedef std::unordered_set<std::shared_ptr<Vertex>, VertexPointerHash, VertexPointerEquals> VertexPointerTable;


#endif //TRAVELLINGSALESMAN_VERTEX_H
