#define GLUT_DISABLE_ATEXIT_HACK
#include <iostream>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <list>

#define KEY_ESC 27
#define DFS '1'
#define HILLCLIMB '2'
#define ASTAR '3'

using namespace std;

int grid_x, grid_y, technique = 0;

struct Point2D{
	int x, y;
	vector<Point2D*> neigh;
	bool activated = true, dfs = false;
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
std::vector<Point2D*> path;

void reset_dfs(){
	for(int i = 0; i < points.size(); i++){
		if (points[i]->activated){
			points[i]->dfs = false;
		}
	}
}

//dibuja un simple gizmo
void displayGizmo(){
	glBegin(GL_LINES);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnd();
}
bool r = false;

void draw_point(int x, int y);

void draw_path(){
	if(path.size()!=0){
		for(int i = 0;i < path.size()-1;++i){
			glPointSize(8);
			glBegin(GL_POINTS);
			glColor3f(0,1.0,0);
			glVertex2f((float)path[i+1]->x,(float)path[i+1]->y);
			glVertex2f((float)path[i+1]->x,(float)path[i+1]->y);
			glEnd();
			glLineWidth(0.3f);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex2f((float)path[i]->x,(float)path[i]->y);
			glVertex2f((float)path[i+1]->x,(float)path[i+1]->y);
			glEnd();
		}
		glBegin(GL_POINTS);
		glColor3d(255, 0, 0);
		glVertex2f((float)path[path.size() - 1]->x, (float)path[path.size() - 1]->y);
		glEnd();
	}
}

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
void b_profundidad(Point2D* partida, Point2D* llegada){
	path.clear();
	reset_dfs();
	Point2D* actual= partida;
	Point2D* fin= llegada;
	Point2D* ante=actual;
	
	list<Point2D* > vacio;
	vacio.push_front(actual);
	path.push_back(actual);
	actual->dfs = true;
	
	while( actual->x != fin->x || actual->y != fin->y ){
		bool cambio=false;
		for(int i=0;i<actual->neigh.size();i++){
			if(actual->neigh[i]->activated==true && actual->neigh[i]->dfs == false){
				vacio.push_front(actual->neigh[i]);
				actual->neigh[i]->dfs = true;
				cambio=true;
			}
		}
		if(cambio==false){
			path.push_back(ante);
			vacio.pop_front();
		}
		ante = actual;
		actual = vacio.front();
		//vacio.pop_front();
		path.push_back(actual);
		//cout<<"sigue"<<actual->x<<" , "<<actual->y<<endl;
	}
	path.push_back(fin);
	
}
void hill(Point2D partida,Point2D llegada)
{
	path.clear();
	reset_dfs();
	Point2D* actual=&partida;
	Point2D* fin=&llegada;
	Point2D* ante=actual;
	
	list<Point2D*> vacio;
	vector<Point2D*> temp;
	vacio.push_front(actual);
	path.push_back(actual);
	
	
	while(actual->x!=fin->x || actual->y!=fin->y  ){
		
		bool cambio=false;
		for(int i=0;i<actual->neigh.size();i++){
			if(actual->neigh[i]->activated==true && actual->neigh[i]->dfs == false){
				temp.push_back(actual->neigh[i]);
				//cout<<actual->neigh[i]->x<<" , "<<actual->neigh[i]->y<<endl;
				actual->neigh[i]->dfs = true;
				cambio=true;
			}
		}
		
		while(temp.size()!=0){
			int i;
			int mayor=0;
			float a,b;
			a=euclidean_distance(actual, temp[0]);
			for(i=1;i<temp.size();i++){
				b=euclidean_distance(actual, temp[i]);
				if(b>a){
					mayor=i;
					a=b;
				}
			}
			vacio.push_front(temp[mayor]);
			temp.erase(temp.begin()+mayor);
		}
		if(cambio==false){
			path.push_back(ante);
			vacio.pop_front();  
		}
		ante=actual;
		actual=vacio.front();
		//vacio.pop_front();
		path.push_back(actual);
	}
	path.push_back(fin);
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
	int tempx = (grid_x - 5)/10;
	int tempy = (grid_y - 5)/10;
	cout<<tempx<<" "<<tempy<<endl;
	for(int i = 0;i<tempx;i++){
		for(int j = 0;j<tempy;j++){
			//cout<<"------------------------------------------------------"<<endl;
			//cout<<points[i*tempy+j]->x<<" "<<points[i*tempy+j]->y<<endl;
			//cout<<"------------------------------------------------------"<<endl;
			if(j-1>=0){
				points[i*tempy+j]->neigh.push_back(points[i*tempy+(j-1)]);
				//cout<<points[i*(j-1)]->x<<" "<<points[i*(j-1)]->y<<endl;
			}
			if(j+1<tempy){
				points[i*tempy+j]->neigh.push_back(points[i*tempy+(j+1)]);
				//cout<<points[i*(j+1)]->x<<" "<<points[i*(j+1)]->y<<endl;
			}
			if(i-1>=0){
				points[i*tempy+j]->neigh.push_back(points[j+(i-1)*tempy]);
				//cout<<points[j*(i-1)]->x<<" "<<points[j*(i-1)]->y<<endl;
			}
			if(i+1<tempx){
				points[i*tempy+j]->neigh.push_back(points[j+(i+1)*tempy]);
				//cout<<points[j*(i+1)]->x<<" "<<points[j*(i+1)]->y<<endl;
			}
			if(i+1<tempx&&j-1>=0){
				points[i*tempy+j]->neigh.push_back(points[(j-1)+(i+1)*tempy]);
				//cout<<points[(j-1)*(i+1)]->x<<" "<<points[(j-1)*(i+1)]->y<<endl;
			}
			if(i-1>=0&&j+1<tempy){
				points[i*tempy+j]->neigh.push_back(points[(j+1)+(i-1)*tempy]);
				//cout<<points[(j+1)*(i-1)]->x<<" "<<points[(j+1)*(i-1)]->y<<endl;
			}
			if(i-1>=0&&j-1>=0){
				points[i*tempy+j]->neigh.push_back(points[(j-1)+(i-1)*tempy]);
				//cout<<points[(j-1)*(i-1)]->x<<" "<<points[(j-1)*(i-1)]->y<<endl;
			}
			if(i+1<tempx&&j+1<tempy){
				points[i*tempy+j]->neigh.push_back(points[(j+1)+(i+1)*tempy]);
				//cout<<points[(j+1)*(i+1)]->x<<" "<<points[(j+1)*(i+1)]->y<<endl;
			}
			//cout<<"------------------------------------------------------"<<endl;
		}
	}
	cout<<"points"<<points.size()<<endl;
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
	glPointSize(6);
	glBegin(GL_POINTS);
	glColor3d(0, 0, 255);
	for(int i = 0; i < points.size(); i++) //all points
		glVertex2f((float)points.at(i)->x,(float)points.at(i)->y);
	glColor3d(255, 0, 0);
	for(int i = 0; i < start_end.size(); i++) //start_end points
		glVertex2f((float)start_end.at(i)->x,(float)start_end.at(i)->y);
	glColor3d(0, 0, 0);
	for(int i = 0; i < deleted_nodes.size(); i++){ //deleted nodes
		glVertex2f((float)deleted_nodes.at(i)->x, (float)deleted_nodes.at(i)->y);
	}
	glEnd();
	for(int i = 0; i<points.size();++i){ //neighbours
		if(points[i]->activated){
			for(int j = 0;j<points[i]->neigh.size();++j){
				if(points[i]->activated && points[i]->neigh[j]->activated){
					glLineWidth(0.1f);
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_LINES);
					glVertex2f((float)points[i]->x,(float)points[i]->y);
					glVertex2f((float)points[i]->neigh[j]->x,(float)points[i]->neigh[j]->y);
					glEnd();
				}
			}
		}
	}
	//imprimir el camino encontrado
	draw_path();
	
	if (start_end.size() == 2){
		if (technique == 1){
			b_profundidad(start_end[0], start_end[1]);
		} else if (technique == 2){
			hill(*start_end[0], *start_end[1]);
		}
	}
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

//en el caso que la ventana cambie de tamaÃ±o
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
	case DFS:
		//b_profundidad(start_end[0], start_end[1]);
		technique = 1;
		break;
	case HILLCLIMB:
		//hill(*start_end[0], *start_end[1]);
		technique = 2;
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
	
	/*hill(*points[0],*points[10]);
	for(int i=0;i<path.size();i++){
	cout<<path[i]->x<<" , "<<path[i]->y<<endl;
	}*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(grid_x, grid_y); //tamaÃ±o de la ventana
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
	
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
