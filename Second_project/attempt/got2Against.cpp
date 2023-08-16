#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

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

        int stampa(bool reset = false, int n = -1){
            if(n > 0 && n <= output.size()) return -1;
            out << counterAddEdge << " " << counterRemoveEdge << "\n";
            int num = 0;
            for(int i = 0; i < output.size(); i++){
                if(output[i].first){
                    out << "+ " << output[i].second.first << " " << output[i].second.second << "\n";
                } else {
                    out << "- " << output[i].second.first << " " << output[i].second.second << "\n";
                }
                num ++;
            }
            out << "***" << "\n";

            if(reset){
                counterAddEdge = 0;
                counterRemoveEdge = 0;
                output.clear();
            }

            return num;
        }

};

Output output;

struct Community{
    int size;
    int id;
    vector<int> nodes;
};

class Graph {
    public:
        // Adjacency List
        vector<set<int>> adjList; 
        // Number of nodes
        int nodes;
        vector<pair<int, int>> merges;

        vector<int> ki;
        
        vector<Community> community;


        // Constructor
        Graph(int numNodes) : nodes(numNodes) {
            adjList.resize(numNodes);
            community.resize(numNodes);
            ki.resize(numNodes, 0);
        }

        // Destructor
        ~Graph() {
            adjList.clear();
        }

        // Method to add an edge
        bool addEdge(int i, int j) {
            if(i >= 0 && i < nodes && j >= 0 && j < nodes && adjList[i].find(j) == adjList[i].end()) {
                adjList[i].insert(j);
                adjList[j].insert(i);

                ki[i]++;
                ki[j]++;

                return true;
            }
            return false;
        }

        // Method to remove an edge
        bool removeEdge(int i, int j) {
            if(i >= 0 && i < nodes && j >= 0 && j < nodes && adjList[i].find(j) != adjList[i].end()) {
                adjList[i].erase(j);
                adjList[j].erase(i);

                ki[i]--;
                ki[j]--;

                return true;
            }
            return false;
        }

        void initCommunity(){
            for(int i = 0; i < nodes; i++){
                community[i].size = 1;
                community[i].id = i;
                community[i].nodes.push_back(i);
            }
        }

