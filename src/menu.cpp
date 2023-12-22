#include "menu.h"

using namespace std;

unsigned const Menu::COLUMN_WIDTH = 50;
unsigned const Menu::COLUMNS_PER_LINE = 3;

Menu::Menu() = default;


/**
 * Delegates extracting file info, calling the appropriate functions for each file
 * Time Complexity: O(n*v), where n is the number of lines of edgesFilename and v is the number of lines in nodesFilename
 */
void Menu::extractFileInfo(const std::string &edgesFilename, const std::string &nodesFilename) {
    if (edgesFilename.contains("tourism")) {
        extractEdgesFile(edgesFilename, true, true);
    } else if (edgesFilename.contains("Extra_Fully_Connected_Graphs")) {
        extractEdgesFile(edgesFilename, false);
    } else extractEdgesFile(edgesFilename);
    if (!nodesFilename.empty()) {
        extractNodesFile(nodesFilename);
    }
}

/**
 * Checks if the input given by the user is appropriate or not
 * Time Complexity: O(1)
 * @param checkLength - Integer indicating if the length of the input should be checked or not, and, if so, its valid max length
 * @return Returns true if the input is appropriate and false if it isn't
 */
bool Menu::checkInput(unsigned int checkLength) {

    /*
    checkLength = 0 Don't check length
    checkLength = 1 Check for length 1
    checkLength = 2 Check for max length 2
    ...
    */

    if (!cin) // User didn't input what expected
    {
        cin.clear(); // Reset failbit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Skip bad input
        cout << "Please enter an appropriate input." << endl;
        return false;
    }

    if (checkLength > 0) {
        string line;
        getline(cin, line);
        if (line.length() >= checkLength) {
            cout << "Please enter an appropriate input." << endl;
            return false;
        }
    }
    return true;
}


/**
 * Outputs main menu screen and calls other menu screens according to user input
 */
void Menu::mainMenu() {

    unsigned char commandIn = '\0';
    string line;

    while (commandIn != 'q') {
        if (commandIn == '\0') { //If program just started or returned from a different menu, print header

            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "ROUTING OPTIMI";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "ZATION SYSTEM" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Backtracking Algorithm: [1]" << setw(COLUMN_WIDTH)
                 << "Triangular Approximation Algorithm: [2]" << setw(COLUMN_WIDTH)
                 << "Nearest Insertion Heuristic: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;
        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                commandIn = backtrackingMenu();
                break;
            }
            case '2': {
                commandIn = triangularApproximationMenu();
                break;
            }
            case '3': {
                commandIn = heuristicMenu();
                break;
            }
            case 'q': {
                cout << "Thank you for using our Routing for Ocean Shipping and Urban Deliveries System!";
                break;
            }
            default: {
                cout << "Please press one of listed keys." << endl;
                break;
            }
        }
    }
}

/**
 * Extracts and stores the information of an edges file
 * Time Complexity: 0(n) (average case) | O(n²) (worst case), where n is the number of lines of the file
 */
void Menu::extractEdgesFile(const std::string &filename, bool hasDescriptors, bool hasLabels) {

    ifstream edges(filename);

    string currentParam, currentLine;
    string originName, destinationName;
    unsigned int originId, destinationId;
    double distance;

    int counter = 0;

    vector<vector<double>> distances;

    if (hasDescriptors) getline(edges, currentParam); //Ignore first line with just descriptors

    while (getline(edges, currentLine, '\n')) {
        currentLine.erase(currentLine.end() - 1); //Remove \r
        istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    originId = stoul(currentParam);
                    break;
                }
                case 1: {
                    destinationId = stoul(currentParam);
                    break;
                }
                case 2: {
                    distance = stod(currentParam);
                    if (!hasLabels) counter = 0;
                    break;
                }
                case 4: {
                    counter = 0;
                    break;
                }
            }
            if (counter == 0) {
                graph.addVertex(originId);
                graph.addVertex(destinationId);
                graph.addBidirectionalEdge(originId, destinationId, distance);
            }
        }
    }
}


/**
 * Extracts and stores the information of a vertices file
 * Time Complexity: 0(n) (average case) | O(n²) (worst case), where n is the number of lines of the file
 */
void Menu::extractNodesFile(const std::string &filename) {
    {
        ifstream nodes(filename);

        string currentParam, currentLine;
        unsigned int id;
        double longitude, latitude;

        int counter = 0;

        getline(nodes, currentParam); //Ignore first line with just descriptors

        while (getline(nodes, currentLine)) {
            currentLine.erase(currentLine.end() - 1); //Remove \r
            istringstream iss(currentLine);
            while (getline(iss, currentParam, ',')) {
                switch (counter++) {
                    case 0: {
                        id = stoul(currentParam);
                        break;
                    }
                    case 1: {
                        longitude = stod(currentParam);
                        break;
                    }
                    case 2: {
                        latitude = stod(currentParam);
                        counter = 0;
                        break;
                    }
                }
                if (counter == 0) {
                    auto vertex = graph.findVertex(id);
                    vertex->setCoordinates({latitude, longitude});
                    dataRepository.addVertexEntry(id, latitude, longitude);
                }
            }
        }
    }
}

