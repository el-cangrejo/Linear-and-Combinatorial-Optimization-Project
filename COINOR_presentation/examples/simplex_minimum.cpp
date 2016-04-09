#include "ClpSimplex.hpp"

int main (int argc, const char *argv[])
{
  ClpSimplex  model;
  int status;
  if (argc < 2)
    status=model.readMps("dovetail.mps");
  else
    status=model.readMps(argv[1]);
  if (!status) {
    model.primal();
  }
  return 0;
} 
