#define GLUT_DISABLE_ATEXIT_HACK
#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <algorithm> 
#include <vector>
#include <GL/glut.h>
#include <list>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include<windows.h>
using namespace std;

int numPoints;
int numOfGenerations;
int numOfSons;
int numOfMutation;

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
vector< vector<Point *> >historicPopulation;
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
			//edges.push_back(new Line(nodes[i],nodes[j]));
		}
		adjacentMatrix.push_back(t);
	}
}
/**
Parametros:
	Numero de Hijos por generacion
	Numero de generaciones
	Numero de mutaciones
	Numero de nodos
Procedimiento:
	Primera poblacion caminos aleatoreos
	Crossover se obtiene un subconjunto de uno de los padres y se completan los nodos faltantes con la madre
	Metodo para la seleccion es elitismo: obtener los dos mejores de cada generacion
	Mutacion swap de dos nodos
**/
float getDistanceBetweenTwoNodes(Point *p1, Point *p2){
	int indexp1;
	int indexp2;
	for(int i = 0;i<nodes.size();++i){
		if(nodes[i]->x==p1->x && nodes[i]->y == p1->y){
			indexp1 = i;
		}else if(nodes[i]->x==p2->x && nodes[i]->y == p2->y){
			indexp2 = i;
		}
	}
	
	return adjacentMatrix[indexp1][indexp2];
	
}
	
float fitnessFunction(vector<Point*>path){
	float totalCost=0;
	for(int i = 0;i<path.size()-1;++i){
		totalCost+= getDistanceBetweenTwoNodes(path[i],path[i+1]);
	}
	return totalCost;
}


vector<vector<Point*>> getInitialPopulation(){
	vector<Point*> nodesTemp = nodes;
	vector< vector<Point*> > res;
	Point *temp;
	unsigned int pos1,pos2,j=0;
	for(int i = 0;i<numOfSons;++i){
		j = 0;
		while(j<10){
			pos1 = (rand()%nodesTemp.size());
			pos2 = (rand()%nodesTemp.size());
			temp = nodesTemp[pos1];
			nodesTemp[pos1] = nodesTemp[pos2];
			nodesTemp[pos2] = temp;
			++j;
		}
		res.push_back(nodesTemp);
	}
	cout<<res.size()<<endl;
	return res;
}
bool findElement(vector<Point *> path,Point * element){
	for(int i = 0; i<path.size() ; i++){
		if(path[i]->x == element->x && path[i]->y == element->y){
			//cout<<"Si esta"<<endl;
			return true;
		}
	}
	return false;
}
vector<vector<Point * >> doCrossOver(vector<Point * >father, vector<Point *> mother){
	int bottomLimit, topLimit;
	vector<vector<Point * >>res;
	vector<Point *> temp;
	vector<Point * >::iterator it;
	for(int i =0;i<numOfSons;++i){
		bottomLimit = rand()%father.size();
		topLimit = rand()%father.size();
		if(bottomLimit>topLimit){
			int t = bottomLimit;
			bottomLimit = topLimit;
			topLimit = t;
		}
		//cout<<"Bot"<<bottomLimit<<"Top"<<topLimit<<endl;
		
		for(int j = bottomLimit; j<topLimit;++j){
			temp.push_back(father[j]);
		}
		//cout<<"father Elements"<<temp.size()<<endl;
		for(int j = 0; j<mother.size();++j){
			//it = find(temp.begin(),temp.end(),mother[j]);
			if(!findElement(temp,mother[j])){
				//cout<<"mother"<<endl;
				temp.push_back(mother[j]);
			}
		}
		res.push_back(temp);
		temp.clear();
	}
	//cout<<"Nueva Poblacion"<<endl;
	/*for(int l = 0 ; l<res.size();++l){
		for(int j = 0; j<res[l].size();++j){
			cout<<res[l][j]->x<<" "<<res[l][j]->y<<" = ";
		}
		cout<<endl;
	}*/
	return res;
}

	
bool compareFuction(vector<Point *> path1, vector<Point *>path2){
	//cout<<"Llego"<<endl;
	return fitnessFunction(path1)<fitnessFunction(path2);
}
vector<vector<Point *>> getFatherMother(vector<vector<Point *>> population){
	vector<vector<Point *>>res;
	sort(population.begin(),population.end(),compareFuction);
	
	res.push_back(population[0]);
	res.push_back(population[1]);
	return res;
}
	

