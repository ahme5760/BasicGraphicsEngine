#ifndef PPYRAMID_H
#define PPYRAMID_H

#include <GL/glut.h>
#include "Shape.hpp"

class pyramid: public Shape {
protected:
	GLfloat vertex[5][4];
	GLint face[5][4];

public:
	pyramid();
	void draw_face(int);
	void draw();
};

#endif
