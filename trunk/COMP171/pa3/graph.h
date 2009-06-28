/*
  Name: XIAO Sa
  Student ID: 07592272
  Lecture Section: L1
  Tutorial Section: T1C
  I did not cheat in this assignment
  Acknowledge: The textbook "Data Structures and Algorithm Analysis in C++"
  and the lecture notes. The work included in this program is all my own work.
  I did not cheat in this assignment.
*/
#ifndef _GRAPH_H_
#define _GRAPH_H_

// Max number of vertices in the graph
#define GRAPH_MAXSIZE 50

#include <string>
#include "queue.h"
using namespace std;

/* This vertex struct is to store vertex information.
	Every vertex should have an unique index
*/
struct Vertex {
	int index;
	int neighbor[GRAPH_MAXSIZE];	// storing indexes of all the outgoing neighbors' vertices
	int numEdges;	// number of outgoing edges of this vertex
};

/* This path struct is to store a path in a graph.
*/
struct Path {
	int numNodes;	// number of nodes of this path
	int node[GRAPH_MAXSIZE];		// array to store all node indexes in this path (max GRAPH_MAXSIZE nodes)
};

/* This Graph class is to model a directed graph.
	This class also implements different features and functionailities of a graph.
	Remarks: Adjacency lists are used for storing edges (for faster BFS)
	Remarks: Assume max 50 vertices, vertex index must be in [0..49]
*/
class Graph {
public:
	// Default constructor to construct a blank graph
	Graph();

	// Default destructor
	~Graph();

	/* Perform BFS on the graph given the starting vertex
		Return the shortest path found by BFS
		Return an empty path of zero nodes if the input index is invalid or shortest path not exists
	*/
	Path performBFS(int start_index, int dest_index);

	/* Add a vertex to the graph
		Return false and No effect if the input index is invalid or already exists
	*/
	bool addVertex(int index);

	/* Remove a vertex from the graph, together with the related edges
		Return false and No effect if the input index is invalid or does not exist
	*/
	bool removeVertex(int index);

	/* Add an edge to the graph as indicated in the arguments
		Return false and No effect if any of the indexes is invalid or the edge already exists
	*/
	bool addEdge(int start_index, int dest_index);

	/* Remove an edge from the graph as indicated in the arguments, together with the related edges
		Return false and No effect if any of the indexes is invalid or the edge does not exist
	*/
	bool removeEdge(int start_index, int dest_index);

	/* Remove all the vertices and edges
	*/
	void clear();



/*
======================================
Add your own member functions here only.
Do not change anything else
=======================================
*/


/*
End of additional member functions
=======================================
*/



private:
	/* array of pointer to all vertices
		node[i] should store the pointer to Vertex of index i
		node[i] should point to NULL if there are no Vertex of index i
	*/
	Vertex* node[GRAPH_MAXSIZE];
};

#endif
