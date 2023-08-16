// Include necessary header files
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

#include "got2.h"

using namespace std;

// Global file streams for input and output
ofstream out("output.txt");
ifstream in("input.txt");

// Class to handle and manage output-related tasks
class Output
{
private:
    int counterAddEdge = 0, counterRemoveEdge = 0; // Counters for operations
    // The boolean value is 1 for add and 0 for remove
    vector<pair<bool, pair<int, int>>> output; // Stores operations

public:
    // Constructor
    Output()
    {
        counterAddEdge = 0;
        counterRemoveEdge = 0;
    }
    // Destructor
    ~Output() {}

    // Add an edge to the output
    void addEdge(int i, int j)
    {
        output.push_back(make_pair(1, make_pair(i, j)));
        counterAddEdge++;
    }

    // Remove an edge from the output
    void removeEdge(int i, int j)
    {
        output.push_back(make_pair(0, make_pair(i, j)));
        counterRemoveEdge++;
    }

    // Print the output to the file with the appropriate format and optionally reset the data
    int stampa(bool reset = false, int n = -1)
    {
        if (n > 0 && n <= output.size())
            return -1;
        out << counterAddEdge << " " << counterRemoveEdge << "\n";
        int num = 0;
        for (int i = 0; i < output.size(); i++)
        {
            if (output[i].first)
            {
                out << "+ " << output[i].second.first << " " << output[i].second.second << "\n";
            }
            else
            {
                out << "- " << output[i].second.first << " " << output[i].second.second << "\n";
            }
            num++;
        }
        out << "***"
            << "\n";

        if (reset)
        {
            counterAddEdge = 0;
            counterRemoveEdge = 0;
            output.clear();
        }

        return num;
    }
};

Output output;

// Struct to represent a community in the graph
struct Community
{
    int size;          // Size of the community
    int id;            // Identifier for the community
    vector<int> nodes; // Nodes within the community
};

// Class to represent the graph and perform operations on it
class Graph
{
public:
    vector<set<int>> adjList;    // Adjacency List representation of the graph
    int nodes;                   // Number of nodes in the graph
    vector<int> ki;              // Stores the degree of each node
    vector<Community> community; // List of communities in the graph

    // Constructor to initialize the graph
    Graph(int numNodes) : nodes(numNodes)
    {
        adjList.resize(numNodes);
        community.resize(numNodes);
        ki.resize(numNodes, 0);
    }

    // Destructor to clear graph resources
    ~Graph()
    {
        adjList.clear();
    }

    // Add an edge to the graph
    bool addEdge(int i, int j)
    {
        // Basic validation
        if (i >= 0 && i < nodes && j >= 0 && j < nodes && adjList[i].find(j) == adjList[i].end())
        {
            adjList[i].insert(j);
            adjList[j].insert(i);

            ki[i]++;
            ki[j]++;

            return true;
        }
        return false;
    }

    // Remove an edge from the graph
    bool removeEdge(int i, int j)
    {
        // Basic validation
        if (i >= 0 && i < nodes && j >= 0 && j < nodes && adjList[i].find(j) != adjList[i].end())
        {
            adjList[i].erase(j);
            adjList[j].erase(i);

            ki[i]--;
            ki[j]--;

            return true;
        }
        return false;
    }

    // Initialize communities such that each node is its own community
    void initCommunity()
    {
        for (int i = 0; i < nodes; i++)
        {
            community[i].size = 1;
            community[i].id = i;
            community[i].nodes.push_back(i);
        }
    }

