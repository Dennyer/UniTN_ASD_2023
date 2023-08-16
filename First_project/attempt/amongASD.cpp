// Program to find Dijkstra's shortest path using STL set
#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <fstream>
#include <set>
#include <queue>
#include <stack>

using namespace std;
#define INF 0x3f3f3f3f

ofstream out("output.txt");
ifstream in("input.txt");
//ifstream in("input/input11.txt");

// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int V; // No. of vertices

    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    //list<pair<int, pair<int, int>>> *adj;
    vector<vector<pair<int, pair<int, int>>>> adj;
    map<int,map<int,bool>> edge;


public:
    Graph(int V); // Constructor

    // function to add an edge to graph
    void addEdge(int u, int v, int w, int k);

    // function to add an edge to graph
    void reduceEdge(int u, int v);

    bool getEdge(int u, int v);

    // prints shortest path from s
    void shortestPathK0(int s, vector<pair<int, int>> &dist);

    // prints shortest path from s
    void shortestPathK1(bool min, int s, vector<pair<int, int>> &dist);

    void shortestPathK2Impostor(int src, vector<pair<int, int>> &dist, vector<pair<int, int>> const &impostor);

    void shortestPathK2Pareggio(int src, vector<pair<int, int>> &dist, vector<pair<int, int>> const &student);

    void shortestPathK2Final (int src, vector<pair<int, int>> &dist);
};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    //adj = new list<pair<int, pair<int, int>>>[V];
    adj.resize (V);
}

void Graph::addEdge(int u, int v, int w, int k)
{
    adj[u].push_back(make_pair(v, make_pair(w, k)));
}

void Graph::reduceEdge(int u, int v){
    edge[u][v] = true;
}

bool Graph::getEdge(int u, int v){
    return edge[u][v];
}

// Prints shortest paths from src to all other vertices with K = 0;
void Graph::shortestPathK0(int src, vector<pair<int, int>> &dist)
{
    // Create a set to store vertices that are being
    // processed

    set<pair<int, int>> setds;
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src].first = 0;
    dist[src].second = src;

    /* Looping till all shortest distance are finalized
    then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;

        // 'i' is used to get all adjacent vertices of a vertex
        for (pair<int, pair<int, int>> i : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = i.first;
            int weight = i.second.first;

            // If there is shorter path to v through u.
            if (dist[v].first > dist[u].first + weight)
            {
                /* If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them,
                    we would never reach here. */
                if (dist[v].first != INF)
                {
                    setds.erase(setds.find(make_pair(dist[v].first, v)));
                }

                // Updating distance of v
                dist[v].first = dist[u].first + weight;
                dist[v].second = u;
                setds.insert(make_pair(dist[v].first, v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    /*cout << ("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        cout << i << " "<< "dist[i] = " << dist[i] << endl;
*/
}
// Prints shortest paths from src to all other vertices
void Graph::shortestPathK1(bool min, int src, vector<pair<int, int>> &dist)
{
    // Create a set to store vertices that are being
    // processed

    set<pair<int, int>> setds;
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src].first = 0;
    dist[src].second = src; //vettore, prima posizione distanza rispetto src, seconda posizione contiene nodo da cui deriva

    /* Looping till all shortest distance are finalized
    then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;

        // 'i' is used to get all adjacent vertices of a vertex
        for (pair<int, pair<int, int>> i : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = i.first;
            int weight;
            if (i.second.second != 0)
            {
                if (min)
                {
                    weight = i.second.first;
                }
                else
                {
                    weight = i.second.second;
                }
            }
            else
            {
                weight = i.second.first;
            }

            // If there is shorter path to v through u.
            if (dist[v].first > dist[u].first + weight)
            {
                /* If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them,
                    we would never reach here. */
                if (dist[v].first != INF)
                {
                    setds.erase(setds.find(make_pair(dist[v].first, v)));
                }

                // Updating distance of v
                dist[v].first = dist[u].first + weight;
                dist[v].second = u;
                setds.insert(make_pair(dist[v].first, v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    /*cout << ("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        cout << i << " "<< "dist[i] = " << dist[i] << endl;
*/
}

void Graph::shortestPathK2Impostor(int src, vector<pair<int, int>> &dist, vector<pair<int, int>> const &student)
{
    // Create a set to store vertices that are being
    // processed

    set<pair<int, int>> setds;
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src].first = 0;
    dist[src].second = src; //vettore, prima posizione distanza rispetto src, seconda posizione contiene nodo da cui deriva

    /* Looping till all shortest distance are finalized
    then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());

        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;
        bool min=(dist[u].first < (student[u].first != 0 ? student[u].first : INF));

        // 'i' is used to get all adjacent vertices of a vertex
        for (pair<int, pair<int, int>> i : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = i.first;
            int weight;
            if (i.second.second != 0)
            {
                if (min)
                {
                    weight = i.second.first;
                }
                else
                {
                    weight = i.second.second;
                }
            }
            else
            {
                weight = i.second.first;
            }

            // If there is shorter path to v through u.
            if (dist[v].first > dist[u].first + weight)
            {
                /* If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them,
                    we would never reach here. */
                if (dist[v].first != INF)
                {
                    setds.erase(setds.find(make_pair(dist[v].first, v)));
                }

                // Updating distance of v
                dist[v].first = dist[u].first + weight;
                dist[v].second = u;
                setds.insert(make_pair(dist[v].first, v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    /*cout << ("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        cout << i << " "<< "dist[i] = " << dist[i] << endl;
*/
}
void Graph::shortestPathK2Pareggio(int src, vector<pair<int, int>> &dist, vector<pair<int, int>> const &student)
{
    // Create a set to store vertices that are being
    // processed

    set<pair<int, int>> setds;
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src].first = 0;
    dist[src].second = src; //vettore, prima posizione distanza rispetto src, seconda posizione contiene nodo da cui deriva

    /* Looping till all shortest distance are finalized
    then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;

        bool min=(dist[u].first <= student[u].first);

        // 'i' is used to get all adjacent vertices of a vertex
        for (pair<int, pair<int, int>> i : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = i.first;
            int weight;
            if (i.second.second != 0)
            {
                if (min)
                {
                    weight = i.second.first;
                }
                else
                {   
                    weight = edge[u][v] ? i.second.first : i.second.second;
                }
            }
            else
            {
                weight = i.second.first;
            }

            // If there is shorter path to v through u.
            if (dist[v].first > dist[u].first + weight)
            {
                /* If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them,
                    we would never reach here. */
                if (dist[v].first != INF)
                {
                    setds.erase(setds.find(make_pair(dist[v].first, v)));
                }

                // Updating distance of v
                dist[v].first = dist[u].first + weight;
                dist[v].second = u;
                setds.insert(make_pair(dist[v].first, v));
            }
        
        }
    }

    // Print shortest distances stored in dist[]
    /*cout << ("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        cout << i << " "<< "dist[i] = " << dist[i] << endl;
*/
}

