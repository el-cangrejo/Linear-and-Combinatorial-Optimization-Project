#include "ClpSimplex.hpp"
#include "CoinHelperFunctions.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

int nodes_size = 0;
std::vector<std::vector<int>> ADJAC;

struct edge {
  int u;
  int v;
};

std::vector<edge> loadAdjacency(std::string );
std::vector<std::vector<int> > adjacencyToMatrix(std::vector<edge> );

int main(int argc, const char *argv[])
{
  ClpSimplex  model;
  // model is as exmip1.mps from Data/samples
  std::vector<edge> edges = loadAdjacency(argv[1]);

  int numberRows = edges.size();
  int numberColumns = nodes_size;
  int numberElements = 2 * numberRows;

  std::vector<CoinBigIndex> start;
  std::vector<int> length;
  std::vector<int> rows;
  std::vector<double> elements(numberElements, 1);

  for (int i = 0; i < numberColumns; ++i) 
      length.push_back(std::accumulate(ADJAC[i].begin(), ADJAC[i].end(), 0));

  std::vector<std::vector<int>> mat = adjacencyToMatrix(edges);

  for (int i = 0; i < mat[1].size(); ++i) {
      for (int j = 0; j < mat.size(); ++j) {
           if (mat[j][i] == 1) {
                rows.push_back(j);
           }
      }
  }

  start.push_back(0);
  for (int i = 0; i < mat[1].size(); ++i) {
    int count = 0;
    for (int j = 0; j < mat.size(); ++j) {
        if (mat[j][i] != 0) ++count;
    }
    start.push_back(count + start.back());
  }
  start.push_back(numberElements);
  if (start.size() == numberColumns + 1) {
    std::cout << "Succesful start vector\n"; 
  }
   

  std::cout << "Started packed matrix! \n";
  std::cout << "numberRows " << numberRows << " \n";
  std::cout << "numberColumns " << numberColumns << " \n";
  std::cout << "numberElements " << numberElements << " \n";


  std::vector<double> colLower(numberColumns, 0);
  std::vector<double> colUpper(numberColumns, COIN_DBL_MAX);
  std::vector<double> rowLower(numberRows, 1);
  std::vector<double> rowUpper(numberRows, COIN_DBL_MAX);
  std::vector<double> objective(numberColumns, 1);

  std::cout << "Loaded matrix to solver! \n";

  CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements.data(), rows.data(), start.data(), length.data());

  model.loadProblem(matrix, colLower.data(), colUpper.data(), objective.data(), rowLower.data(), rowUpper.data());


  // for (int i = 0; i < numberColumns; ++i) {
  //     model.setInteger(i);
  // }
  
  model.setOptimizationDirection(1);

  // Solve
  model.initialSolve();

  // Solution
  int num_rows = model.numberRows();
  double * rowPrimal = model.primalRowSolution();
  double * rowDual = model.dualRowSolution();

  for (int iRow = 0; iRow < num_rows; ++iRow)   
    std::cout << "Row " << iRow 
              << ", primal " << rowPrimal[iRow] << "\n";
  
  for (int iRow = 0; iRow < num_rows; ++iRow)   
    std::cout << "Row " << iRow 
              << ", dual " << rowDual[iRow] << "\n";

  const double * solution = model.primalColumnSolution();
  int i;
  for (i = 0; i < numberColumns; i++)
      if (solution[i])
           printf("Column %d has value %g\n", i, solution[i]);

  std::cout << "Finished printing\n";

  return 0;
}

std::vector<edge> loadAdjacency(std::string filename) {  
  std::vector<edge> edges;
  std::vector<std::vector<int> > adjacency;
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

    std::cout << "Number of vertices " << num_verts << "\n";

    for (int j = 0; j < num_verts; ++j) {
      adjacency[j].resize(num_verts);

      getline(adjacency_file, line);
      std::istringstream in(line);
      int edge;

      std::cout << "j " << j << "\n";

      for (int i = 0; i < num_verts; ++i) {
        std::cout << "i " << i << "\n";

        in >> edge;
        adjacency[j][i] = edge;
      }
    }
  } else {
    std::cout << "Could not open file\n";
  }

  nodes_size = adjacency.size();

  std::cout << "Started making edges vector \n";
  for (int i = 0; i < adjacency.size(); ++i) {
    for (int j = i; j < adjacency.size(); ++j) {
      if (adjacency[i][j] == 1) {
        edge e;
        e.u = i;
        e.v = j;
        edges.push_back(e);
      }
    }
  }
  ADJAC = adjacency;

  std::cout << "Finished reading adjacency file\n";
  return edges;
}

std::vector<std::vector<int> > adjacencyToMatrix(std::vector<edge> edges) {
  std::vector<std::vector<int> > mat;
  mat.resize(edges.size());
  for (int i = 0; i < edges.size(); ++i) {
    mat[i].resize(nodes_size);
    for (int j = 0; j < nodes_size; ++j) {
      mat[i][j] = 0;
    }
    mat[i][edges[i].u] = 1;
    mat[i][edges[i].v] = 1;
  } 

  return mat;
}