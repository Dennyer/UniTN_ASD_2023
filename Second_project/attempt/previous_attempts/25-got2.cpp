#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

#include "got2.h"

using namespace std;

ofstream out("output.txt");
ifstream in("input.txt");

class Graph {
    private:
        // Adjacency Matrix
        vector<vector<bool>> adjMatrix; 
        // Number of nodes
        int nodes;
    public:
        // Constructor
        Graph(int numNodes) : nodes(numNodes) {
            adjMatrix.resize(numNodes, vector<bool>(numNodes, false));
        }

        // Destructor
        ~Graph() {
            for(int i = 0; i < nodes; i++) {
                adjMatrix[i].clear();
            }
            adjMatrix.clear();
        }

        // Method to add an edge
        void addEdge(int i, int j) {
            if(i >= 0 && i < nodes && j >= 0 && j < nodes) {
                adjMatrix[i][j] = true;
                adjMatrix[j][i] = true; 
            }
        }

        // Method to remove an edge
        void removeEdge(int i, int j) {
            if(i >= 0 && i < nodes && j >= 0 && j < nodes) {
                adjMatrix[i][j] = false;
                adjMatrix[j][i] = false; 
            }
        }

        // Method to check if there is a conneciton
        bool isConnection(int i, int j){
            return adjMatrix[i][j];
        }

        // Method to print the graph
        void printGraph() {
            for(int i = 0; i < nodes; i++) {
                for(int j = 0; j < nodes; j++) {
                    cout << adjMatrix[i][j] << " ";
                }
                cout << "\n";
            }
        }

        // Method to get the number of nodes
        int getNodes() {
            return nodes;
        }

        // Method that returns the number of edges
        int getNumberOfEdges(int node) {
            int edges = 0;
            for (int i = 0; i < nodes; i++) {
                if (adjMatrix[node][i]) {
                    edges++;
                }
            }
            return edges;
        }

        // Method that returns the connected components and if they are fully connected
        vector<pair<int, bool>> getConnectedComponents() {
            vector<bool> visited(nodes, false);
            vector<int> component(nodes);
            vector<pair<int, bool>> result(nodes);
            int compIndex = 0;

            for (int i = 0; i < nodes; i++) {
                if (!visited[i]) {
                    DFS(i, visited, compIndex, component);
                    compIndex++;
                }
            }

            vector<bool> stronglyConnected(compIndex, true);

            for (int i = 0; i < nodes; i++) {
                if(!stronglyConnected[component[i]]){
                    continue;
                }
                for (int j = 0; j < nodes; j++) {
                    if (i != j && component[i] == component[j] && !adjMatrix[i][j]) {
                        stronglyConnected[component[i]] = false;
                        break;
                    }
                }
            }

            for (int i = 0; i < nodes; i++) {
                result[i] = make_pair(component[i], stronglyConnected[component[i]]);
            }

            return result;
        }

        // Method to check if a node is part of a fully connected component
        bool isStronglyConnected(int node) {
            if (node >= 0 && node < nodes) {
                vector<bool> visited(nodes, false);
                vector<int> component(nodes, -1);
                int compIndex = 0;
                DFS(node, visited, compIndex, component);
                compIndex++;

                for (int i = 0; i < nodes; i++) {
                    if(component[i] == -1){
                        continue;
                    }
                    for(int j = 0; j < nodes; j++){
                        if (i != j && component[i] == component[j] && !adjMatrix[i][j]) {
                            return false;
                        }
                    }
                
                }
                return true;
            }
            return false;
        }
        
    private:
        // Depth First Search
        void DFS(int node, vector<bool>& visited, int compIndex, vector<int>& component) {
            visited[node] = true;
            component[node] = compIndex;
            for (int i = 0; i < nodes; i++) {
                if (adjMatrix[node][i] && !visited[i]) {
                    DFS(i, visited, compIndex, component);
                }
            }
        }
};

class Output{
    private:
        int counterAddEdge = 0, counterRemoveEdge = 0;
        // The boolean value is 1 for add and 0 for remove
        vector<pair<bool, pair<int, int>>> output;

    public:
        // Constructor
        Output() {
            counterAddEdge = 0;
            counterRemoveEdge = 0;
        }
        // Destructor
        ~Output() {
        }
        void addEdge(int i, int j){
            output.push_back(make_pair(1,make_pair(i,j)));
            counterAddEdge++;
        }
        void removeEdge(int i, int j){
            output.push_back(make_pair(0,make_pair(i,j)));
            counterRemoveEdge++;
        }

        void stampa(bool reset = false){
            out << counterAddEdge << " " << counterRemoveEdge << endl;
            for(int i = 0; i < output.size(); i++){
                if(output[i].first){
                    out << "+ " << output[i].second.first << " " << output[i].second.second << endl;
                } else {
                    out << "- " << output[i].second.first << " " << output[i].second.second << endl;
                }
            }
            out << "***" << endl;

            if(reset){
                counterAddEdge = 0;
                counterRemoveEdge = 0;
                output.clear();
            }
        }

};


int main() {

    int N, M;
    in >> N >> M;

    Graph g(N);
    Output output;

    int x, y;

    while(in >> x >> y){
        g.addEdge(x,y);
    }

    vector<pair<int, bool>> t = g.getConnectedComponents();
    vector<int> nodesConnections(N, -1);
    vector<int> nodesPerComponent(N/3, 0);
    for(int i = 0; i < t.size(); i++){
        nodesPerComponent[t[i].first]++;
    
    // Forse questi due cicli si possono mettere in uno solo ↑
    // Tanto i vettori si espandono da soli

        if(!t[i].second){
            nodesConnections[i] = g.getNumberOfEdges(i);
        }
    }

    
    for(int i = 0; i < t.size(); i++){

        if(nodesConnections[i] <= 0){
            continue;
        }

        if(nodesConnections[i] < nodesPerComponent[t[i].first]/2){
            for(int j = 0; j < N; j++){
                if(i != j && t[j].first == t[i].first && g.isConnection(i, j)){
                    g.removeEdge(i,j);
                    output.removeEdge(i,j);
                }
            }
        } else {
            for(int j = 0; j < N; j++){
                if(i != j && t[j].first == t[i].first && !g.isConnection(i, j)){
                    g.addEdge(i,j);
                    output.addEdge(i,j);
                }
            }
        }


    }


    output.stampa();
    
    return 0;
}




/*

Use of getConnectedComponents
vector<pair<int, bool>> t = g.getConnectedComponents();

for(int i = 0; i < t.size(); i++){
    cout << t[i].first << " " << t[i].second << endl;
}

Use of isStronglyConnected
for(int i = 0; i < g.getNodes(); i++){
    cout << g.isStronglyConnected(i) << endl;
}

*/