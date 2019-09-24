#define GLUT_DISABLE_ATEXIT_HACK
#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <list>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <time.h>
using namespace std;

int numPoints;
int numOfGenerations;

GLfloat RandomFloat(GLfloat a, GLfloat b) {
	GLfloat random = ((GLfloat) rand()) / (GLfloat) RAND_MAX;
	GLfloat diff = b - a;
	GLfloat r = random * diff;
	return a + r;
}

class Point{
public:
	GLfloat x;
	GLfloat y;

	Point(GLfloat x, GLfloat y){
		this->x = x;
		this->y = y;
	}
};

class Line{
public:
	Point *p1;
	Point *p2;
	
	Line(Point *p1, Point *p2){
		this->p1 = p1;
		this->p2 = p2;
	}
	
};
vector< Point * > nodes;
vector< vector<GLfloat> > adjacentMatrix;
vector<Line *> edges;


bool findPoint(GLfloat x, GLfloat y){
	for(int i = 0; i<nodes.size(); ++i){
		if(nodes[i]->x == x && nodes[i]->y==y){
			return true;
		}
	}
	return false;
}
	
GLfloat euclideanDistance(Point* p1, Point* p2){
	return sqrt(pow(p1->x - p2->x,2)+pow(p1->y - p2->y,2));
}

void generateNodes(){
	GLfloat x = RandomFloat(-799,799);
	GLfloat y = RandomFloat(-799,799);
	for(int i = 0; i<numPoints; ++i){
		while(findPoint(x,y)){
			x = RandomFloat(-799,799);
			y = RandomFloat(-799,799);
		}
		//cout<<x<<" "<<y<<endl;
		nodes.push_back(new Point(x,y));
	}
}
	
void generateEdges(){
	for(int i = 0 ; i<nodes.size();++i){
		vector<GLfloat> t;
		for(int j = 0;j<nodes.size();++j){
			if(i==j)
				continue;
			t.push_back(euclideanDistance(nodes[i],nodes[j]));
			edges.push_back(new Line(nodes[i],nodes[j]));
		}
		adjacentMatrix.push_back(t);
	}
}
/**
Parametros:
	Numero de Hijos por generacion
	Numero de generacion
	Primera poblacion caminos aleatoreos
	Crossover se obtiene un subconjunto de uno de los padres y se completa con los nodos faltantes de la madre
	Metodo para la seleccion elitismo: obtener los dos mejores de cada generacion
	Mutacion swap de dos ciudades
	
	
	
**/
	
	
void geneticAlgorithmTSP(){
	
}

void drawPoints(){
	glPointSize(6);
	glColor3d(0, 0, 255);
	glBegin(GL_POINTS);
		for(int i = 0; i<nodes.size();++i){
			glVertex2f((float)nodes.at(i)->x,(float)nodes.at(i)->y);
		}
	glEnd();
	
}
	
void drawEdges(){
	glColor3d(255, 0, 0);
	for(int i = 0;i<edges.size();++i){
		glBegin(GL_LINES);
			glVertex2f(edges[i]->p1->x,edges[i]->p1->y);
			glVertex2f(edges[i]->p2->x,edges[i]->p2->y);
		glEnd();
	}
}

void glPaint(void) {
	
	//El fondo de la escena al color initial
	
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-800.0f,  800.0f,-800.0f, 800.0f, -1.0f, 1.0f);
	
	drawPoints();
	drawEdges();
	//doble buffer, mantener esta instruccion al fin de la funcion
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

void OnMouseClick(int button, int state, int x, int y){
	
}
	
GLvoid window_key(unsigned char key, int x, int y) {
	
	
}

void OnMouseMotion(int x, int y){
	
}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}
	
//
//el programa principal
//
int main(int argc, char** argv) {
	
	
	cout<<"Numero de puntos: "<<endl;
	cin>>numPoints;
	cout<<"Numero de generaciones: "<<endl;
	cin>>numOfGenerations;
	generateNodes();
	generateEdges();
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TSP"); //titulo de la ventana
	
	init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
