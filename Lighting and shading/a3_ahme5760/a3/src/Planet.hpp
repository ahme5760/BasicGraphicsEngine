/*
 *	Name: 
 *	ID: 
 *	Class: CP411
 *	Assignment: 3
 */

#ifndef CPlanet_H
#define CPlanet_H

#include <GL/glut.h>
#include "Shape.hpp"

class Planet: public Shape{
	protected:
		GLfloat radius, r ,g, b; //radius, red, green, blue
		GLint splices, stacks;
	public:
		Planet();
		void draw();
		void set_colour(GLfloat, GLfloat, GLfloat);

};
#endif