    // Method to try and minimize the number of edges that cross between communities
    void minimize()
    {

        // Stores the connections of a node to various communities.
        unordered_map<int, int> communitiesConnections;

        // Nodes that do not fit well in the initial community placement and require revisiting.
        vector<int> prunedNodes;

        long sum = 0;  // Tracks total connections for average calculations.
        int count = 0; // Counter for total nodes processed.

        // Determine the best community for every node.
        for (int i = 0; i < nodes; i++)
        {
            int bestCommunity = -1; // Best community ID for node i.
            int minimum = nodes;    // Minimum connections a node has to any community.

            // Iterates over neighbors of node i.
            for (const int &j : adjList[i])
            {
                if (i != j)
                {
                    // If the connections of node i to the community of node j is not known, compute it.
                    if (communitiesConnections.find(community[j].id) == communitiesConnections.end())
                    {
                        communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                    }
                    int connections = communitiesConnections[community[j].id];

                    // If the connections are less than the current minimum, update the best community.
                    if (connections < minimum)
                    {
                        minimum = connections;
                        bestCommunity = community[j].id;
                    }
                }
            }

            communitiesConnections.clear();

            // If a better community is found for the node i.
            if (bestCommunity != -1 && minimum != nodes)
            {
                sum += minimum;
                count++;

                // If the node's connections to its best community are significantly smaller than average,
                // then move the node to this community.
                if (minimum * 5 <= sum / count)
                {
                    if (bestCommunity != community[i].id)
                    {
                        community[bestCommunity].size++;
                        community[bestCommunity].nodes.push_back(i);
                        community[i].id = bestCommunity;
                    }
                }
                // Otherwise, mark the node for reevaluation.
                else
                {
                    prunedNodes.push_back(i);
                }
            }
        }

        // Re-evaluate nodes that were pruned in the previous loop to see if they fit better elsewhere.
        for (int i = 0; i < prunedNodes.size(); i++)
        {
            int currentNode = prunedNodes[i];

            int bestCommunity = -1;
            int minimum = nodes;

            for (int j = 0; j < nodes; j++)
            {
                if (currentNode != j && adjList[currentNode].find(j) != adjList[currentNode].end())
                {
                    if (communitiesConnections.find(community[j].id) == communitiesConnections.end())
                    {
                        communitiesConnections[community[j].id] = connectToCommunity(currentNode, community[j].id);
                    }
                    int connections = communitiesConnections[community[j].id];

                    if (connections < minimum)
                    {
                        minimum = connections;
                        bestCommunity = community[j].id;
                    }
                }
            }

            communitiesConnections.clear();

            if (bestCommunity != -1 && minimum != nodes)
            {
                community[bestCommunity].size++;
                community[bestCommunity].nodes.push_back(currentNode);
                community[currentNode].id = bestCommunity;
            }
        }

        // Update graph based on the new community assignments without actually editing the graph structure.
        for (int i = 0; i < nodes; i++)
        {
            changeCommunityWithoutEdit(i, community[i].id);
        }

        int num1 = output.stampa(true);
        int tempNum1 = num1;

        vector<Community> community2 = community; // Backup of current community placements.

        // Clear previous data and repeat the process with a different criteria.
        prunedNodes.clear();
        communitiesConnections.clear();

        sum = 0;
        count = 0;

        // A process similar to the first loop but with a different criteria (minimum * 2).
        for (int i = 0; i < nodes; i++)
        {

            int bestCommunity = -1;
            int minimum = nodes;

            for (const int &j : adjList[i])
            {
                if (i != j)
                {
                    if (communitiesConnections.find(community[j].id) == communitiesConnections.end())
                    {
                        communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                    }
                    int connections = communitiesConnections[community[j].id];

                    if (connections < minimum)
                    {
                        minimum = connections;
                        bestCommunity = community[j].id;
                    }
                }
            }

            communitiesConnections.clear();

            if (bestCommunity != -1 && minimum != nodes)
            {
                sum += minimum;
                count++;
                if (minimum * 2 <= sum / count)
                {
                    if (bestCommunity != community[i].id)
                    {
                        community[bestCommunity].size++;
                        community[bestCommunity].nodes.push_back(i);
                        community[i].id = bestCommunity;
                    }
                }
                else
                {
                    prunedNodes.push_back(i);
                }
            }
        }

        // Re-evaluate nodes with the new criteria.
        for (int i = 0; i < prunedNodes.size(); i++)
        {
            int currentNode = prunedNodes[i];

            int bestCommunity = -1;
            int minimum = nodes;

            for (int j = 0; j < nodes; j++)
            {
                if (currentNode != j && adjList[currentNode].find(j) != adjList[currentNode].end())
                {
                    if (communitiesConnections.find(community[j].id) == communitiesConnections.end())
                    {
                        communitiesConnections[community[j].id] = connectToCommunity(currentNode, community[j].id);
                    }
                    int connections = communitiesConnections[community[j].id];

                    if (connections < minimum)
                    {
                        minimum = connections;
                        bestCommunity = community[j].id;
                    }
                }
            }

            communitiesConnections.clear();

            if (bestCommunity != -1 && minimum != nodes)
            {
                community[bestCommunity].size++;
                community[bestCommunity].nodes.push_back(currentNode);
                community[currentNode].id = bestCommunity;
            }
        }

        // Update graph based on the new community assignments again without editing the graph structure.
        for (int i = 0; i < nodes; i++)
        {
            changeCommunityWithoutEdit(i, community[i].id);
        }

        // Compare the two community assignment strategies and keep the one with fewer changes.
        num1 = output.stampa(true, num1);
        if (num1 >= tempNum1 || num1 < 0)
        {
            community = community2; // Restore from backup if the new strategy wasn't better.
            num1 = tempNum1;
        }
        else
        {
            tempNum1 = num1;
        }

        // Keep refining the communities until no improvements are found.
        while (true)
        {
            communitiesConnections.clear();

            for (int i = 0; i < nodes; i++)
            {

                int bestCommunity = -1;
                int minimum = nodes;

                for (const int &j : adjList[i])
                {
                    if (i != j)
                    {
                        if (communitiesConnections.find(community[j].id) == communitiesConnections.end())
                        {
                            communitiesConnections[community[j].id] = connectToCommunity(i, community[j].id);
                        }
                        int connections = communitiesConnections[community[j].id];

                        if (connections < minimum)
                        {
                            minimum = connections;
                            bestCommunity = community[j].id;
                        }
                    }
                }

                communitiesConnections.clear();

                if (bestCommunity != -1 && minimum != nodes && bestCommunity != community[i].id)
                {
                    community[bestCommunity].size++;
                    community[bestCommunity].nodes.push_back(i);
                    community[i].id = bestCommunity;
                }
            }

            // Update graph based on the new community assignments.
            for (int i = 0; i < nodes; i++)
            {
                changeCommunityWithoutEdit(i, community[i].id);
            }

            // If there are no improvements, exit the loop.
            num1 = output.stampa(true, num1);
            if (tempNum1 == num1 || num1 < 0)
            {
                break;
            }
            tempNum1 = num1;
        }

        cout << num1 << " -- " << tempNum1 << endl;
    }

