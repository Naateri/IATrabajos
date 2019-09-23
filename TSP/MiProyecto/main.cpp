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
vector<Point*> nodes;
vector<vector<GLfloat>> adjacentMatrix;


bool findPoint(GLfloat x, GLfloat y){
	for(int i = 0; i<nodes.size(); ++i){
		if(nodes[i]->x == x && nodes[i]->y==y){
			return true;
		}
	}
	return false;
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
	
void drawPoints(){
	glPointSize(6);
	glColor3d(0, 0, 255);
	glBegin(GL_POINTS);
		for(int i = 0; i<nodes.size();++i){
			glVertex2f((float)nodes.at(i)->x,(float)nodes.at(i)->y);
		}
	glEnd();
	
}

void glPaint(void) {
	
	//El fondo de la escena al color initial
	
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-800.0f,  800.0f,-800.0f, 800.0f, -1.0f, 1.0f);
	
	drawPoints();
	glColor3d(0, 0, 255);
	glBegin(GL_POINTS);
		glVertex2f(nodes[0]->x,nodes[0]->y);
	glEnd();
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
