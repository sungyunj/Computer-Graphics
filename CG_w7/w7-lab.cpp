#include "gl/glut.h"
#include <cstdio>
#include <ctime>
#include <cmath>

#define PI 3.14159265358979323846

bool isAnimation = false;
double mousePt[2] = {1, 0};
int arrowNum[2] = { 0,0 };
int curveRES = 1000;
int width = 500, height = 500;
int timeIdx = 0;

void DrawCircle(double radius, bool isFill = true)
{
    glPointSize(15.0);
    if (isFill)
	glBegin(GL_POLYGON);
    else
	glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++)
	glVertex2d(radius * cos(i * PI / 180.0),
		   radius * sin(i * PI / 180.0));
    glEnd();
}

void DrawAxis()
{
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(10.0, 0.0, 0.0);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glColor3f(0.0, 10.0, 0.0);
    glVertex2i(0, 0);
    glVertex2i(0, 1);
    glColor3f(0.0, 0.0, 10.0);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 1);
    glEnd();
}

void display() {
    printf("time: %d\n", timeIdx);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 20+arrowNum[1], 0, 0, 0, 0, 1, 0);
    DrawAxis();
    // ------------------ Matrix Stack ------------------------
    //Sun
    glPushMatrix();
    glColor3f(1, 0, 0);
    DrawCircle(3.0);

    //Earth
    glPushMatrix();
    glRotated(timeIdx / 365.0 * 360.0, 0, 0, 1);
    glTranslated(7, 0.0, 0.0);
    glColor3f(0, 0, 1);
    DrawCircle(1.0);

    //Moon	
    glPushMatrix();
    glRotated(timeIdx / 27.0 * 360.0, 0, 0, 1);
    glTranslated(1.5, 0.0, 0.0);
    glColor3f(1, 0, 1);
    DrawCircle(0.3);

    glPopMatrix(); // for Moon
    glPopMatrix(); // for Earth
    glPopMatrix(); // for Sun

    //Mars

    //Jupiter

    //Saturn

    //Titan


    // ------------------ Matrix Stack (End) ------------------

    // ------------------ Look around the model ----------------
    //gluLookAt(10 * sin(timeIdx * PI / 180.0),	//eye x
	   //   0,				//eye y
	   //   10 * cos(timeIdx * PI / 180.0),	//eye z
	   //   0,				//center x
	   //   0,				//center y
	   //   0,				//center z
	   //   0,				//up x
	   //   1,				//up y
	   //   0);				//up z
    //glLineWidth(1.0);
    //glColor3f(0, 0, 0);
    //glutWireTeapot(2.0);
    //glColor3f(1, 0, 0);
    // ------------------ Look around the model (End)-----------



    glFinish();
    glutSwapBuffers();
}



void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
	timeIdx++;
	break;
    case GLUT_KEY_DOWN:
	timeIdx--;
	break;
    case GLUT_KEY_RIGHT:
	arrowNum[1]++;
	break;
    case GLUT_KEY_LEFT:
	arrowNum[1]--;
	break;
    }
    glutPostRedisplay();
}

void mouseCoordinateTranslate(int winX, int winY)
{
    mousePt[0] = double(winX) / width * 10 - 5;
    mousePt[1] = (double(winY) / height * 10 - 5) * (-1.0);
}

void mouse(int button, int state, int x, int y)
{
    printf("mouse: %d %d %d %d\n", button, state, x, y);
    if (state == GLUT_DOWN) {
	mouseCoordinateTranslate(x, y);
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    printf("mouse motion: %d %d\n", x, y);
    mouseCoordinateTranslate(x, y);
    glutPostRedisplay();
}

void mousePassiveMotion(int x, int y)
{
    printf("mouse passive motion: %d %d\n", x, y);
    mouseCoordinateTranslate(x, y);
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    if (h == 0)
	h = 1;

    width = w;
    height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
}

void timer(int value)
{
    if (isAnimation) {
	glutTimerFunc(10, timer, 0);
	timeIdx += 1;
	glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p': case 'P':
	isAnimation = !isAnimation;
	glutTimerFunc(10, timer, 0);
	break;
    case 27: // ESC
	exit(0);
	break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(1480, 100);

    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    //glutPassiveMotionFunc(mousePassiveMotion);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}