#include <iostream>
#include <vector>
#include <GL/freeglut.h>

#include "conf.h"
#include "vector.h"
#include "nfg.h"
#include "tga.h"

using namespace std;

bool anim = false;
float xrot = 0.0f;
float yrot = 0.0f;

float scale = 1.0f;
float xmov = 0.0f;
float ymov = 0.0f;
float zmov = 0.0f;

void LoadTextures() {
	Model file = Model("Woman1.tga");

	glEnable(GL_TEXTURE_2D);

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, file.HasAlphaChannel() ? GL_RGBA : GL_RGB, file.GetWidth(), file.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, file.GetPixels().data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void transform() {
	glRotatef(xrot, 1.0f, 0.0f, 0.0f); // Putar horizontal
	glRotatef(yrot, 0.0f, 1.0f, 0.0f); // Putar vertical
	glScalef(scale, scale, scale); // Perskalaan
	glTranslatef(xmov, ymov, zmov); // Translasi
}

void display() {

	Nfg model;
	model.process();

	int NTriangle = model.totalNrIndices / 3;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();

	LoadTextures();

	gluLookAt(0.0, 2.0, 8.0, 0.0, 1.5, 5.0, 0, 1, 0);

	transform();


	for (int i = 0; i < NTriangle; i++) {
		

		glBegin(GL_POLYGON);
		glTexCoord2f(model.myVertex[model.myPattern[i].pos.x].uv.x, model.myVertex[model.myPattern[i].pos.x].uv.y);
		glVertex3f(model.myVertex[model.myPattern[i].pos.x].pos.x, model.myVertex[model.myPattern[i].pos.x].pos.y, model.myVertex[model.myPattern[i].pos.x].pos.z);
		glTexCoord2f(model.myVertex[model.myPattern[i].pos.y].uv.x, model.myVertex[model.myPattern[i].pos.y].uv.y);
		glVertex3f(model.myVertex[model.myPattern[i].pos.y].pos.x, model.myVertex[model.myPattern[i].pos.y].pos.y, model.myVertex[model.myPattern[i].pos.y].pos.z);
		glTexCoord2f(model.myVertex[model.myPattern[i].pos.z].uv.x, model.myVertex[model.myPattern[i].pos.z].uv.y);
		glVertex3f(model.myVertex[model.myPattern[i].pos.z].pos.x, model.myVertex[model.myPattern[i].pos.z].pos.y, model.myVertex[model.myPattern[i].pos.z].pos.z);
		glEnd();

		

	}

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w > h) {
		glViewport(0, 0, w, w);
		glTranslatef(0.0, -0.5, 0.0);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glScalef(-0.5, 0.5, -0.5);
	}
	else {
		glViewport(0, 0, w, h);
	}

	gluPerspective(20.0, w / h, 0.1f, 100.0f);

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myinit() {
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	case 'A':
		yrot += 5.0f;
		break;
	case 'd':
	case 'D':
		yrot -= 5.0f;
		break;
	case 'w':
	case 'W':
		xrot += 5.0f;
		break;
	case 's':
	case 'S':
		xrot -= 5.0f;
		break;
	case 'p':
	case 'P':
		if (anim) {
			anim = false;
			printf("Stop\n");
		}
		else {
			printf("Play\n");
			anim = true;
		}
		break;
	case 'B':
	case 'b':
		scale += 0.2f;
		break;
	case 'M':
	case 'm':
		scale -= 0.2f;
		break;
	case 27:
		exit(1);
		break;
	}

	glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		xmov -= 0.025f;
		break;
	case GLUT_KEY_UP:
		ymov += 0.025f;
		break;
	case GLUT_KEY_RIGHT:
		xmov += 0.025f;
		break;
	case GLUT_KEY_DOWN:
		ymov -= 0.025f;
		break;
	}

	glutPostRedisplay();
}

void animasi(int) {
	glutTimerFunc(1000 / 30, animasi, 0);

	if (anim)
		yrot -= 3.0f;
	else
		yrot = yrot;

	glutPostRedisplay();
}

int main(int argc, char** argv) {

	Config conf;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(conf.winWidth, conf.winHeight);
	glutInitWindowPosition(conf.winPosX, conf.winPosY);
	glutCreateWindow("TR GRAFKOM");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutTimerFunc(0, animasi, 0);

	cout << " Keterangan :" << endl << endl;
	cout << " >> W, A, S, D untuk rotasi" << endl;
	cout << " >> B, M untuk scaling" << endl;
	cout << " >> Panah untuk move camera" << endl;
	cout << " >> P untuk Play/Stop animasi" << endl;
	cout << " >> Esc untuk keluar"<< endl;
	myinit();
	glutMainLoop();

	return 0;
}
