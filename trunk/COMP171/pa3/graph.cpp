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
#include "graph.h"

/**
 * \brief Default constructor to construct a blank graph.Make all the pointers to be NULL.
 */
Graph::Graph()
{
  // set each pointer to be NULL
  for (int i = 0; i < GRAPH_MAXSIZE; i++)
    {
      node[i] = NULL;
    }
}

/**
 * \brief Default destructor to release the dynamic memory.
 */
Graph::~Graph()
{
  // release the dynamic memory and set the pointer to be NULL for safty
  for (int i = 0; i < GRAPH_MAXSIZE; i++)
    {
      delete node[i];
      node[i] = NULL;
    }
}

/**
 * \brief Perform BFS on the graph given te starting vertex and ending vertex. 
 * Find the shortest path from the startint vertex to the ending vertex. 
 * \param start_index The vetex which path starts from.
 * \param dest_index The vertex which path ends in. 
 * \return The shortest path found by BFS(return an empty path of zero nodes if the 
 * The input index is invalid or shortest path not exists.
 */
Path Graph::performBFS(int start_index, int dest_index)
{
  Path path1, path2, path3;
  path1.numNodes = 0;
  path2.numNodes = 0;
  path3.numNodes = 0;

  // make path1, path2 and path3 to be empty path
  for (int i = 0; i < GRAPH_MAXSIZE; i++)
    {
      path1.node[i] = -1;
      path2.node[i] = -1;
      path3.node[i] = -1;
    }
  
  // reutrn empty path if the input index is invalid
  if (start_index == -1 || dest_index == -1)
    {
      return path1;
    }
  
  // Given an source vertex node[start_index], BFS visits the other
  // vertices at increasing distances away from node[start_index]. In 
  // each step, BFS visits all reachable vertices with constant distance.
  // And the BFS is end up with find the node[dest_index]. If it doesn't 
  // find node[dest_index], return an empty path.
  Queue q;
  q.enqueue(start_index);
  int v = -1;
  path1.node[start_index] = 50;
  
  while (q.size() != 0)
    {
      v = q.dequeue();
      
      // If find node[dest_index], the BFS finishes
      if (v == dest_index)
	{
	  break;
	}
      
      // visit all reachable vertices
      for (int i = 0; i < GRAPH_MAXSIZE; i++)
	{
	  // jump out of the loop if all the reachable vertices are marked
	  if (node[v]->neighbor[i] == -1)
	    {
	      break;
	    }
	  
	  // mark the rachable vertices and record vertex in the path
	  if (path1.node[node[v]->neighbor[i]] == -1)
	    {
	      q.enqueue(node[v]->neighbor[i]);
	      path1.node[node[v]->neighbor[i]] = v;
	    }
	}
    }
  
  // if BFS doesn't find node[dest_index], return an empty path
  if (v != dest_index)
    {
      return path2;
    }
  else
    {
      // check whether the start_index equals to dest_index
      if (start_index == dest_index)
	{
	  path2.node[path2.numNodes++] = v;
	  return path2;
	}

      // push the vertices in the path into stack
      while (v != 50)
     	{
	  path2.node[path2.numNodes++] = v;
	  v = path1.node[v];
	}
      
      // pop the verices from the stack
      while (path2.numNodes != 0)
	{
	  path3.node[path3.numNodes++] = path2.node[--path2.numNodes];
	}
      return path3;
    }
}

/**
 * \brief Add a vertex to the graph.
 * \param The index of the vertex to be added.
 * \return False and do nothing if the index is invalid or already 
 * exists. Otherwise, return true.
 */
bool Graph::addVertex(int index)
{
  // check whether the index is invalid or already exists
  if (index < 0 || index > 49 || node[index] != NULL)
    {
      return false;
    }
  else
    {
      node[index] = new Vertex;
      node[index]->index = index;
      node[index]->numEdges = 0;
      
      // make the index of outgoing neighbor's vertices to be -1, which is an empty array
      for (int i = 0; i < GRAPH_MAXSIZE; i++)
	{
	  node[index]->neighbor[i] = -1;
	}
      return true;
    }
}

