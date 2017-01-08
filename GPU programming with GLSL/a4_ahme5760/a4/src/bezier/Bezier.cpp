#include "Bezier.hpp"
#include "../Camera.hpp"
#include "../Light.hpp"
#include "../Shape.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define Pi 3.141592653589793238462643
extern GLint winWidth;
extern GLint winHeight;

extern GLfloat red;
extern GLfloat blue;
extern GLfloat green;


Bezier::Bezier() {
	nCtrlPts = 0;
	R = 45;
	S = 5;
	T = 5;
	red = 1;
	green = 0;
	blue = 0;
	mode = MODE_WIRE;
}

void Bezier::reset() {
	nCtrlPts = 0;
	R = 45;
	S = 5;
	T = 5;
	red = 1;
	green = 0;
	blue = 0;
	mode = MODE_WIRE;
}

void Bezier::draw2DFrame() {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-winWidth / 4, 0.0);
	glVertex2f(winWidth / 4, 0.0);

	glColor3f(0.0, 1.0, 0.0);

	glVertex2f(0.0, -winHeight / 4);
	glVertex2f(0.0, winHeight / 4);
	glEnd();
}

void Bezier::setPixel(GLfloat x, GLfloat y) {
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}

/*  Compute the array of binomial coefficients C for given value of n,   */
void Bezier::binomialCoeffs(GLint n, GLint* C) {

// compute c(n, i) and put in C[i]
	GLint k, j;

	for(k=0; k<=n; k++){
		C[k]=1;
		for(j=n; j>= k + 1; j--){
			C[k] *= j;
		}
		for(j=n-k; j>=2; j--){
			C[k] /= j;
		}
	}
}

/*  Compute bezier point p(u) and p'(u)  */

void Bezier::computeBezPt(GLfloat u, Point* bezPt, Point* BezTan, GLint nCtrlPts, Point* ctrlPts, GLint* C)
{
    GLint k, n = nCtrlPts - 1;
    GLfloat bezBlendFcn, factor = 1.0;

    bezPt->x = bezPt->y = bezPt->z = 0.0;
    BezTan->x = BezTan->y = BezTan->z = 0.0;

    //  Compute blending functions and blend control points.
    for (k = 0; k < nCtrlPts; k++) {
        bezBlendFcn = C[k]*pow(u,k)*pow(1-u,n-k);

        bezPt->x += ctrlPts[k].x * bezBlendFcn;
        bezPt->y += ctrlPts[k].y * bezBlendFcn;
        bezPt->z += ctrlPts[k].z * bezBlendFcn;

// compute tangent

        if (u < 0.001) {  // u = 0

            BezTan->x = ctrlPts[1].x - ctrlPts[0].x;
            BezTan->y = ctrlPts[1].y - ctrlPts[0].y;
            BezTan->z = ctrlPts[1].z - ctrlPts[0].z;
        } else if (1 - u < 0.001) {  // u = 1

            BezTan->x = ctrlPts[n].x - ctrlPts[n-1].x;
            BezTan->y = ctrlPts[n].y - ctrlPts[n-1].y;
            BezTan->z = ctrlPts[n].z - ctrlPts[n-1].z;
        }
    }
}

//generate the mesh of rotation surface
void Bezier::RotateBezier(Point * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts) {
    Point bezCurvePt, bezCurveTangent;
    GLfloat u;
    GLint i, *C = new GLint[nCtrlPts];  /*Allocate space for binomial coefficients  */

    binomialCoeffs(nCtrlPts - 1, C);

    for (int j = 0; j <= T; j++) {

        u = GLfloat(j) / GLfloat(T);

        computeBezPt(u, &bezCurvePt, &bezCurveTangent, nCtrlPts, ctrlPts, C);

        //  generate mesh point and its normal of rotation surface
        for (i = 0; i <= (int) (R / S); i++) {
            // compute the rotation mesh point
            RBM[i][j].x = bezCurvePt.x;
            RBM[i][j].y = bezCurvePt.y * cos((Pi / 180) * S * i);
            RBM[i][j].z = bezCurvePt.y * sin((Pi / 180) * S * i);

            // compute the normal of mesh point
            RBM_norm[i][j].x  = - bezCurveTangent.y;;
            RBM_norm[i][j].y = bezCurveTangent.x * cos((Pi / 180) * S * i);
            RBM_norm[i][j].z = bezCurveTangent.x * sin((Pi / 180) * S * i);
        }

        if (R == 360) {
            RBM[i][j].x = RBM[0][j].x;
            RBM[i][j].y = RBM[0][j].y;
            RBM[i][j].z = RBM[0][j].z;

            RBM_norm[i][j].x  = RBM_norm[0][j].x;
            RBM_norm[i][j].y  = RBM_norm[0][j].y;
            RBM_norm[i][j].z  = RBM_norm[0][j].z;
        }
    }

    delete[] C;
}


