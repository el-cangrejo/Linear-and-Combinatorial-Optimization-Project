#include "Visuals.hpp"
#include "Graph.hpp"

#include <GL/glut.h>
#include <cmath>

extern graph::Graph graph_rnd;
extern std::vector<graph::Node> VC;

int main(int argc, char *argv[]) {
  
  if (argc <= 1) {
    std::cout << "Give number of nodes!\n"; 
  } else if (std::isdigit(argv[1][0])) {
  	int num  = atoi(argv[1]);
  	graph_rnd.randomGraph(num);
  } else {
    graph_rnd.readAdjacency(argv[1]);
  }
  
  VC = graph_rnd.vertexCover();
  graph_rnd.Complement();
  
  graph_rnd.computeAdjacency();
  graph_rnd.printAdjacency();
  graph_rnd.exportAdjacency("adjacency" + std::to_string(graph_rnd.nodes.size()) + ".adj");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 25);
  glutInitWindowSize(900, 620); // w h
  glutCreateWindow("OpenGL Window");

  initializeGL();

  // GLUT Display Function
  glutDisplayFunc(renderScene);
  glutReshapeFunc(resize);

  // Keyboard handlers
  glutKeyboardFunc(keyboardDown);

  // Mouse handlers
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouseMotion);

  // GLUT Main Lopp
  glutMainLoop();

  return 0;
}