void makeMutation(vector<vector<Point *>> &path){
	int pos,swap1,swap2;
	Point * temp;
	vector<Point *>tempPath;
	for(int i = 0; i<numOfMutation;++i){
		pos = rand()%path.size();
		tempPath = path[pos];
		swap1 = rand()%tempPath.size();
		swap2 = rand()%tempPath.size();
		temp = path[pos][swap1];
		path[pos][swap1] = path[pos][swap2];
		path[pos][swap2] = temp;
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
	
void drawEdges(){
	glColor3d(255, 0, 0);
	for(int i = 0;i<edges.size();++i){
		glBegin(GL_LINES);
			glVertex2f(edges[i]->p1->x,edges[i]->p1->y);
			glVertex2f(edges[i]->p2->x,edges[i]->p2->y);
		glEnd();
	}
}

void drawPath(vector<Point *> path){
	nodes.clear();
	edges.clear();
	for(int i = 0 ;i<path.size()-1;++i){
		nodes.push_back(path[i]);
		edges.push_back(new Line(path[i],path[i+1]));
	}
	nodes.push_back(path[path.size()-1]);
	edges.push_back(new Line(path[path.size()-1],path[0]));
}
	
	
void geneticAlgorithmTSP(){
	vector<vector<Point *>> population = getInitialPopulation();
	int i = 0;
	historicPopulation.push_back(population[0]);
	//cout<<"Generacion 0"<<endl;
	/*for(int l = 0 ; l<population.size();++l){
		for(int j = 0; j<population[l].size();++j){
			cout<<population[l][j]->x<<" "<<population[l][j]->y<<" = ";
		}
		cout<<endl;
	}*/
	while(i<numOfGenerations){
		makeMutation(population);
		//cout<<"Mutaciones:"<<endl;
		/*for(int l = 0 ; l<population.size();++l){
			for(int j = 0; j<population[l].size();++j){
				cout<<population[l][j]->x<<" "<<population[l][j]->y<<" = ";
			}
			cout<<endl;
		}*/
		population= getFatherMother(population);
		
		population = doCrossOver(population[0],population[1]);
		historicPopulation.push_back(population[0]);
		++i;
		//cout<<"Generacion "<<i<<endl;
		
	}
	for(int i = 0 ;i<historicPopulation[historicPopulation.size()-1].size()-1;++i){
		cout<<historicPopulation[historicPopulation.size()-1][i]->x<<"-"<<historicPopulation[historicPopulation.size()-1][i]->y<<" - Distancia: "<<getDistanceBetweenTwoNodes(historicPopulation[historicPopulation.size()-1][i],historicPopulation[historicPopulation.size()-1][i+1])<<endl;
		nodes.push_back(historicPopulation[historicPopulation.size()-1][i]);
		edges.push_back(new Line(historicPopulation[historicPopulation.size()-1][i],historicPopulation[historicPopulation.size()-1][i+1]));
	}
	cout<<historicPopulation[historicPopulation.size()-1][historicPopulation[historicPopulation.size()-1].size()-1]->x<<"-"<<historicPopulation[historicPopulation.size()-1][historicPopulation[historicPopulation.size()-1].size()-1]->y<<endl;
	nodes.push_back(historicPopulation[historicPopulation.size()-1][historicPopulation[historicPopulation.size()-1].size()-1]);
	edges.push_back(new Line(historicPopulation[historicPopulation.size()-1][historicPopulation[historicPopulation.size()-1].size()-1],historicPopulation[historicPopulation.size()-1][0]));
	
	
}
	
void glPaint(void) {
	
	//El fondo de la escena al color initial
	
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-800.0f,  800.0f,-800.0f, 800.0f, -1.0f, 1.0f);
	
	drawPoints();
	drawEdges();
	//drawPath(historicPopulation[historicPopulation.size()-1]);
		//Sleep(1);
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
	
	srand (time(NULL));
	cout<<"Numero de puntos: "<<endl;
	cin>>numPoints;
	cout<<"Numero de generaciones: "<<endl;
	cin>>numOfGenerations;
	cout<<"Numero de poblacion: "<<endl;
	cin>>numOfSons;
	cout<<"Numero de Mutaciones: "<<endl;
	cin>>numOfMutation;
	generateNodes();
	generateEdges();
	geneticAlgorithmTSP();
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
