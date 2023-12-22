#include "dataRepository.h"

using namespace std;

DataRepository::DataRepository() = default;

const VertexPointerTable &DataRepository::getVertices() const {
    return vertices;
}

void DataRepository::setVertices(const VertexPointerTable &vertices) {
    this->vertices = vertices;
}

/**
 * Adds a new entry to the unordered_set of vertices, creating the corresponding Vertex object
 * Time Complexity: O(1) (average case) | O(size(vertices)) (worst case)
 * @param id - Id of the Vertex to be created
 * @param latitude - Latitude of the Vertex to created
 * @param longitude - Longitude of the Vertex to be created
 * @param name - Name of the Vertex to be created
 * @return New Vertex object created
 */
std::shared_ptr<Vertex>
DataRepository::addVertexEntry(unsigned int id, double latitude, double longitude) {
    std::shared_ptr<Vertex> newVertex = std::make_shared<Vertex>(id, Coordinates(latitude, longitude));
    vertices.insert(newVertex);
    sumLatitude += latitude;
    sumLongitude += longitude;
    return newVertex;
}

/**
 * Finds the Vertex object with the given id
 * Time Complexity: O(1) (average case) | O(size(vertices)) (worst case)
 * @param id - Id of the Vertex to be returned
 * @return Pointer to the Vertex found, or nullptr if none was found
 */
shared_ptr<Vertex> DataRepository::findVertex(const unsigned int &id) {
    auto it = vertices.find(make_shared<Vertex>(id));
    return it != vertices.end() ? *it : nullptr;
}

double DataRepository::getAverageLatitude() {
    return vertices.empty() ? 0 : sumLatitude / (double) vertices.size();
}

double DataRepository::getAverageLongitude() {
    return vertices.empty() ? 0 : sumLongitude / (double) vertices.size();
}

/**
 * Returns the Vertex furthest away from the average Coordinates of vertices in the set
 * Time Complexity: O(nlog(n)), where n is the size of the vertices set
 * @return Reference to furthest Vertex
 */
Vertex &DataRepository::getFurthestVertex() {
    std::vector<std::shared_ptr<Vertex>> orderedvertices(vertices.begin(), vertices.end());
    Coordinates averageLocation(getAverageLatitude(), getAverageLongitude());
    std::sort(orderedvertices.begin(), orderedvertices.end(),
              [averageLocation](const std::shared_ptr<Vertex> &n1, const std::shared_ptr<Vertex> &n2) {
                  Coordinates c1(n1->getCoordinates().getLatitude(), n1->getCoordinates().getLongitude());
                  Coordinates c2(n2->getCoordinates().getLatitude(), n2->getCoordinates().getLongitude());
                  return averageLocation.distanceTo(c1) > averageLocation.distanceTo(c2);
              });
    return *orderedvertices[0];
}

/**
 * Clears all current data in DataRepository
 */
void DataRepository::clearData() {
    vertices = {};
    sumLongitude = 0;
    sumLatitude = 0;
}
