#include "Graph.hpp"

#include <algorithm>
#include <random>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace graph {

Node::Node() : _id(0) {}
Node::Node(int id) : _id(id) {}
int Node::ID() const { return _id; }
bool Node::operator==(const Node other) const { return _id == other.ID(); }
Node::~Node() {}

Edge::Edge() : u(Node()), v(Node()) {}
Edge::Edge(Node _u, Node _v) : u(_u), v(_v) {}
bool Edge::operator==(const Edge other) const { 
  return (u == other.u && v == other.v) |
         (u == other.v && v == other.u); }
Edge::~Edge() {}

Graph::Graph() {}
Graph::~Graph() {}
void Graph::randomGraph(int numV) {
  pushBackNodes(numV);
  randomEdges();

  std::cout << "\033[1;34mGraph vertex size "
    << nodes.size() << " \n"
    << "Graph edge size "
    << edges.size() << "\033[0m\n";
}
 
void Graph::randomBiGraph(int numV) {
  pushBackNodes(numV);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> bi(nodes.size() / 2 - 1, nodes.size() / 2 + 1);

  int x = bi(gen);
  std::vector<Node> U, V;
  U = getNodes(nodes, 0, x);
  V = getNodes(nodes, x, nodes.size());
  randomEdges(U, V);

  std::cout << "\033[1;34m Graph vertex size "
  << nodes.size() << " \n"
  << "Graph edge size "
  << edges.size() << "\033[0m\n";
}

void Graph::randomEdges(std::vector<Node> &U, std::vector<Node> &V) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> u_dist(0, U.size() - 1);
  std::uniform_int_distribution<> vu_dist(0, V.size() - 1);
  std::uniform_int_distribution<> v_dist(5, 6);

  for (int i = 0; i < U.size(); ++i) {
    int numV = v_dist(gen);
    for (int j = 0; j < numV; ++j) {
      int rnd_node = v_dist(gen);
      if (rnd_node == i)
        continue;
      Edge to_be_inserted(V[i], V[rnd_node]);

      if (std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == to_be_inserted; }) == std::end(edges)) {
        edges.push_back(to_be_inserted);
      }
    }
  }
  for (int i = 0; i < V.size(); ++i) {
    int numU = v_dist(gen);
    for (int j = 0; j < numU; ++j) {
      int rnd_node = vu_dist(gen);
      if (rnd_node == i)
        continue;
      Edge to_be_inserted(U[i], U[rnd_node]);

      if (std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == to_be_inserted; }) == std::end(edges)) {
        edges.push_back(to_be_inserted);
      }
    }
  }
}

void Graph::pushBackNodes(int numV) {
  for (int i = 0; i < numV; ++i) {
    nodes.push_back(Node(i));
  }
}

std::vector<Node> Graph::getNodes(std::vector<Node> V, int n1, int n2) {
  std::vector<Node> v;
  for (int i = n1; i < n2; ++i) {
    v.push_back(V[i]);
  }
  return std::move(v);
}

void Graph::randomEdges() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> node_dist(0, nodes.size() - 1);
  // std::uniform_int_distribution<> edge_dist(1, nodes.size() - 1);
  // std::geometric_distribution<> edge_dist;
  std::binomial_distribution<> edge_dist(2, 0.5);
  
  for (int i = 0; i < nodes.size(); ++i) {
    int numE = edge_dist(gen) + 1;
    for (int j = 0; j < numE; ++j) {
      int rnd_node = node_dist(gen);
      if (rnd_node == i)
        continue;
      Edge to_be_inserted(nodes[i], nodes[rnd_node]);

      if (std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == to_be_inserted; }) == std::end(edges)) {
        edges.push_back(to_be_inserted);
      }
    }
  }
}

std::vector<Node> Graph::vertexCover() {
  std::vector<Node> vc;
  std::vector<Edge> edge;
  edge = edges;
  while (edge.size() > 0) {
    Edge e = edge[0];
    vc.push_back(e.u);
    vc.push_back(e.v);
    remove_incident_edge(edge, e.u);
    remove_incident_edge(edge, e.v);
  }
  return vc;
}

void Graph::Complement() {
  for (const auto &n1 : nodes) {
    for (const auto &n2 : nodes) {
      if (n2 == n1) continue;
      Edge _e(n1, n2);
      if (!(std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == _e; }) == std::end(edges))) continue;

      comp_edges.push_back(Edge(n1, n2));
    }
  }
}

void Graph::computeAdjacency() {
  if (adjacency.size() > 0) return;

  for (int i = 0; i < nodes.size(); ++i) {
    adjacency.push_back(std::vector<int>());
    for (int j = 0; j < nodes.size(); ++j) {
      Edge _e(nodes[i], nodes[j]);
      if (!(std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == _e; }) == std::end(edges))) {
        adjacency[i].push_back(1);
        continue;
      }
      adjacency[i].push_back(0);
    }
  }
}

void Graph::printAdjacency() {
  std::cout << "\033[1;33mADJACENCY MATRIX: \n\033[0m";
  std::cout << "\t";
  for (int i = 0; i < nodes.size(); ++i) {
    std::cout << "\033[1;33m" << nodes[i].ID() << "\t\033[0m";
  }
  std::cout << "\n";

  for (int i = 0; i < nodes.size(); ++i) {
    std::cout << "\033[1;33m" << nodes[i].ID() << "\t\033[0m";
    for (int j = 0; j < nodes.size(); ++j) {
      if (adjacency[i][j] == 0) {
        std::cout << 0 << "\t";
        continue;
      }
      std::cout << "\033[1;34m" << 1 << "\t\033[0m";
    }
    std::cout << "\n";
  }
}

