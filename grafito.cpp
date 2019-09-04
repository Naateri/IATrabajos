#define GLUT_DISABLE_ATEXIT_HACK
#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <list>
#include <stack>

#define KEY_ESC 27
#define DFS '1'
#define HILLCLIMB '2'
#define ASTAR '3'
#define RESET '4'

using namespace std;

int grid_x, grid_y, technique = 0;

struct Point2D{
	int x, y;
	vector<Point2D*> neigh;
	vector<Point2D*> path;
	bool activated = true, dfs = false;
	float distance = 0.0f;
	Point2D(){
		this->x = this->y = 0;
	}
	Point2D(int x, int y){
		this-> x = x;
		this->y = y;
	}
	
};

template<typename T>
void pop_front(std::vector<T> &v)
{
	if (v.size() > 0) {
		v.erase(v.begin());
	}
}

bool compare_distances(Point2D* A, Point2D* B){
	return A->distance < B->distance;
}

std::vector<Point2D*> points;
std::vector<Point2D*> start_end; //si el tamaño de este es dos, pedimos [0] como inicio
//y [1] como objetivo
std::vector<Point2D*> deleted_nodes; //nodos que ya no pertenecen al grafo
std::vector<Point2D*> path;

void resetGraph(){
	for(unsigned int i = 0; i<points.size();++i){
		points[i]->activated = true;
	}
	deleted_nodes.clear();
	start_end.clear();
	path.clear();
}

void reset_dfs(){
	for(int i = 0; i < points.size(); i++){
		if (points[i]->activated){
			points[i]->dfs = false;
		}
	}
}

void clear_paths(){
	for(int i = 0; i < points.size(); i++){
		if (points[i]->activated){
			points[i]->path.clear();
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
	if(path.size() > 1){
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
		glVertex2f((float)path[0]->x, (float)path[0]->y);
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
	//delete A;
	Point2D* temp;
	temp = points[index];
	points[index]->activated = false;
	for(int i = 0; i < deleted_nodes.size(); i++){
		if (deleted_nodes[i]->x == temp->x &&
			deleted_nodes[i]->y == temp->y){
			deleted_nodes.erase(deleted_nodes.begin() + i);
			points[index]->activated = true;
			return;
		}
	}
	deleted_nodes.push_back(temp);
}

/*void b_profundidad(Point2D* partida, Point2D* llegada){
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
}*/

stack<Point2D*> my_stack;

void dfs(Point2D* partida, Point2D* llegada){
	bool flag;
	Point2D* v;
	
	flag = 0;
	v = my_stack.top();
	v->path.push_back(v);
	
	if (v->x == llegada->x && v->y == llegada->y){
		path = v->path;
		return;
	}
	
	for(int i=0;i<v->neigh.size();i++){
		if(v->neigh[i]->activated==true && v->neigh[i]->dfs == false){
			my_stack.push(v->neigh[i]);
			v->neigh[i]->dfs = true;
			v->neigh[i]->path = v->path;
			flag=true;
		}
	}
	if (!flag){
		my_stack.pop();
	}
	dfs(partida, llegada);
	
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

void update_distances(Point2D* llegada){
	for(int i = 0; i < points.size(); i++){
		if (points[i]->activated){
			points[i]->distance = euclidean_distance(points[i], llegada);
		}
	}
}

list<Point2D*> stack_sim;

void hillclimbing(Point2D* partida, Point2D* llegada){
	
	/*if (my_stack.size() == 0){
		cout << "No path found.\n";
		return;
	}*/
	
	bool flag;
	Point2D* v;
	
	flag = 0;
	/*v = stack_sim.front();
	stack_sim.pop_front();*/
	v = my_stack.top();
	my_stack.pop();
	v->path.push_back(v);
	
	if (v->x == llegada->x && v->y == llegada->y){
		//path = cur_path;
		path = v->path;
		return;
	}
	
	list<Point2D*> childs;
	
	for(int i=0;i<v->neigh.size();i++){
		if(v->neigh[i]->activated == true && v->neigh[i]->dfs == false){
			childs.push_front(v->neigh[i]);
			v->neigh[i]->dfs = true;
			v->neigh[i]->path = v->path;
			flag=true;
		}
	}
	if (flag){
		childs.sort(compare_distances);
		childs.reverse();
		for(list<Point2D*>::iterator it = childs.begin(); it != childs.end();
			it++){
			//stack_sim.push_back(*it);
			my_stack.push(*it);
		}
	} else {
		//stack_sim.pop_front();
		my_stack.pop();
	}
	hillclimbing(partida, llegada);
}


void aStarAlgorithm(){
	path.clear();
	Point2D *start = start_end[0];
	Point2D *end = start_end[1];
	Point2D *q = nullptr;
	vector<Point2D *>::iterator it;
	vector<Point2D *>openList;
	vector<Point2D *> closedList;
	vector<Point2D *>p;
	openList.push_back(start);
	p.push_back(start);
	while(!openList.empty()){
		q = openList[0];
		pop_front(openList);
		for(unsigned int i = 0; i<q->neigh.size();++i){
			it = find(closedList.begin(),closedList.end(),q->neigh[i]);
			if(q->neigh[i]==end){
				p.push_back(end);
				path = p;
				return;
			}else if(q->neigh[i]->activated&&it==closedList.end()){
				q->neigh[i]->distance = euclidean_distance(q,q->neigh[i]) + euclidean_distance(q->neigh[i],end);
				it = find(openList.begin(),openList.end(),q->neigh[i]);
				if(it==openList.end()){
					openList.push_back(q->neigh[i]);
				}
			}
		}
		closedList.push_back(q);
		sort(openList.begin(),openList.end(), compare_distances);
		p.push_back(openList[0]);
	}
	path=p;
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
	cout<<"points "<<points.size()<<endl;
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
	
	//techniques
	if (start_end.size() == 2){
		if (technique == 1){
			while(! my_stack.empty()){ //emptying stack
				my_stack.pop();
			}
			my_stack.push(start_end[0]);
			start_end[0]->dfs = true;
			dfs(start_end[0], start_end[1]);
			reset_dfs();
		} else if (technique == 2){
			//hill(*start_end[0], *start_end[1]);
			stack_sim.clear();
			while(! my_stack.empty()){ //emptying stack
				my_stack.pop();
			}
			update_distances(start_end[1]);
			//stack_sim.push_front(start_end[0]);
			my_stack.push(start_end[0]);
			start_end[0]->dfs = true;
			hillclimbing(start_end[0], start_end[1]);
			reset_dfs();
		}else if(technique == 3){
			aStarAlgorithm();
		}else if(technique == 4){
			resetGraph();
			technique = 0;
		}
	}
	
	//imprimir el camino encontrado
	draw_path();
	
	glColor3d(255, 0, 0);
	for(int i = 0; i < start_end.size(); i++) //start_end points
		glVertex2f((float)start_end.at(i)->x,(float)start_end.at(i)->y);
	
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
	case ASTAR:
		technique = 3;
		break;
	case RESET:
		technique = 4;
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