/**
 * \brief Remove a vertex form the graph, together with related edges.
 * \param The index of the vertex to be removed.
 * \return Flase and do nothing if the index is invalid or 
 * doesn't exist. Otherwise, return True.
 */
bool Graph::removeVertex(int index)
{
  // check whether the index is invalid or exists
  if (index < 0 || index > 49 || node[index] == NULL)
    {
      return false;
    }
  else
    {
      int i = 0;
      int j = 0;
      
      while (i < GRAPH_MAXSIZE)
	{
	  if (i == index)
	    {
	      i++;
	      continue;
	    }
	  
	  // remove all the edges related with vertex of the input index
	  for (j = 0; j < GRAPH_MAXSIZE; j++)
	    {
	      if (node[i] == NULL)
		{
		  break;
		}
	      else if (node[i]->neighbor[j] == -1)
		{
		  break;
		}
	      else if (node[i]->neighbor[j] == index)
		{
		  removeEdge(i, index);
		}
	    }
	  i++;
	}
      delete node[index];
      node[index] = NULL;
    }
}

/**
 * \breif Add an edge to the graph as indicated in the arguments.
 * \param start_index The begining of the edge.
 * \param dest_index The end of the edge.
 * \return False and do nothing if any of the input index is invalid
 * or doesn't exist. Otherwise, return True.
 */
bool Graph::addEdge(int start_index, int dest_index)
{
  // check whether the input indexs are valid or exist
  if (start_index < 0 || start_index > 49
      || dest_index < 0 || dest_index > 49
      || start_index == dest_index
      || node[start_index] == NULL 
      || node[dest_index] == NULL)
    {
      return false;
    }
  else
    {
      int i = 0;
      
      while (i < GRAPH_MAXSIZE && node[start_index]->neighbor[i] != -1)
	{
	  // return false if the edge has already exists
	  if (node[start_index]->neighbor[i] == dest_index)
	    {
	      return false;
	    }
	  i++;
	}
      node[start_index]->neighbor[i] = dest_index;
      node[start_index]->numEdges += 1;
      return true;
    }
}

/**
 * \brief Remove an edge from the graph as indicated in the arguments.
 * \param start_index The beginning of the edge.
 * \param dest_index The end of the edg.
 * \return False and do nothing if the input indexes are invalid or do not exist.
 */
bool Graph::removeEdge(int start_index, int dest_index)
{
  // check whether the input indexes are invalid or exist
  if (start_index < 0 || start_index > 49
      || dest_index < 0 || dest_index > 49
      || start_index == dest_index
      || node[start_index] == NULL 
      || node[dest_index] == NULL)
    {
      return false;
    }
  else
    {
      int i = 0;
      
      while (i < GRAPH_MAXSIZE && node[start_index]->neighbor[i] != -1)
	{
	  if (node[start_index]->neighbor[i] == dest_index)
	    {
	      break;
	    }
	  i++;
	}
      
      if (node[start_index]->neighbor[i] != -1)
	{
	  // delete the neightbor[dest_index] by shifting the following neighbors
	  for (int j = i + 1; j < GRAPH_MAXSIZE; j++, i++)
	    {
	      if (node[start_index]->neighbor[i] == -1)
		{
		  break;
		}
	      node[start_index]->neighbor[i] = node[start_index]->neighbor[j];
	    }
	  node[start_index]->numEdges--;
	  return true;
	}
      else
	{
	  return false;
	}
    }
}

/**
 * \brief Remove all the vertices and edges.
 */
void Graph::clear()
{
  // release dynamic memory and set pointer to be NULL
  for (int i = 0; i < GRAPH_MAXSIZE; i++)
    {
      delete node[i];
      node[i] = NULL;
    }
}

