#ifndef TRAVELLINGSALESMAN_MENU_H
#define TRAVELLINGSALESMAN_MENU_H

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>
#include <unordered_set>
#include <chrono>
#include "graph.h"
#include "dataRepository.h"

class Menu {
  private:
    DataRepository dataRepository;
    Graph graph;
    unsigned static const COLUMN_WIDTH;
    unsigned static const COLUMNS_PER_LINE;

    template<typename T>
    T random(T range_from, T range_to) {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<T> distr(range_from, range_to);
        return distr(generator);
    }

  public:
    Menu();

    void extractNodesFile(const std::string &filename);

    void extractEdgesFile(const std::string &filename, bool hasDescriptors = true, bool hasLabels = false);

    void extractFileInfo(const std::string &edgesFilename, const std::string &nodesFilename = "");

    void mainMenu();

    static bool checkInput(unsigned int checkLength = 0);

    unsigned int backtrackingMenu();

    unsigned int triangularApproximationMenu();

    unsigned int heuristicMenu();

    void printTime(double time);
};


#endif //TRAVELLINGSALESMAN_MENU_H