    // Change the community of a node and update the graph edges accordingly
    void changeCommunity(int node, int communityId)
    {
        bool t = false;
        for (int i = node + 1; i < nodes; i++)
        {
            if (adjList[node].find(i) != adjList[node].end() && community[i].id != communityId)
            {
                t = removeEdge(node, i);
                if (t)
                    output.removeEdge(node, i);
            }
            else if (adjList[node].find(i) == adjList[node].end() && community[i].id == communityId)
            {
                t = addEdge(node, i);
                if (t)
                    output.addEdge(node, i);
            }
        }
    }

    // Change the community of a node but do not edit the actual graph
    void changeCommunityWithoutEdit(int node, int communityId)
    {
        for (int i = node + 1; i < nodes; i++)
        {
            if (adjList[node].find(i) != adjList[node].end() && community[i].id != communityId)
            {
                output.removeEdge(node, i);
            }
            else if (adjList[node].find(i) == adjList[node].end() && community[i].id == communityId)
            {
                output.addEdge(node, i);
            }
        }
    }

    // Count the number of edges a node has to a given community
    int connectToCommunity(int node, int communityId)
    {

        int count = 0;
        for (int i = 0; i < community[communityId].nodes.size(); i++)
        {
            if (community[communityId].nodes[i] != node && adjList[node].find(community[communityId].nodes[i]) != adjList[node].end())
            {
                count++;
            }
        }

        int temp = community[communityId].nodes.size();
        if (community[node].id == communityId)
        {
            temp--;
        }

        return (temp - count) + (ki[node] - count);
    }
};

int main()
{

    int N, M; // Number of nodes and edges
    in >> N >> M;

    Graph g(N); // Initialize graph

    int i, j;

    // Read edges from input file
    while (in >> i >> j)
    {
        g.addEdge(i, j);
    }
    g.initCommunity();
    g.minimize();

    return 0;
}