/**
 * Prints the time past during the algorithm's execution
 * @param time - Time generated in milliseconds
 */
void Menu::printTime(double time) {
    if (time > 1000.0){
        double seconds = time / 1000.0;
        std::cout << "Algorithm execution time: " << seconds << " seconds" << std::endl;
    }
    else std::cout << "Algorithm execution time: " << time << " milliseconds" << std::endl;
}

/**
 * Outputs backtracking algorithm menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned int Menu::backtrackingMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        //Header
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "BACKTRACKIN";
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "G ALGORITHM" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Shipping: [1]" << setw(COLUMN_WIDTH)
             << "Stadiums: [2]" << setw(COLUMN_WIDTH) << "Tourism: [3]"
             << endl;
        cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;

        cout << endl << "Please select the problem for which you'd like to execute the backtracking algorithm: ";
        cin >> commandIn;

        std::string nodesFilePath, edgesFilePath;

        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                edgesFilePath = "../dataset/Toy-Graphs/shipping.csv";
                break;
            }
            case '2': {
                edgesFilePath = "../dataset/Toy-Graphs/stadiums.csv";
                break;
            }
            case '3': {
                edgesFilePath = "../dataset/Toy-Graphs/tourism.csv";
                break;
            }
            case 'b': {
                return '\0';
            }
            case 'q': {
                cout << "Thank you for using our Routing for Ocean Shipping and Urban Deliveries System!" << endl;
                break;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }

        if (!edgesFilePath.empty()) {
            graph.clearGraph();
            dataRepository.clearData();
            extractFileInfo(edgesFilePath, nodesFilePath);
            cout << endl << "Calculating..." << endl;

            std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

            auto result = graph.tspBT();

            std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = endTime - startTime;
            double milliseconds = duration.count();
            printTime(milliseconds);

            cout << "TOUR LENGTH: " << fixed << setprecision(2) << result.first << endl;

            if (graph.getNumVertex() <= 25) {
                graph.printTour(result.second);
            }
        }
    }
    return commandIn;
}


/**
 * Outputs triangular approximation heuristic menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */

unsigned int Menu::triangularApproximationMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        //Header
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "TRIANGULAR APPRO";
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "XIMATION HEURISTIC" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Stadiums: [1]" << setw(COLUMN_WIDTH) << "Tourism: [2]" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 25: [3]" << setw(COLUMN_WIDTH)
             << "Connected Graph 50: [4]" << setw(COLUMN_WIDTH)
             << "Connected Graph 75: [5]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 100: [6]" << setw(COLUMN_WIDTH)
             << "Connected Graph 200: [7]" << setw(COLUMN_WIDTH)
             << "Connected Graph 300: [8]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 400: [9]" << setw(COLUMN_WIDTH) << setfill(' ')
             << "Connected Graph 500: [A]" << setw(COLUMN_WIDTH)
             << "Connected Graph 600: [C]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 700: [D]" << setw(COLUMN_WIDTH) << setfill(' ')
             << "Connected Graph 800: [E]" << setw(COLUMN_WIDTH)
             << "Connected Graph 900: [F]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Real World Graph 1: [G]" << setw(COLUMN_WIDTH) << setfill(' ')
             << "Real World Graph 2: [H]" << setw(COLUMN_WIDTH)
             << "Real World Graph 3: [I]" << endl;
        cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;

        cout << endl
             << "Please select the problem for which you'd like to execute the triangular approximation algorithm: ";
        cin >> commandIn;

        if (commandIn != 'q' && commandIn != 'b') commandIn = toupper(commandIn);
        std::string nodesFilePath, edgesFilePath;

        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                edgesFilePath = "../dataset/Toy-Graphs/stadiums.csv";
                break;
            }
            case '2': {
                edgesFilePath = "../dataset/Toy-Graphs/tourism.csv";
                break;
            }
            case '3': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_25.csv";
                break;
            }
            case '4': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_50.csv";
                break;
            }
            case '5': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_75.csv";
                break;
            }
            case '6': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_100.csv";
                break;
            }
            case '7': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_200.csv";
                break;
            }
            case '8': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_300.csv";
                break;
            }
            case '9': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_400.csv";
                break;
            }
            case 'A': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_500.csv";
                break;
            }
            case 'C': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_600.csv";
                break;
            }
            case 'D': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_700.csv";
                break;
            }
            case 'E': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_800.csv";
                break;
            }
            case 'F': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_900.csv";
                break;
            }
            case 'G': {
                edgesFilePath = "../dataset/Real-world-Graphs/graph1/edges.csv";
                nodesFilePath = "../dataset/Real-world-Graphs/graph1/nodes.csv";
                break;
            }
            case 'H': {
                edgesFilePath = "../dataset/Real-world-Graphs/graph2/edges.csv";
                nodesFilePath = "../dataset/Real-world-Graphs/graph2/nodes.csv";
                break;
            }
            case 'I': {
                edgesFilePath = "../dataset/Real-world-Graphs/graph3/edges.csv";
                nodesFilePath = "../dataset/Real-world-Graphs/graph3/nodes.csv";
                break;
            }
            case 'b': {
                return '\0';
            }
            case 'q': {
                cout << "Thank you for using our Routing for Ocean Shipping and Urban Deliveries System!" << endl;
                break;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }

        if (!edgesFilePath.empty()) {
            cout << endl << "Loading graph..." << endl;
            graph.clearGraph();
            dataRepository.clearData();
            extractFileInfo(edgesFilePath, nodesFilePath);

            cout << "Calculating..." << endl;

            std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

            graph.triangularTSPTour();

            std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = endTime - startTime;
            double milliseconds = duration.count();
            printTime(milliseconds);

            cout << endl << "TOUR LENGTH: " << fixed << setprecision(2) << graph.getTourDistance() << endl;

            if (graph.getNumVertex() <= 25) {
                graph.printTour();
            }
        }
    }
    return commandIn;
}