std::vector<Node> Graph::vertexCoverAdjacency() {
  std::vector<Node> vertex_cover;
  std::vector<std::vector<int>> adjacency_new;
  adjacency_new = adjacency;
  
  printAdjacency();

  for (int i = 0; i < adjacency_new[0].size(); ++i) {
    for (int j = i; j < adjacency_new[0].size(); ++j) {
      //*
      if (adjacency[i][j] == 1) {
        Node u(i);
        Node v(j);
        vertex_cover.push_back(u);  
        vertex_cover.push_back(v);  
        for (int k = 0; k < adjacency_new[0].size(); ++k) {
          adjacency[j][k] = 0;
          adjacency[k][j] = 0;
        }
        
      }
      //*/

      /*
      if (adjacency[i][j] == 1) {
        Node u(i);
        Node v(j);
        if (find_if(std::begin(vertex_cover), std::end(vertex_cover), [&](Node e) {
            return e == v; }) == std::end(vertex_cover)) {
          vertex_cover.push_back(v);  
        }
        if (find_if(std::begin(vertex_cover), std::end(vertex_cover), [&](Node e) {
            return e == u; }) == std::end(vertex_cover)) {
          vertex_cover.push_back(u);
        }
        adjacency[i][j] = 0;
        for (int k = 0; k < adjacency_new[0].size(); ++k) {
          adjacency[j][k] = 0;
          adjacency[k][j] = 0;
        }
      }
      //*/
    }
    printAdjacency();
  }
  return vertex_cover;
}

void Graph::readAdjacency(std::string filename) {

  std::ifstream adjacency_file;
  adjacency_file.open(filename);

  std::string line;
  
  if (adjacency_file.is_open()) {
    std::cout << "Started reading\n";

    getline(adjacency_file, line);
    std::string adj;
    int num_verts(0);

    std::istringstream in_(line);
    in_ >> adj;
    in_ >> num_verts;
    for (int j = 0; j < num_verts; ++j) {
      nodes.push_back(Node(j+1));

      getline(adjacency_file, line);
      std::istringstream in(line);
      int edge;

      for (int i = 0; i < num_verts; ++i) {
        in >> edge;
        Edge _e(Node(i+1), Node(j+1));
        if (edge && (std::find_if(std::begin(edges), std::end(edges), [&](Edge e) {
            return e == _e; }) == std::end(edges))) {
          edges.push_back(Edge(Node(i+1), Node(j+1)));
        }        
      } 
    }

  } else {
    std::cout << "Could not open file\n";
  }
}

void Graph::exportAdjacency(std::string filename) {
	if (adjacency.size() == 0) {
		std::cout << "Adjacency matrix not computed\n";
		return;
	}

	std::ofstream adjacency_file;
  adjacency_file.open(filename);

  if (adjacency_file.is_open()) {
	
		adjacency_file << "ADJACENCY " << nodes.size() << "\n";
  	for (int i = 0; i < nodes.size(); ++i) {
 			for (int j = 0; j < nodes.size(); ++j) {
 				adjacency_file << adjacency[i][j] << " "; 
 			}
 			adjacency_file << "\n";
  	}
	} else {
	  std::cout << "Could not open file\n";
	}
}

void remove_incident_edge(std::vector<Edge> &edges, Node u) {
  int c = 0;
  for (auto it = edges.begin(); it != edges.end();) {
    if ((*it).u == u | (*it).v == u) {
      edges.erase(it);
      c++;
    } else {
      ++it;
    }
  }
  std::cout << "\033[1;31mRemoved " << c << " edges! \033[0m \n";
}

std::vector<std::vector<int>> Graph::loadAdjacency(std::string filename) {
	std::vector<std::vector<int>> adjacency;
	std::ifstream adjacency_file;
  adjacency_file.open(filename);

  std::string line;
  
  if (adjacency_file.is_open()) {
    std::cout << "Started reading\n";

    getline(adjacency_file, line);
    std::string adj;
    int num_verts(0);

    std::istringstream in_(line);
    in_ >> adj;
    in_ >> num_verts;
    adjacency.resize(num_verts);
    
    for (int j = 0; j < num_verts; ++j) {
      adjacency[j].resize(num_verts);

      getline(adjacency_file, line);
      std::istringstream in(line);
      int edge;

      for (int i = 0; i < num_verts; ++i) {
      	in >> edge;
      	adjacency[i][j] = edge;
      }
    }

  } else {
    std::cout << "Could not open file\n";
  }
  return adjacency;
}

std::vector<std::vector<int>> Graph::adjacencyToMatrix() {
	std::vector<std::vector<int>> mat;
	std::cout << "Edges " << edges.size() << "\n";	
	mat.resize(edges.size());
	for (int i = 0; i < edges.size(); ++i) {
		mat[i].resize(nodes.size());
		for (int j = 0; j < nodes.size(); ++j) {
			mat[i][j] = 0;
		}
		mat[i][edges[i].u.ID() - 1] = 1;
		mat[i][edges[i].v.ID() - 1] = 1;
	}	

	std::vector<int> rows;
	for (int i = 0; i < mat[1].size(); ++i) {
    for (int j = 0; j < mat.size(); ++j) {
      if (mat[j][i] == 1) {
        rows.push_back(j);
      }
    }
  }
  for (int i = 0; i < rows.size(); ++i) {
  	std::cout << rows[i] << " ";
  }
  std::cout << "\n";

	return mat;
}

void print_mat(std::vector<std::vector<int>> mat) {
	for (int i = 0; i < mat.size(); ++i) {
		for (int j = 0; j < mat[i].size(); ++j) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << "\n";
	}
}

}

