#include "ClpSimplex.hpp"

int main (int argc, const char *argv[])
{
  ClpSimplex  model;
  int status;
  
  if (argc < 2)
    status=model.readMps("p0033.mps");
  else
    status=model.readMps(argv[1]);
  if (!status) {
    model.setOptimizationDirection(-1);
    model.primal();
  }

  int numberRows = model.numberRows();
  double * rowPrimal = model.primalRowSolution();
  double * rowDual = model.dualRowSolution();

  for (int iRow = 0; iRow < numberRows; ++iRow)   
    std::cout << "Row " << iRow 
              << ", primal " << rowPrimal[iRow] << "\n";
  
  for (int iRow = 0; iRow < numberRows; ++iRow)   
    std::cout << "Row " << iRow 
              << ", dual " << rowDual[iRow] << "\n";
                
  int numberColumns = model.numberColumns();
  double * columnPrimal = model.primalColumnSolution();
  double * columnDual = model.dualColumnSolution();

  for (int iColumn = 0; iColumn < numberColumns; ++iColumn)   
    std::cout << "Column " << iColumn 
              << ", primal " << columnPrimal[iColumn] << "\n"; 

  for (int iColumn = 0; iColumn < numberColumns; ++iColumn)   
    std::cout << "Column " << iColumn  
              << ", dual " << columnDual[iColumn] << "\n"; 

  std::cout << "Objective value : " << model.objectiveValue() << "\n";

  double * objcoefs = model.objective();

  for (int i = 0; i < numberColumns; ++i) {
    std::cout << "c" << i << " = " << objcoefs[i] << "\t";
  }

  std::cout << std::endl;
  
  return 0;
} 
  
  