        void minimize(){

            unordered_map<int, int> communitiesConnections;
            vector<int> prunedNodes;

            long sum = 0;
            int count = 0;

            for(int i = 0; i < nodes; i++){

                int bestCommunity = -1;
                int minimum = nodes;

                for(const int& j : adjList[i]){
                    if(i != j){
                        if(communitiesConnections.find(community[j].id) == communitiesConnections.end()){
                            communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                        }
                        int connections = communitiesConnections[community[j].id];
                        
                        if(connections < minimum){
                            minimum = connections;
                            bestCommunity = community[j].id;
                        }
                    }
                }

                communitiesConnections.clear();

                if(bestCommunity != -1 && minimum != nodes){
                    sum += minimum;
                    count++;
                    if(minimum*5 <= sum/count){
                        if(bestCommunity != community[i].id){
                            community[bestCommunity].size++;
                            community[bestCommunity].nodes.push_back(i);
                            community[i].id = bestCommunity;
                        }
                    }else{
                        prunedNodes.push_back(i);
                    }
                }
            }

            for(int i = 0; i < prunedNodes.size(); i++){
                int currentNode = prunedNodes[i];

                int bestCommunity = -1;
                int minimum = nodes;

                for(int j = 0; j < nodes; j++){
                    if(currentNode != j && adjList[currentNode].find(j) != adjList[currentNode].end()){
                        if(communitiesConnections.find(community[j].id) == communitiesConnections.end()){
                            communitiesConnections[community[j].id] = connectToCommunity(currentNode, community[j].id);
                        }
                        int connections = communitiesConnections[community[j].id];
                        
                        if(connections < minimum){
                            minimum = connections;
                            bestCommunity = community[j].id;
                        }
                    }
                }

                communitiesConnections.clear();

                if(bestCommunity != -1 && minimum != nodes){
                    community[bestCommunity].size++;
                    community[bestCommunity].nodes.push_back(currentNode);
                    community[currentNode].id = bestCommunity;
                }
            }

            for(int i = 0; i < nodes; i++){
                changeCommunityWithoutEdit(i, community[i].id);
            }

            int num1 = output.stampa(true);
            int tempNum1 = num1;

            vector<Community> community2 = community;

            prunedNodes.clear();
            communitiesConnections.clear();

            sum = 0;
            count = 0;
            for(int i = 0; i < nodes; i++){

                int bestCommunity = -1;
                int minimum = nodes;

                for(const int& j : adjList[i]){
                    if(i != j){
                        if(communitiesConnections.find(community[j].id) == communitiesConnections.end()){
                            communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                        }
                        int connections = communitiesConnections[community[j].id];
                        
                        if(connections < minimum){
                            minimum = connections;
                            bestCommunity = community[j].id;
                        }
                    }
                }

                communitiesConnections.clear();

                if(bestCommunity != -1 && minimum != nodes){
                    sum += minimum;
                    count++;
                    if(minimum*2 <= sum/count){
                        if(bestCommunity != community[i].id){
                            community[bestCommunity].size++;
                            community[bestCommunity].nodes.push_back(i);
                            community[i].id = bestCommunity;
                        }
                    }else{
                        prunedNodes.push_back(i);
                    }
                }
            }

            for(int i = 0; i < prunedNodes.size(); i++){
                int currentNode = prunedNodes[i];

                int bestCommunity = -1;
                int minimum = nodes;

                for(int j = 0; j < nodes; j++){
                    if(currentNode != j && adjList[currentNode].find(j) != adjList[currentNode].end()){
                        if(communitiesConnections.find(community[j].id) == communitiesConnections.end()){
                            communitiesConnections[community[j].id] = connectToCommunity(currentNode, community[j].id);
                        }
                        int connections = communitiesConnections[community[j].id];
                        
                        if(connections < minimum){
                            minimum = connections;
                            bestCommunity = community[j].id;
                        }
                    }
                }

                communitiesConnections.clear();

                if(bestCommunity != -1 && minimum != nodes){
                    community[bestCommunity].size++;
                    community[bestCommunity].nodes.push_back(currentNode);
                    community[currentNode].id = bestCommunity;
                }
            }

            for(int i = 0; i < nodes; i++){
                changeCommunityWithoutEdit(i, community[i].id);
            }

            num1 = output.stampa(true, num1);
            if(num1 >= tempNum1 || num1 < 0){
                community = community2;  
                num1 = tempNum1;              
            }else{
                tempNum1 = num1;
            }

            while(true){
                communitiesConnections.clear();

                for(int i = 0; i < nodes; i++){

                    int bestCommunity = -1;
                    int minimum = nodes;

                    for(const int& j : adjList[i]){
                        if(i != j){
                            if(communitiesConnections.find(community[j].id) == communitiesConnections.end()){
                                communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                            }
                            int connections = communitiesConnections[community[j].id];
                            
                            if(connections < minimum){
                                minimum = connections;
                                bestCommunity = community[j].id;
                            }
                        }
                    }

                    communitiesConnections.clear();

                    if(bestCommunity != -1 && minimum != nodes && bestCommunity != community[i].id){
                        community[bestCommunity].size++;
                        community[bestCommunity].nodes.push_back(i);
                        community[i].id = bestCommunity;
                    }
                }
                
                for(int i = 0; i < nodes; i++){
                    changeCommunityWithoutEdit(i, community[i].id);
                }

                num1 = output.stampa(true, num1);
                if(tempNum1 == num1 || num1 < 0){
                    break;
                }
                tempNum1 = num1;
            }

            cout << num1 << " -- " << tempNum1 << endl;

        }    

        void changeCommunity(int node, int communityId){
            bool t = false;
            for(int i = node+1; i < nodes; i++){
                if(adjList[node].find(i) != adjList[node].end() && community[i].id != communityId){
                    t = removeEdge(node, i);
                    if (t) output.removeEdge(node, i);
                }else if(adjList[node].find(i) == adjList[node].end() && community[i].id == communityId){
                    t = addEdge(node, i);
                    if (t) output.addEdge(node, i);
                }
            }
        }

        void changeCommunityWithoutEdit(int node, int communityId){
            for(int i = node+1; i < nodes; i++){
                if(adjList[node].find(i) != adjList[node].end() && community[i].id != communityId){
                    output.removeEdge(node, i);
                }else if(adjList[node].find(i) == adjList[node].end() && community[i].id == communityId){
                    output.addEdge(node, i);
                }
            }
        }

       int connectToCommunity(int node, int communityId){

            int count = 0;
            for(int i = 0; i < community[communityId].nodes.size(); i++){
                if(community[communityId].nodes[i] != node && adjList[node].find(community[communityId].nodes[i]) != adjList[node].end()){
                    count++;
                }
            }
            
            int temp = community[communityId].nodes.size();
            if(community[node].id == communityId){
                temp--;
            }
            
            return (temp - count) + (ki[node] - count);
        }
        
};

int main() {

    clock_t start = clock();

    int N, M;
    in >> N >> M;

    Graph g(N);

    int i, j;

    while(in >> i >> j){
        g.addEdge(i,j);
    }
    g.initCommunity();
    g.minimize();

    //output.stampa2();

    cout << "Tempo di esecuzione got2A: " << (clock() - start) / (double)CLOCKS_PER_SEC << endl;
    
    return 0;
}