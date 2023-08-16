#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <set>
#include <climits>
#include <csignal>

#include "got2.h"

using namespace std;

ofstream out("output.txt");
ifstream in("input.txt");

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

Output output;

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
                adjMatrix[j][i] = true; // This line is for undirected graph. Remove this line for directed graph.
            }
        }

        // Method to remove an edge
        void removeEdge(int i, int j) {
            if(i >= 0 && i < nodes && j >= 0 && j < nodes) {
                adjMatrix[i][j] = false;
                adjMatrix[j][i] = false; // This line is for undirected graph. Remove this line for directed graph.
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

        int getNumberOfEdges(int node) {
            int edges = 0;
            for (int i = 0; i < nodes; i++) {
                if (adjMatrix[node][i]) {
                    edges++;
                }
            }
            return edges;
        }

        vector<pair<int, bool>> getConnectedComponents() {
            vector<bool> visited(nodes, false);
            vector<int> component(nodes);
            vector<pair<int, bool>> result(nodes);
            int compIndex = 0;

            // DFS to mark components.
            for (int i = 0; i < nodes; i++) {
                if (!visited[i]) {
                    DFS(i, visited, compIndex, component);
                    compIndex++;
                }
            }

            vector<bool> stronglyConnected(compIndex, true);

            // Check if components are strongly connected.
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

        // Method to check if a node is part of a strongly connected component
        bool isStronglyConnected(int node) {
            if (node >= 0 && node < nodes) {
                vector<bool> visited(nodes, false);
                vector<int> component(nodes, -1);
                // Perform DFS from the given node
                int compIndex = 0;
                DFS(node, visited, compIndex, component);
                compIndex++;

                // Check if we are able to reach all other nodes in the same component
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
            return false; // Return false if the node index is out of range
        }

        void greedy() {
            vector<int> ordinato(nodes);
            vector<bool> preso(nodes, 0);

            for(int i=0; i<nodes; i++){
                ordinato[i]=i;
            }  

            for(int i=0; i<nodes; i++)
            {
                for(int j=i+1; j<nodes; j++) { 
                    if(getNumberOfEdges(ordinato[i])<getNumberOfEdges(ordinato[j]))
                    {
                        int temp = ordinato[i];
                        ordinato[i] = ordinato[j];
                        ordinato[j] = temp;
                    }
                }
            }

            /* for(int i=0; i<nodes; i++){
                cout << getNumberOfEdges(ordinato[i]) << ", ";
            }
            cout << endl;
            for(int i=0; i<nodes; i++){
                cout << ordinato[i] << ", ";
            } */

            for(int i=0; i<nodes; i++){
                int currentNode = ordinato[i];
                if (!preso[currentNode]) {
                    vector<int> partitionNodes;
                    set<int> setNodes;
                    vector<int> partitionDegree;
                    partitionNodes.push_back(currentNode);
                    setNodes.insert(currentNode);
                    for(int i=0; i<nodes; i++){
                        if(i == currentNode) continue;
                        if(preso[i]) continue;

                        int edge = adjMatrix[currentNode][i];

                        if(edge==1){
                            partitionNodes.push_back(i);
                            setNodes.insert(i);
                        }
                    }

                    int totalPoints = 0;
                    int min = INT_MAX;
                    int ele = -1;
                    int posCounter = 0;
                    int pos = -1;
                    
                    for(const int elem: partitionNodes){
                        int degreeElem = 0;
                        for (int i = 0; i < nodes; i++) {
                            if(i == elem)   continue;
                            int edge = adjMatrix[elem][i];
                            auto contain = setNodes.find(i);
                            if (edge == 1 && contain != setNodes.end()) {
                                ++degreeElem;
                            }
                            else if ((edge == 1 && contain == setNodes.end()) || (edge == 0 && contain !=setNodes.end())) {
                                --degreeElem;
                                    --degreeElem;
                            }
                        }

                        if(degreeElem < min){
                            min = degreeElem;
                            ele = elem;
                            pos = posCounter;
                        }
                        
                        totalPoints += degreeElem;
                        partitionDegree.push_back(degreeElem);

                        posCounter ++;
                    }

                    bool improved = true;
                    int oldEle;
                    bool addBack = true;

                    if(partitionDegree.size() > 1){
                        while(improved){
                            if(partitionDegree.size() > 1){
                                oldEle = ele;
                                partitionDegree.clear();
                                partitionNodes.erase(partitionNodes.begin() + pos);
                                setNodes.erase(ele);

                                int points = 0;

                                min = INT_MAX;
                                pos = -1;
                                posCounter = 0;
                                
                                for(const int elem: partitionNodes){
                                    int degreeElem = 0;
                                    for (int i = 0; i < nodes; i++) {
                                        if(i == elem)   continue;
                                        int edge = adjMatrix[elem][i];
                                        auto contain = setNodes.find(i);
                                        if (edge == 1 && contain != setNodes.end()) {
                                            degreeElem += 20;
                                        }
                                        else if ((edge == 1 && contain == setNodes.end()) || (edge == 0 && contain !=setNodes.end())) {
                                            degreeElem -= 21;
                                        }
                                    }
                                    if(degreeElem < min){
                                        min = degreeElem;
                                        ele = elem;
                                        pos = posCounter;
                                    }
                                    
                                    points += degreeElem;
                                    partitionDegree.push_back(degreeElem);

                                    posCounter ++;
                                }

                                improved = points > totalPoints;
                                if(improved){
                                    totalPoints = points;
                                }
                            }else{
                                improved = false;
                                addBack = false;
                            }
                        }
                        if(addBack){
                            partitionNodes.push_back(oldEle);
                        }
                    }

                    for(const int elem: partitionNodes){
                        for (int i = elem; i < nodes; i++) {
                            if(i == elem)   continue;
                            bool edge = adjMatrix[elem][i];
                            
                            bool presente = false;
                            
                            for (int j = 0; j < partitionNodes.size(); j++) {
                                    if(partitionNodes[j] == i) {presente=true;}
                            }

                            if (presente && !edge) {
                                output.addEdge(elem, i);
                            }
                            else if(!presente && edge){
                                output.removeEdge(elem, i);
                            }
                        }
                    }

                    bool reset = true;
                    
                    for (int i = 0; i < partitionNodes.size(); i++) {
                            preso[partitionNodes[i]] = true;
                            if(currentNode == partitionNodes[i]) reset = false;
                    }

                    if(reset){
                        --i;
                    }


                    
                }
                
            }
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

extern "C" void handle_sigint(int sig_num) { 
    output.stampa();
}

int main() {

    int N, M;
    in >> N >> M;

    Graph g(N);

    signal(SIGINT, handle_sigint);

    int x, y;

    while(in >> x >> y){
        g.addEdge(x,y);
    }

    // vector<pair<int, bool>> t = g.getConnectedComponents();

    // for(int i = 0; i < t.size(); i++){
    //     if(!t[i].second){
    //         for(int j = 0; j < N; j++){
    //             if(i != j && t[j].first == t[i].first && !g.isConnection(i,j)){
    //                 g.addEdge(i,j);
    //                 output.addEdge(i,j);
    //             }
    //         }
    //     }
    // }

    // output.stampa();
    
    g.greedy();
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