// #include <iostream>
// #include <vector>
// #include <queue>
// #include <random>
// #include <chrono>
// #include <algorithm> // Include for std::find

#include<bits/stdc++.h>

using namespace std;

// Structure to represent a network node
struct Node {
    int id;
    vector<int> neighbors; // IDs of neighboring nodes
};

struct Packet {
    int source;
    int destination;
    int size;
    double arrivalTime;
    double departureTime;
    vector<int> path; // Store the path taken by the packet
};

void simulateDosAttack(vector<Node>& network, int attackerNode, int targetNode, int attackIntensity, double currentTime) {
    for (int i = 0; i < attackIntensity; ++i) {
        Packet packet;
        packet.source = attackerNode;
        packet.destination = targetNode;
        packet.size = 1024; // Example packet size (bytes)
        packet.arrivalTime = currentTime;
        handlePacket(network, packet, currentTime);
    }
}

void simulateDdosAttack(vector<Node>& network, const vector<int>& attackerNodes, int targetNode, int attackIntensity, double currentTime) {
    for (int attacker : attackerNodes) {
        for (int i = 0; i < attackIntensity; ++i) {
            Packet packet;
            packet.source = attacker;
            packet.destination = targetNode;
            packet.size = 1024; // Example packet size (bytes)
            packet.arrivalTime = currentTime;
            handlePacket(network, packet, currentTime);
        }
    }
}


vector<Node> generateUnDirectedNetwork(int numNodes) {
    vector<Node> network(numNodes);
    random_device rd;
    mt19937 gen(rd());
    set<pair<int, int>> connections; // Use a set for efficient lookup

    for (int i = 0; i < numNodes; ++i) {
        network[i].id = i;
        int numNeighbors = 2; // Average number of neighbors

        poisson_distribution<> distrib_neighbors(numNeighbors);
        int actual_neighbors = distrib_neighbors(gen);
        actual_neighbors = min(actual_neighbors, numNodes - 1);

        // Limit the number of neighbors to avoid excessive connections
        actual_neighbors = min(actual_neighbors, 5);

        for (int j = 0; j < actual_neighbors; ++j) {
            int neighborId;
            int attempts = 0; // To avoid infinite loop
            do {
                neighborId = uniform_int_distribution<>(0, numNodes - 1)(gen);
                attempts++;
                if (attempts > 100) {
                    // cout << "Too many attempts to find a valid neighbor for Node " << i << endl;
                    break;
                }
            } while (neighborId == i || 
                     network[i].neighbors.size() >= numNodes - 1 || // Check if node has reached its max connections
                     network[i].neighbors.size() > 0 && find(network[i].neighbors.begin(), network[i].neighbors.end(), neighborId) != network[i].neighbors.end() ||
                     connections.count(make_pair(i, neighborId)) > 0 || // Use count for set
                     connections.count(make_pair(neighborId, i)) > 0); // Use count for set

            if (attempts > 100) {
                continue;
            }

            network[i].neighbors.push_back(neighborId);
            connections.insert(make_pair(i, neighborId)); // Use insert for set
            network[neighborId].neighbors.push_back(i);

            // Debug prints
            // cout << "Node " << i << " connected to Node " << neighborId << endl;
            // cout << "Node " << neighborId << " connected to Node " << i << endl;
        }
    }
    return network;
}

vector<Node> generateDirectedNetwork(int numNodes) {
    vector<Node> network(numNodes);
    random_device rd;
    mt19937 gen(rd());

    for (int i = 0; i < numNodes; ++i) {
        network[i].id = i;
        int numNeighbors = 2; // Average number of neighbors

        // Use a Poisson distribution for a more realistic number of neighbors
        poisson_distribution<> distrib_neighbors(numNeighbors);
        int actual_neighbors = distrib_neighbors(gen);
        actual_neighbors = min(actual_neighbors, numNodes - 1); // Limit to max possible neighbors

        for (int j = 0; j < actual_neighbors; ++j) {
            int neighborId;
            do {
                neighborId = uniform_int_distribution<>(0, numNodes - 1)(gen); // More concise random number generation
            } while (neighborId == i || find(network[i].neighbors.begin(), network[i].neighbors.end(), neighborId) != network[i].neighbors.end());
            network[i].neighbors.push_back(neighborId);
        }
    }
    return network;
}


// Function to simulate packet transmission (BFS example)
void simulateTransmission(const vector<Node>& network, int startNode, int endNode) {
    int numNodes = network.size();
    vector<bool> visited(numNodes, false);
    queue<int> q;

    q.push(startNode);
    visited[startNode] = true;

    cout << "Transmission path: ";

    while (!q.empty()) {
        int currentNode = q.front();
        q.pop();

        cout << currentNode << " ";

        if (currentNode == endNode) {
            cout << endl;
            return;
        }

        for (int neighbor : network[currentNode].neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    cout << "No path found." << endl;
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    int choice;
    cout << "Choose network type:\n";
    cout << "1. Directed\n";
    cout << "2. Undirected\n";
    cin >> choice;

    vector<Node> network; // Declare network outside the switch

    switch (choice) {
        case 1:
            network = generateDirectedNetwork(numNodes);
            break;
        case 2:
            network = generateUnDirectedNetwork(numNodes);
            break;
        default:
            cout << "Invalid Choice" << endl;
            return 1; // Indicate an error
    }

    // Example: Print the network topology
    cout << "Network Topology:" << endl;
    for (const auto& node : network) {
        cout << "Node " << node.id << ": ";
        for (int neighbor : node.neighbors) {
            cout << neighbor << " ";
        }
        cout << endl;
    }


    int startNode ;
    cout << "Enter the start node: ";
    cin >> startNode;
    int endNode ;
    cout << "Enter the end node: ";
    cin >> endNode;
    simulateTransmission(network, startNode, endNode);


    // startNode = 0;
    // endNode = 12; // Test no path
    // simulateTransmission(network, startNode, endNode);


   double currentTime = 0;
    double simulationDuration = 10; // Seconds

    while (currentTime < simulationDuration) {
        // ... (Simulate normal traffic)

        // Simulate DoS or DDoS attack (example)
        int attackerNode = 0;
        int targetNode = 5;
        int attackIntensity = 5;

        vector<int> attackers = {0, 1};

        //simulateDosAttack(network, attackerNode, targetNode, attackIntensity, currentTime);
        simulateDdosAttack(network, attackers, targetNode, attackIntensity, currentTime);

        // ... (Collect metrics)

        currentTime += 0.1; // Increment time
    }

    // ... (Output and analysis)


    return 0;
}
