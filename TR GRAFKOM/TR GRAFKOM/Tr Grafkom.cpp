#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <Windows.h>

void init(void);
void disp(void);
void kb(unsigned char, int, int);
void kb2(int, int, int);
void mouse(int button, int state, int x, int y);
void size(int, int);
void germou(int x, int y);

float xp = 0.0f;
float yp = 0.0f;
float xd = 0.0f;
float yd = 0.0f;
bool mousedo = false;
int is_depth;

void init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	is_depth = 1;
	glMatrixMode(GL_MODELVIEW);
	glPointSize(8.0f);
	glLineWidth(5.0f);
}
void disp() {
	if (is_depth)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
	if (mousedo) {
		glLoadIdentity();
		glRotatef(xp, 1.0f, 0.0f, 0.0);
		glRotatef(yp, 0.0f, 1.0f, 0.0f);
	}
	
	
}
void kb(unsigned char k, int x, int y) {
	switch (k)
	{
	case '7':
		glTranslatef(0.0f, 3.0, 0.0);
		break;
	case '9':
		glTranslatef(0.0f, -3.0f, 0.0f);
		break;
	case '2':
		glRotatef(2.0, 1.0, 0.0, 0.0);
		break;
	case '8':
		glRotatef(-2.0, 1.0, 0.0, 0.0);
		break;
	case '6':
		glRotatef(2.0, 0.0, 1.0, 0.0);
		break;
	case '4':
		glRotatef(-2.0, 0.0, 1.0, 0.0);
		break;
	case '1':
		glRotatef(2.0, 0.0, 0.0, 1.0);
		break;
	case '3':
		glRotatef(-2.0, 0.0, 0.0, 1.0);
		break;
	case 27:
		exit(0);
		break;
	}
	disp();
}
void kb2(int k2, int x2, int y2) {
	if (k2 == GLUT_KEY_LEFT)
	{
		glTranslatef(-3.0, 0.0, 0.0);
	}
	else if (k2 == GLUT_KEY_UP)
	{
		glTranslatef(0.0, 0.0, 3.0);
	}

	else if (k2 == GLUT_KEY_RIGHT)
	{
		glTranslatef(3.0, 0.0, 0.0);
	}
	if (k2 == GLUT_KEY_DOWN)
	{
		glTranslatef(0.0, 0.0, -3.0);
	}
	disp();
}
void idle() {
	if (!mousedo) {
		xp += 0.3f;
		yp += 0.4f;
	}
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{


		mousedo = true;
		xd = x - yp;
		yd = xp - y;
		disp();
	}
	else
		mousedo = false;
	disp();
}
void germou(int x, int y) {
	if (mousedo) {
		yp = x - xd;
		xp = y + yd;
		glutPostRedisplay();
	}
}
void size(int w, int h) {
	if (h == 0)
		h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, w / h, 5.0, 500.0);
	glTranslatef(0.0, -5.0, -150.0);
	glMatrixMode(GL_MODELVIEW);
}
int main(int *argc, char** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE || GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TR GRAFKOM");
	glutDisplayFunc(disp);

	glutKeyboardFunc(kb);
	glutSpecialFunc(kb2);

	glutReshapeFunc(size);
	glutMouseFunc(mouse);
	glutMotionFunc(germou);
	glutMainLoop();
	return 0;
}
