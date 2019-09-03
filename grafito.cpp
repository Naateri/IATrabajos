#define GLUT_DISABLE_ATEXIT_HACK
#include <iostream>
#include <math.h>
#include <vector>
#include<GL/glut.h>

#define KEY_ESC 27
#define DFS 1
#define HILLCLIMB 2
#define ASTAR

using namespace std;

int grid_x, grid_y;

struct Point2D{
	int x, y;
	bool activated = true;
	Point2D(){
		this->x = this->y = 0;
	}
	Point2D(int x, int y){
		this-> x = x;
		this->y = y;
	}
};

std::vector<Point2D*> points;
std::vector<Point2D*> start_end; //si el tamaño de este es dos, pedimos [0] como inicio
//y [1] como objetivo
std::vector<Point2D*> deleted_nodes; //nodos que ya no pertenecen al grafo

//dibuja un simple gizmo
void displayGizmo(){
	glBegin(GL_LINES);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnd();
}
bool r = false;

void draw_point(int x, int y);

float euclidean_distance(Point2D* A, Point2D* B){
	return sqrt( pow((A->x - B->x), 2) + pow((A->y - B->y), 2) );
}

void find_closest(Point2D* A){
	if (start_end.size() >= 2) start_end.clear();
	float dist = euclidean_distance(A, points[0]), cur_dist;
	int index = 0;
	for(int i = 1; i < points.size(); i++){
		cur_dist = euclidean_distance(A, points[i]);
		if (cur_dist < dist){
			dist = cur_dist;
			index = i;
		}
	}
	start_end.push_back(points[index]);
}

void delete_node(Point2D* A){
	float dist = euclidean_distance(A, points[0]), cur_dist;
	int index = 0;
	for(int i = 1; i < points.size(); i++){
		cur_dist = euclidean_distance(A, points[i]);
		if (cur_dist < dist){
			dist = cur_dist;
			index = i;
		}
	}
	delete A;
	A = points[index];
	points[index]->activated = false;
	for(int i = 0; i < deleted_nodes.size(); i++){
		if (deleted_nodes[i]->x == A->x &&
			deleted_nodes[i]->y == A->y){
			deleted_nodes.erase(deleted_nodes.begin() + i);
			points[index]->activated = true;
			return;
		}
	}
	deleted_nodes.push_back(A);
}

void generate_points(){
	int min_x = -grid_x/2.0f, max_x = grid_x/2.0f;
	int min_y = -grid_y/2.0f, max_y = grid_y/2.0f;
	Point2D* pt;
	for(int i = min_x + 5.0f; i < max_x - 5.0f; i += 10){
		for (int j = min_y + 5.0f; j < max_y - 5.0f; j += 10){
			pt = new Point2D(i, j);
			points.push_back(pt);
		}
	}
}

void OnMouseClick(int button, int state, int x, int y){
	Point2D* pt;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		//convertir x,y
		pt = new Point2D(x-(grid_x/2.0f),(grid_y/2.0f) - y);
		find_closest(pt);
		delete pt;
	}else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		r = true;
		//pt = new Point2D(x-(grid_x/2.0f),(grid_y/2.0f) - y);
		//delete_node(pt);
	}else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		r = false;
	}
}

void OnMouseMotion(int x, int y){
	Point2D* pt;
	if(r){
		pt = new Point2D(x-(grid_x/2.0f),(grid_y/2.0f) - y);
		delete_node(pt);
	}
	//opcional
}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {
	
	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-(float(grid_x)/2.0f), (float(grid_x)/2.0f), (-float(grid_y)/2.0f), (float(grid_y)/2.0f), -1.0f, 1.0f);
	
	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3d(0, 0, 255);
	for(int i = 0; i < points.size(); i++)
		glVertex2f((float)points.at(i)->x,(float)points.at(i)->y);
	glColor3d(255, 0, 0);
	for(int i = 0; i < start_end.size(); i++)
		glVertex2f((float)start_end.at(i)->x,(float)start_end.at(i)->y);
	glColor3d(0, 0, 0);
	for(int i = 0; i < deleted_nodes.size(); i++){
		glVertex2f((float)deleted_nodes.at(i)->x, (float)deleted_nodes.at(i)->y);
	}
	glEnd();
	//dibuja el gizmo
	displayGizmo();
	
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

void draw_point(int x, int y){
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	displayGizmo();
	
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0, 0, 0, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
		
	default:
		break;
	}
	
}
//
//el programa principal
//
int main(int argc, char** argv) {
	
	//Inicializacion de la GLUT
	
	cout << "tamaño grilla\n";
	cin >> grid_x >> grid_y;
		
	generate_points();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(grid_x, grid_y); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Grafito"); //titulo de la ventana
	
	init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	
	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
