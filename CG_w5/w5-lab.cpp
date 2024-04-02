#include "gl/glut.h"
#include <cstdio>
#include <ctime>
#include <queue>

#include "GCAABBBVH.h"

bool isOrtho = true;
Vec2 mousePt = { 1, 0 };
int arrowNum[2] = { 0,0 };
int curveRES = 1000;
int width = 500, height = 500;

std::vector<Vec2> ctrlPts0, ctrlPts1;

BezierCurve2D *curve0 = nullptr;
BVH *bvh0 = nullptr;
BezierCurve2D *curve1 = nullptr;
BVH *bvh1 = nullptr;

void InitCurve()
{
    if (curve0 != nullptr) {
	delete curve0;
	curve0 = nullptr;
    }

    if (bvh0 != nullptr) {
	delete bvh0;
	bvh0 = nullptr;
    }

    if (curve1 != nullptr) {
	delete curve1;
	curve1 = nullptr;
    }

    if (bvh1 != nullptr) {
	delete bvh1;
	bvh1 = nullptr;
    }

    curve0 = new BezierCurve2D(ctrlPts0);
    curve1 = new BezierCurve2D(ctrlPts1);

    std::vector<Vec2> curvePts0, curvePts1;

    for (int i = 0; i <= curveRES; i++)
	curvePts0.push_back(curve0->Evaluate(i / double(curveRES)));

    for (int i = 0; i <= curveRES; i++)
	curvePts1.push_back(curve1->Evaluate(i / double(curveRES)));

    bvh0 = new BVH(curvePts0);
    bvh1 = new BVH(curvePts1);
}

void DrawAABB(Vec2 min, Vec2 max)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(min.x, min.y);
    glVertex2f(max.x, min.y);
    glVertex2f(max.x, max.y);
    glVertex2f(min.x, max.y);
    glEnd();
}

void DrawBV(int level, BVH *bvh)
{
    std::queue<BV *> q;

    for (auto &bv : bvh->roots)
	q.push(&bv);

    while (!q.empty()) {
	auto &bv = q.front();
	q.pop();

	if (bv->level == level || level < 0)
	    DrawAABB(bv->min, bv->max);

	if (bv->left_ != nullptr)
	    q.push(bv->left_);

    	if (bv->right_ != nullptr)
	    q.push(bv->right_);
    }
}

void DrawBezierCurves(BezierCurve2D *curve)
{
    // Draw control points
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (auto &pt : curve->controlPt)
	glVertex2f(pt.x, pt.y);
    glEnd();

    //Draw control line
    glBegin(GL_LINE_STRIP);
    for (auto &pt : curve->controlPt)
	glVertex2f(pt.x, pt.y);
    glEnd();

    //Draw Bezier curves
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= curveRES; i++) {
	auto pt = curve->Evaluate(i / float(curveRES));
	glVertex2f(pt.x, pt.y);
    }
    glEnd();
}

void DrawBox(Vec2 min0, Vec2 max0)
{
    glPointSize(15.0);
    glBegin(GL_POINTS);
    glVertex2d((min0.x + max0.x) / 2.0, (min0.y + max0.y) / 2.0);
    glEnd();
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0, 5.0, -5.0, 5.0);

    printf("curveRes : %d, arrowNum[0] : %d, arrowNum[1] : %d\n", curveRES,
	   arrowNum[0], arrowNum[1]);

    //Draw Bezier curves and their AABBs
    glColor3f(1, 0, 0);
    DrawBezierCurves(curve0);
    DrawBV(arrowNum[0], bvh0);

    glColor3f(0, 0, 1);
    DrawBezierCurves(curve1);
    DrawBV(arrowNum[0], bvh1);

    //Draw intersection points between two curves using the BVH.
    /* TODO : Need to implementation...*/

    glFinish();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
	exit(0);
	break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
	arrowNum[0]++;
	break;
    case GLUT_KEY_DOWN:
	arrowNum[0]--;
	break;
    case GLUT_KEY_RIGHT:
	curveRES += 1000;
	break;
    case GLUT_KEY_LEFT:
	if (curveRES > 1000)
	    curveRES -= 1000;
	break;
    }
    glutPostRedisplay();
}

void mouseCoordinateTranslate(int winX, int winY)
{
    mousePt.x = double(winX) / width * 10 - 5;
    mousePt.y = (double(winY) / height * 10 - 5) * (-1.0);
}

void changeCtrlPts()
{
    for (auto &pt : ctrlPts0) {
	if (sqrt(pow(pt.x - mousePt.x, 2) + pow(pt.y - mousePt.y, 2)) < 0.5) {
	    pt = mousePt;
	    InitCurve();
	    return;
	}
    }

    for (auto &pt : ctrlPts1) {
	if (sqrt(pow(pt.x - mousePt.x, 2) + pow(pt.y - mousePt.y, 2)) < 0.5) {
	    pt = mousePt;
	    InitCurve();
	    return;
	}
    }
}

void mouse(int button, int state, int x, int y)
{
    printf("mouse: %d %d %d %d\n", button, state, x, y);
    if (state == GLUT_DOWN) {
	mouseCoordinateTranslate(x, y);
	changeCtrlPts();
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    printf("mouse motion: %d %d\n", x, y);
    mouseCoordinateTranslate(x, y);
    changeCtrlPts();

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

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(1480, 100);


    ctrlPts0.push_back({ -3, -1 });
    ctrlPts0.push_back({ -1, 2 });
    ctrlPts0.push_back({ 1, 2 });
    ctrlPts0.push_back({ 3, -1 });
    curve0 = new BezierCurve2D(ctrlPts0);

    ctrlPts1.push_back({ -2, 1 });
    ctrlPts1.push_back({ 0, -4 });
    ctrlPts1.push_back({ 1, -4 });
    ctrlPts1.push_back({ 3, 1 });
    curve1 = new BezierCurve2D(ctrlPts1);

    InitCurve();

    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    //glutPassiveMotionFunc(mousePassiveMotion);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}