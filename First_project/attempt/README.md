# AmongASD - Algoritmi e Strutture Dati

## Problem Description
The problem revolves around determining the shortest path in a graph representing university's corridor system. The objective is to determine who will reach the FabLab first: Professor Montresus or the students. The corridors can have fixed traversal times or variable traversal times (due to fans that can be adjusted).

## Source Code Overview
The main components of `amongASD.cpp` are:
- A `Graph` class that represents the university's corridor system.
- Dijkstra's shortest path algorithm with certain modifications to account for the fans in the corridors.

### Main Functions
- `addEdge`: Adds an edge to the graph.
- `reduceEdge`: Modifies an edge in the graph.
- `shortestPathK0`, `shortestPathK1`, etc.: Functions to determine the shortest path under different conditions.
