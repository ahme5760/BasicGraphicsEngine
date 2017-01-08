#ifndef HHOUSE_H
#define HHOUSE_H

#include <GL/glut.h>
#include "Shape.hpp"

class House: public Shape {
protected:
	GLfloat vertex[13][4];
	GLint face[11][4];

public:
	House();
	void draw_face(int);
	void draw();
};

#endif