/**
 * Outputs nearest insertion heuristic menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned int Menu::heuristicMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        //Header
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "NEAREST IN";
        cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "SERTION" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Stadiums: [1]" << setw(COLUMN_WIDTH) << "Tourism: [2]" << endl;

        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 25: [3]" << setw(COLUMN_WIDTH)
             << "Connected Graph 50: [4]" << setw(COLUMN_WIDTH)
             << "Connected Graph 75: [5]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 100: [6]" << setw(COLUMN_WIDTH)
             << "Connected Graph 200: [7]" << setw(COLUMN_WIDTH)
             << "Connected Graph 300: [8]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 400: [9]" << setw(COLUMN_WIDTH) << setfill(' ')
             << "Connected Graph 500: [A]" << setw(COLUMN_WIDTH)
             << "Connected Graph 600: [C]" << endl;
        cout << setw(COLUMN_WIDTH) << setfill(' ') << "Connected Graph 700: [D]" << setw(COLUMN_WIDTH) << setfill(' ')
             << "Connected Graph 800: [E]" << setw(COLUMN_WIDTH)
             << "Connected Graph 900: [F]" << endl;
        cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;

        cout << endl
             << "Please select the problem for which you'd like to execute the nearest insertion algorithm: ";
        cin >> commandIn;

        if (commandIn != 'q' && commandIn != 'b') commandIn = toupper(commandIn);
        std::string nodesFilePath, edgesFilePath;

        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                edgesFilePath = "../dataset/Toy-Graphs/stadiums.csv";
                break;
            }
            case '2': {
                edgesFilePath = "../dataset/Toy-Graphs/tourism.csv";
                break;
            }
            case '3': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_25.csv";
                break;
            }
            case '4': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_50.csv";
                break;
            }
            case '5': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_75.csv";
                break;
            }
            case '6': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_100.csv";
                break;
            }
            case '7': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_200.csv";
                break;
            }
            case '8': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_300.csv";
                break;
            }
            case '9': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_400.csv";
                break;
            }
            case 'A': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_500.csv";
                break;
            }
            case 'C': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_600.csv";
                break;
            }
            case 'D': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_700.csv";
                break;
            }
            case 'E': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_800.csv";
                break;
            }
            case 'F': {
                edgesFilePath = "../dataset/Extra_Fully_Connected_Graphs/edges_900.csv";
                break;
            }
            case 'b': {
                return '\0';
            }
            case 'q': {
                cout << "Thank you for using our Routing for Ocean Shipping and Urban Deliveries System!" << endl;
                break;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }

        if (!edgesFilePath.empty()) {
            graph.clearGraph();
            dataRepository.clearData();
            cout << endl << "Loading graph..." << endl;
            extractFileInfo(edgesFilePath, nodesFilePath);

            auto start = random<unsigned int>(0, graph.getNumVertex() - 1);
            if (edgesFilePath.contains("Real-world-Graphs"))
                start = dataRepository.getFurthestVertex().getId();

            cout << "Calculating..." << endl;

            std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

            auto result = graph.nearestInsertionHeuristic(start);

            std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = endTime - startTime;
            double milliseconds = duration.count();
            printTime(milliseconds);

            cout << "TOUR LENGTH: " << fixed << setprecision(2) << result.first << endl;

            if (graph.getNumVertex() <= 25) {
                graph.printTour(result.second);
            }
        }
    }
    return commandIn;
}

