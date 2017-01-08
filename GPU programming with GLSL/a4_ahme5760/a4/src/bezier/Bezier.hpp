#ifndef BEZIER_H
#define BEZIER_H

#include <GL/glut.h>
#include "../Shape.hpp"
#include "../Point.hpp"
#include "../Vector.hpp"
#include "../Light.hpp"


extern GLint style;
extern GLint winWidth, winHeight;
extern GLint Ptgen, BCgen, BCRotation;

class Bezier: public Shape {
public:
	enum RenderMode {
		MODE_WIRE = 0,
		MODE_SOLID,
		MODE_GLSL_SOLID,
		MODE_TEXTURE
	};

	RenderMode mode;
	GLfloat red, green, blue;
	GLint nCtrlPts;
	GLint R, S, T;
	Point ctrlPts[10];  		// to store control points,
	Point RBM[73][26];  		// mesh vertex
	Vector RBM_norm[73][26]; 	// vertex normal

	Bezier();

   ~Bezier() {};

	void draw2DFrame();

	void setPixel(GLfloat x, GLfloat y);

	void binomialCoeffs(GLint n, GLint * C);

	void computeBezPt(GLfloat u, Point * bezPt, Point * BezTan, GLint nCtrlPts, Point * ctrlPts, GLint * C);

	void RotateBezier(Point * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts);

	void drawRBC();

	void drawCPts();

	void displayCPts();

	void drawBezCurve(Point * ctrlPts, GLint nCtrlPts);

	void displayBezCurve();

	void draw();

	void setRenderMode(RenderMode m);

	void reset();

	virtual void set_colour(GLfloat x, GLfloat y, GLfloat z);

};

#endif
