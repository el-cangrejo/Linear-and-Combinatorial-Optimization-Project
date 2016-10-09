#include "ClpSimplex.hpp"


int main(int argc, char const *argv[]) {
	
	ClpSimplex  model;
	model.setOptimizationDirection(-1);
  int status;
  
  if (argc < 2)
    status=model.readMps("dual.mps");
  else
    status=model.readMps(argv[1]);
  if (!status) {
    model.primal();
  }

	return 0; 
}