#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <iterator>
namespace graph {

class Node {
public:
	Node();
	Node(int );
	int ID() const;
	bool operator==(const Node other) const;
	~Node();
private:
	int _id;
};

class Edge {
public:
	Edge();
	Edge(Node ,Node);
	bool operator==(const Edge other) const;
	~Edge();
	Node u, v;
};

class Graph {
public:
	Graph();
	~Graph();
	void randomGraph(int numV);
	std::vector<Node> vertexCover();
	std::vector<Node> vertexCoverAdjacency();
	void Complement();
 	void randomBiGraph(int numV);
	void printAdjacency(); 
  void readAdjacency(std::string filename);
  void exportAdjacency(std::string filename);
	void computeAdjacency(); 
	std::vector<Node> getNodes(std::vector<Node> V, int n1, int n2);

	std::vector<std::vector<int>> adjacencyToMatrix();
  std::vector<std::vector<int>> loadAdjacency(std::string filename);

	std::vector<Node> nodes;
	std::vector<Edge> edges;
	std::vector<Edge> comp_edges;
	std::vector<std::vector<int>> adjacency;

private:
	void randomEdges();
	void pushBackNodes(int numV);
	void randomEdges(std::vector<Node> &U, std::vector<Node> &V);
};

void remove_incident_edge(std::vector<Edge> &edges, Node u);

void print_mat(std::vector<std::vector<int>> );
}
#endif // GRAPH_HPP