void Graph::shortestPathK2Final (int src, vector<pair<int, int>> &dist)
{
    // Create a set to store vertices that are being
    // processed

    set<pair<int, int>> setds;
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src].first = 0;
    dist[src].second = src; //vettore, prima posizione distanza rispetto src, seconda posizione contiene nodo da cui deriva

    /* Looping till all shortest distance are finalized
    then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;

        // 'i' is used to get all adjacent vertices of a vertex
        for (pair<int, pair<int, int>> i : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = i.first;
            int weight;
            if (i.second.second != 0)
            {
                weight = edge[u][v] ? i.second.first : i.second.second;
            }
            else
            {
                weight = i.second.first;
            }

            // If there is shorter path to v through u.
            if (dist[v].first > dist[u].first + weight)
            {
                /* If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them,
                    we would never reach here. */
                if (dist[v].first != INF)
                {
                    setds.erase(setds.find(make_pair(dist[v].first, v)));
                }

                // Updating distance of v
                dist[v].first = dist[u].first + weight;
                dist[v].second = u;
                setds.insert(make_pair(dist[v].first, v));
            }
        
        }
    }

    // Print shortest distances stored in dist[]
    /*cout << ("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        cout << i << " "<< "dist[i] = " << dist[i] << endl;
*/
}

// Driver program to test methods of graph class
int main()
{

    int N, M, K, I, S, F; // Numero di nodi, Numero di corridoi semplici, Numero di corridoi con ventole, Posizione iniziale Impostore, Posizione iniziale Studenti, Posizione finale
    in >> N >> M >> K;
    in >> I >> S >> F; // Inserimento // Numero di nodi, Numero di corridoi semplici, Numero di corridoi con ventole, Posizione iniziale Impostore, Posizione iniziale Studenti, Posizione finale
    Graph g(N);        // crea grafo con N nodi

    int a, b, c, d; // variabili temporanee per inserimento
    int index = 0;  // Indice nodicout
    // Create a vector for distances and initialize all
    vector<pair<int, int>> distS(N, make_pair(INF, -1));
    //vettore, prima posizione distanza rispetto src, seconda posizione contiene nodo da cui deriva
    vector<pair<int, int>> distI(N, make_pair(INF, -1));
    // distances as infinite (INF)
    vector<pair<pair<int, int>, pair<int, int>>> ks;

    while (index < M)
    {
        in >> a;
        in >> b;
        in >> c;
        g.addEdge(a, b, c, 0);
        index++;
    }
    index = 0;
    while (index < K)
    {
        in >> a;
        in >> b;
        in >> c;
        in >> d;
        g.addEdge(a, b, c, d);
        ks.push_back(make_pair(make_pair(a, b), make_pair(c, d)));
        index++;
    }
    if (K == 0)
    {
        g.shortestPathK0(S, distS);
        int a = distS[F].first;
        g.shortestPathK0(I, distI);
        int b = distI[F].first;
        // cout << "risultatoS = " << a << endl;
        // cout << "risultatoI = " << b << endl;
        if (a == b)
        {
            out << 0 << endl;
            out << b << " " << a << endl;
            // cout << 0 << endl;
        }
        else if (a < b)
        {
            out << 2 << endl;
            out << b << " " << a << endl;
            // cout << 2 << endl;
        }
        else
        {
            out << 1 << endl;
            out << b << " " << a << endl;
            // cout << 1 << endl;
        }
        out << endl;

        int index = F;
        stack<int> listaNodi;
        listaNodi.push(F);
        while (distI[index].second != I)
        {
            listaNodi.push(distI[index].second);
            index = distI[index].second;
        }
        listaNodi.push(I);

        out << listaNodi.size() << endl;
        while (!listaNodi.empty())
        {
            out << listaNodi.top() << " ";
            listaNodi.pop();
        }
        out << endl;
    }
    else if (K == 1)
    {
        vector<pair<int, int>> distSMIN(N, make_pair(INF, -1));
        vector<pair<int, int>> distIMIN(N, make_pair(INF, -1));
        vector<pair<int, int>> distSMAX(N, make_pair(INF, -1));
        vector<pair<int, int>> distIMAX(N, make_pair(INF, -1));
    
        int i, s;

        g.shortestPathK1(true, I, distIMIN);
        int IMIN = distIMIN[F].first;
        
        set<pair<int, int>> listaNodiIMin;
        //parte corretta
        listaNodiIMin.insert(make_pair(distIMIN[F].second, F));
        int index = distIMIN[F].second;
        while (index != I)
        {
            listaNodiIMin.insert(make_pair(distIMIN[index].second, index));
            index = distIMIN[index].second;
        }
        listaNodiIMin.insert(make_pair(I, I));

        if (listaNodiIMin.find(make_pair((*(ks.begin())).first.first, (*(ks.begin())).first.second)) == listaNodiIMin.end())
        {
            g.shortestPathK1(false, S, distSMAX);
            int SMAX = distSMAX[F].first;

            i = IMIN;
            s = SMAX;
            // cout << (*(ks.begin())).first << " - " << (*(ks.begin())).second << " - " << endl;

            if (i < s)
            {
                out << 1 << endl;
            }
            else if (i == s)
            {
                out << 0 << endl;
            }
            else
            {
                out << 2 << endl;
            }
            out << i << " " << s << endl;
            out << (*(ks.begin())).second.second << endl;
            distI = distIMIN;
            distS = distSMAX;
        }
        else
        {
            g.shortestPathK1(true, S, distSMIN);
            int SMIN = distSMIN[F].first;
            //parte corretta
            set<pair<int, int>> listaNodiSMin;
            listaNodiSMin.insert(make_pair(distSMIN[F].second, F));
            int index = distSMIN[F].second;
            while (index != S)
            {
                listaNodiSMin.insert(make_pair(distSMIN[index].second, index));
                index = distSMIN[index].second;
            }
            listaNodiSMin.insert(make_pair(S, S));

            if (listaNodiSMin.find(make_pair((*(ks.begin())).first.first, (*(ks.begin())).first.second)) == listaNodiSMin.end())
            {
                i = IMIN;
                s = SMIN;
                if (i < s)
                {
                    out << 1 << endl;
                }
                else if (i == s)
                {
                    out << 0 << endl;
                }
                else
                {
                    out << 2 << endl;
                }
                out << i << " " << s << endl;
                out << (*(ks.begin())).second.first << endl;
                distI = distIMIN;
                distS = distSMIN;
            }
            else
            {
                i = IMIN;
                s = SMIN;
                if (i < s)
                {
                    out << 1 << endl;
                    out << i << " " << s << endl;
                    out << (*(ks.begin())).second.first << endl;
                    distI = distIMIN; //cosa aggiunta
                    distS = distSMIN; // " "
                }
                else
                {
                    g.shortestPathK1(false, I, distIMAX);
                    int IMAX = distIMAX[F].first;
                    g.shortestPathK1(false, S, distSMAX);
                    int SMAX = distSMAX[F].first;

                    i = IMAX;
                    distI = distIMAX;
                    s = SMAX;
                    distS = distSMAX;
                    if (i < s)
                    {
                        out << 1 << endl;
                        out << i << " " << s << endl;
                        out << (*(ks.begin())).second.second << endl;

                    }
                    else if (i == s)
                    {
                        if (IMIN == SMIN)
                        {
                            i = IMIN;
                            distI = distIMIN;
                            s = SMIN;
                            distS = distSMIN; 
                            out << 0 << endl;
                            out << i << " " << s << endl;
                            out << (*(ks.begin())).second.first << endl;
                        }
                        else
                        {
                            out << 0 << endl;
                            out << i << " " << s << endl;
                            out << (*(ks.begin())).second.second << endl;
                        }
                    }
                    else if (IMIN == SMIN && i > s)
                    {
                        i = IMIN;
                        distI = distIMIN;
                        s = SMIN;
                        distS = distSMIN;

                        out << 0 << endl;
                        out << i << " " << s << endl;
                        out << (*(ks.begin())).second.first << endl;
                    }
                    else
                    {
                        i = IMIN;
                        distI = distIMIN;
                        s = SMIN;
                        distS = distSMIN;
                        out << 2 << endl;
                        out << i << " " << s << endl;
                        out << (*(ks.begin())).second.first << endl;
                    }
                }
            }
        }
        index = F;
        stack<int> listaNodi;
        listaNodi.push(F);
        while (distI[index].second != I)
        {
            listaNodi.push(distI[index].second);
            index = distI[index].second;
        }
        listaNodi.push(I);

        out << listaNodi.size() << endl;
        while (!listaNodi.empty())
        {
            out << listaNodi.top() << " ";
            listaNodi.pop();
        }
        out << endl;
    } else {
        vector<pair<int, int>> distSMAX(N, make_pair(INF, -1));
        vector<pair<int, int>> distSMAX2(N, make_pair(INF, -1));
        vector<pair<int, int>> distIMIN(N, make_pair(INF, -1));
        vector<pair<int, int>> distIFIN(N, make_pair(INF, -1));
        vector<pair<int, int>> distSFIN(N, make_pair(INF, -1));

        stack<int> listaNodi;
        g.shortestPathK1(false,S,distSMAX);
        g.shortestPathK2Impostor(I,distIMIN,distSMAX);
        

        int IMIN = distIMIN[F].first;
        int SMIN = distSMAX[F].first;

        if(IMIN < SMIN){
            // I vince
            out << 1 << endl;
            int index = F;
            while (index != I)
            {
                g.reduceEdge(distIMIN[index].second,index);
                index = distIMIN[index].second;
            }

            index = F;
            listaNodi.push(F);
            while (distIMIN[index].second != I)
            {
                listaNodi.push(distIMIN[index].second);
                index = distIMIN[index].second;
            }
            listaNodi.push(I);

        } else {
            // check pareggio
            vector<pair<int, int>> distIPareggio(N, make_pair(INF, -1));
            g.shortestPathK2Pareggio(I,distIPareggio,distSMAX);

            IMIN = distIPareggio[F].first;
            cout << SMIN << endl;

            if(IMIN <= SMIN){
                // pareggio
                out << 0 << endl;

                int index = F;
                while (index != I)
                {
                    g.reduceEdge(distIPareggio[index].second,index);
                    index = distIPareggio[index].second;
                }

                index = F;
            
                listaNodi.push(F);
                while (distIPareggio[index].second != I)
                {
                    listaNodi.push(distIPareggio[index].second);
                    index = distIPareggio[index].second;
                }

                listaNodi.push(I);

                vector<pair<int, int>> distIPareggio2(N, make_pair(INF, -1));
                g.shortestPathK2Pareggio(I,distIPareggio2,distSMAX);

                SMIN = distIPareggio2[F].first;

            }else{
                // sconfitta
                out << 2 << endl;
                while (index != I)
                {
                    g.reduceEdge(distIMIN[index].second,index);
                    index = distIMIN[index].second;
                }

                index = F;
            
                listaNodi.push(F);
                while (distIMIN[index].second != I)
                {
                    listaNodi.push(distIMIN[index].second);
                    index = distIMIN[index].second;
                }

                listaNodi.push(I);
            }
            
        }

        /* da qui in poi dovrei rifare il dikstra finale*/
        g.shortestPathK2Final(I,distIFIN);
        g.shortestPathK2Final(S,distSFIN);

        out << distIFIN[F].first << " " << distSFIN[F].first << endl;
        //out << IMIN << " " << SMIN << endl;
        for (pair<pair<int, int>, pair<int, int>> i : ks) {
            int value = g.getEdge(i.first.first,i.first.second) ? i.second.first : i.second.second;
            out << value << " ";
        }
        out << endl;



        out << listaNodi.size() << endl;
        while (!listaNodi.empty())
        {
            out << listaNodi.top() << " ";
            listaNodi.pop();
        }
        out << endl;
    }

    return 0;
}