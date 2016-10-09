#include "Visuals.hpp"

static bool ShowOrder = false;
static bool ShowComp = false;
static bool ShowEdges = true;
static bool BIGRAPH = false;

// Event handle variables
static bool mouseClickDown = false;
static int mx0 = 0;
static int my0 = 0;

graph::Graph graph_rnd;
std::vector<graph::Node> VC;

void drawGraph(graph::Graph &g) {
  float phi_n = (2 * PI) / g.nodes.size();
  for (int i = 0; i < g.nodes.size(); ++i) {
    glPushMatrix();
      glColor3f(1.0, 1.0, 1.0);
      int id = g.nodes[i].ID();
      float x = 0.5 * cos(id * phi_n);
      float y = 0.5 * sin(id * phi_n);
      glTranslatef(x, y, 0);
      glutWireSphere(0.015, 15, 15);
    glPopMatrix();
    glutSwapBuffers();
  }

  for (int i = 0; i < VC.size(); ++i) {
    glPushMatrix();
      glColor3f(1., 0., 0.);
      int id = VC[i].ID();
      float x = 0.5 * cos(id * phi_n);
      float y = 0.5 * sin(id * phi_n);
      glTranslatef(x, y, 0);
      if (ShowOrder)
        glutWireSphere((i + 1) * 0.015, 25, 25);
      else
        glutWireSphere(0.015, 15, 15);
    glPopMatrix();
    glutSwapBuffers();
  }
  
  if (ShowEdges) {
    glBegin(GL_LINES);
    for (int i = 0; i < g.edges.size(); ++i) {
      glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        int id_1 = g.edges[i].u.ID();
        float x_1 = 0.5 * cos(id_1 * phi_n);
        float y_1 = 0.5 * sin(id_1 * phi_n);
        int id_2 = g.edges[i].v.ID();
        float x_2 = 0.5 * cos(id_2 * phi_n);
        float y_2 = 0.5 * sin(id_2 * phi_n);
        glVertex3f(x_1, y_1, 0);
        glVertex3f(x_2, y_2, 0);
      glPopMatrix();
      glutSwapBuffers();
    }     
    glEnd();
  }

  if (ShowComp) {
    glBegin(GL_LINES);
    for (int i = 0; i < g.comp_edges.size(); ++i) {
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      int id_1 = g.comp_edges[i].u.ID();
      float x_1 = 0.5 * cos(id_1 * phi_n);
      float y_1 = 0.5 * sin(id_1 * phi_n);
      int id_2 = g.comp_edges[i].v.ID();
      float x_2 = 0.5 * cos(id_2 * phi_n);
      float y_2 = 0.5 * sin(id_2 * phi_n);
      glVertex3f(x_1, y_1, 0);
      glVertex3f(x_2, y_2, 0);
    glPopMatrix();
    glutSwapBuffers();
    }     
    glEnd();
  }

  glFlush();
}

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  drawGraph(graph_rnd);
}

void initializeGL(void) {
  glClearColor(0.1, 0.1, 0.1, 0.0);
}

void mouseClick(int button, int state, int x, int y) {
  if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
    mouseClickDown = true;
    mx0 = x;
    my0 = y;
  } else {
    mouseClickDown = false;
  }
}

void keyboardDown(unsigned char key, int x, int y) {
  switch (key) {
  case 'q': {
    std::cout << "User evoked exiting..\n";
    exit(0);
    break;
  }
  case 'R': 
  case 'r': {
    int n = graph_rnd.nodes.size();
    graph_rnd.nodes.clear();
    graph_rnd.nodes.shrink_to_fit();
    graph_rnd.edges.clear();
    graph_rnd.edges.shrink_to_fit();
    graph_rnd.adjacency.clear();
    graph_rnd.adjacency.shrink_to_fit();
    VC.clear();
    VC.shrink_to_fit();
    graph_rnd.comp_edges.clear();
    graph_rnd.comp_edges.shrink_to_fit();
    if (BIGRAPH) {
      graph_rnd.randomBiGraph(n);  
    } else {
      graph_rnd.randomGraph(n);  
    }
    VC = graph_rnd.vertexCover();
    graph_rnd.Complement();
    graph_rnd.computeAdjacency();
    graph_rnd.printAdjacency();
    graph_rnd.exportAdjacency("adjacency" + std::to_string(graph_rnd.nodes.size()) + ".adj");
    break;
  }
  case 'O': 
  case 'o': {
    std::cout << "Showing vertex selection order\n";
    ShowOrder = !ShowOrder;
    break;
  }
  case 'C': 
  case 'c': {
    ShowComp = !ShowComp;
    ShowEdges = !ShowEdges;
    break;
  }
  case 'A': 
  case 'a': {
    graph_rnd.computeAdjacency();
    graph_rnd.printAdjacency();
    break;
  }
  case 'V': 
  case 'v': {
    graph_rnd.computeAdjacency(); 
    VC = graph_rnd.vertexCoverAdjacency();
    break;
  }
  default: {
    break;
  }
  }
  glutPostRedisplay();
}

void mouseMotion(int x, int y) {
  if (mouseClickDown) {
  }
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (float)w / h, 0.01, 100);
  glLoadIdentity();
}