void Bezier::drawRBC(){
    int i, j;

    glColor3f(red, green, blue);

    int rsteps = (int) (R/S);
    //int rsteps = (int) (R/S) -1;
    if (R == 360) rsteps++;

    if (mode == MODE_WIRE) {

        for (j = 0; j < T; j++) {
            for (i = 0; i < rsteps; i++) {
                glBegin(GL_LINE_LOOP);
                glVertex3f(RBM[i][j].x, RBM[i][j].y, RBM[i][j].z);
                glVertex3f(RBM[i + 1][j].x, RBM[i + 1][j].y, RBM[i + 1][j].z);
                glVertex3f(RBM[i + 1][j + 1].x, RBM[i + 1][j + 1].y, RBM[i + 1][j + 1].z);
                glVertex3f(RBM[i][j + 1].x, RBM[i][j + 1].y, RBM[i][j + 1].z);
                glEnd();
            }
        }

    }

    else if (mode == MODE_SOLID || mode == MODE_GLSL_SOLID ) {
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

        for (i = 0; i < rsteps; i++) {
            for (j = 0; j < T; j++) {
            	//glBegin(GL_QUADS);
                glBegin(GL_POLYGON);
                glNormal3f(RBM_norm[i][j].x, RBM_norm[i][j].y,RBM_norm[i][j].z);
                glVertex3f(RBM[i][j].x, RBM[i][j].y, RBM[i][j].z);

                glNormal3f(RBM_norm[i+1][j].x, RBM_norm[i+1][j].y,RBM_norm[i+1][j].z);
                glVertex3f(RBM[i + 1][j].x, RBM[i + 1][j].y, RBM[i + 1][j].z);

                glNormal3f(RBM_norm[i+1][j+1].x, RBM_norm[i+1][j+1].y,RBM_norm[i+1][j+1].z);
                glVertex3f(RBM[i + 1][j + 1].x, RBM[i + 1][j + 1].y, RBM[i + 1][j + 1].z);

                glNormal3f(RBM_norm[i][j+1].x, RBM_norm[i][j+1].y,RBM_norm[i][j+1].z);
                glVertex3f(RBM[i][j + 1].x, RBM[i][j + 1].y, RBM[i][j + 1].z);
                glEnd();
            }

        }
    }

}

void Bezier::drawCPts() {
	glPointSize(3.0);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < nCtrlPts; i++) {
		setPixel(ctrlPts[i].x, ctrlPts[i].y);
	}
}

void Bezier::displayCPts() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);
	glClearColor(0.0, 0.0, 0.0, 1.0); // Set display-window color to black
	draw2DFrame();
	drawCPts();
}

void Bezier::drawBezCurve(Point* ctrlPts, GLint nCtrlPts) {
	Point bezCurvePt, bezCurveTangent;
	GLfloat u;
	GLint *C = new GLint[nCtrlPts];  /*Allocate space for binomial coefficients*/

	binomialCoeffs(nCtrlPts - 1, C);

	glColor3f(red, green, blue);
	glLineWidth(3);

	glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= 20; k++) {  // approximate by 20 line segments
			u = GLfloat(k) / GLfloat(20);
			computeBezPt(u, &bezCurvePt, &bezCurveTangent, nCtrlPts, ctrlPts, C);
			glVertex3f(bezCurvePt.x, bezCurvePt.y, 0);
		}
	glEnd();

	delete[] C;
}

void Bezier::displayBezCurve() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);
	draw2DFrame();
	drawCPts();
	drawBezCurve(ctrlPts, nCtrlPts);
}

void Bezier::draw() {

	if (Ptgen) {
		displayCPts();
	}

	else if (BCgen) {
		displayBezCurve();
	}

	else if (BCRotation) {

		glPushMatrix();

		this->ctm_multiply();

		glScalef(s * 0.01, s * 0.01, s * 0.01);		// to shrink the object to size of viewable

		drawRBC();

		glPopMatrix();

	}
}

void Bezier::setRenderMode(RenderMode m)
{
  mode = m;
}
void Bezier::set_colour(GLfloat x, GLfloat y, GLfloat z){
}
