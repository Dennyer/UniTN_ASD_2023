#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>


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



struct Edge {
    int to;
    double weight;
};

class Community {
public:
    int size;
    double tot;
    map<int, double> in;

    void remove(int i, double weight, double ki) {
        size--;
        tot -= ki;
        in[i] -= weight;
        if (in[i] == 0) {
            in.erase(i);
        }
        if (size == 0) {
            in.clear();
        }
    }


    void add(int i, double weight, double ki) {
        size++;
        tot += ki;
        in[i] += weight;
    }
};


class Louvain {
public:
    int n;
    double m;
    vector<vector<Edge>> g;
    vector<Community> comm;
    vector<int> commId;
    vector<double> ki;
    vector<double> kiTot;

public:
    Louvain(int n) : n(n), g(n), comm(n), commId(n), ki(n), kiTot(n) {
    }

    void addEdge(int u, int v, double w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
        m += w;
        ki[u] += w;
        ki[v] += w;
    }

    void initCommunities() {
        for (int i = 0; i < n; i++) {
            comm[i].size = 1;
            comm[i].tot = ki[i];
            comm[i].in[i] = ki[i];
            commId[i] = i;
            kiTot[i] = ki[i];
        }
    }

    double modularity() {
        double q = 0;
        for (const Community &c : comm) {
            for (auto& entry : c.in) {
                const double& kin = entry.second;
                q += kin - ((c.tot * c.tot) / (2 * m));
            }
        }
        return q / (2 * m);
    }

    bool optimize() {
        bool updated = false;
        for (int i = 0; i < n; i++) {
            // Pre-compute community statistics
            map<int, double> commStats;
            int oldComm = commId[i];
            commStats[oldComm] = 0;
            for (const Edge& e : g[i]) {
                commStats[commId[e.to]] += e.weight;
            }

            // Current community data
            double kiInOldComm = commStats[oldComm];
            double oldCommTot = comm[oldComm].tot;

            // Remove node i from its old community
            comm[oldComm].remove(i, kiInOldComm, ki[i]);

            // Compute ΔQ when inserting node i into its old community
            double bestDeltaQ = kiInOldComm - oldCommTot * ki[i] / m;
            int bestComm = oldComm;

            // Compute ΔQ for all neighbor communities
            for (const auto& pair : commStats) {
                int newComm = pair.first;
                double deltaQ = pair.second - comm[newComm].tot * ki[i] / m;
                const double EPSILON = 1e-9;
                if (deltaQ > bestDeltaQ + EPSILON || (abs(deltaQ - bestDeltaQ) < EPSILON && newComm < bestComm)) {
                    bestDeltaQ = deltaQ;
                    bestComm = newComm;
                }
            }

            // Insert node i into the best community found
            comm[bestComm].add(i, commStats[bestComm], ki[i]);
            commId[i] = bestComm;

            // Update flag if a node changed its community
            updated |= (bestComm != oldComm);
        }
        return updated;
    }



    void aggregate() {
        map<int, int> newCommId;
        int newN = 0;
        for (int i = 0; i < n; i++) {
            if (comm[i].size > 0) {
                newCommId[i] = newN++;
                comm[i].tot = comm[i].size = 0;
                comm[i].in.clear();
            } else {
                newCommId.erase(i);  // ensure that this community ID is not mapped to a new ID
            }
        }

        vector<vector<Edge>> newG(newN);
        vector<double> newKi(newN);
        for (int i = 0; i < n; i++) {
        int u = newCommId[commId[i]];
        for (const Edge &e : g[i]) {
            int v = newCommId[commId[e.to]];
            if (u != v) {
                auto it = find_if(newG[u].begin(), newG[u].end(), [&](const Edge& edge) { return edge.to == v; });
                if (it != newG[u].end()) {
                    it->weight += e.weight;
                } else {
                    newG[u].push_back({v, e.weight});
                    newG[v].push_back({u, e.weight});
                }
            }
            newKi[u] += e.weight;
        }
    }

        n = newN;
        g = newG;
        ki = newKi;
        comm.resize(n);
        commId.resize(n);
        kiTot.resize(n);
        initCommunities();
    }


    double run() {
        initCommunities();
        while (optimize()) {
            aggregate();
        }
        return modularity();
    }
};


int main() {

    int N, M;
    in >> N >> M;

    Graph g(N);
    Output output;
    Louvain louvain(N);

    int i, j;

    while(in >> i >> j){
        g.addEdge(i,j);
        louvain.addEdge(i, j, 1);
    }

    louvain.run();
    vector<pair<int, bool>> t = g.getConnectedComponents();

    for(int i = 0; i < N; i++){
        if(t[i].second){
           continue; 
        }
        for(int j = 0; j < N; j++){
            if(i != j && !t[j].second && louvain.commId[i] == louvain.commId[j] && !g.isConnection(i, j)){
                g.addEdge(i,j);
                output.addEdge(i,j);
            }else if(i != j && louvain.commId[i] != louvain.commId[j] && g.isConnection(i, j)){
                g.removeEdge(i,j);
                output.removeEdge(i,j);
            }
        }
        t = g.getConnectedComponents();
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