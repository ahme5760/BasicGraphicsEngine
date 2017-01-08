/*
 *	Name: 
 *	ID: 
 *	Class: CP411
 *	Assignment: 3
 */

#include "Planet.hpp"
#include <stdio.h>
using namespace std;
GLUquadricObj *spherer = NULL;
extern GLuint texture[9];
extern int planet_num;

Planet::Planet(){
	radius = 1.0;
	stacks = 20;
	splices = 20;
	r = 1.0;
	b = 1.0;
	g = 1.0;
}


/*
 * set_colour
 * a function to set the colour of the planet
 * Parameters:
 * 	red: the amount of red
 * 	green: the amount of green
 * 	blue: the amount of blue
 * Returns:
 * 	none
 */
void Planet::set_colour(GLfloat red, GLfloat green, GLfloat blue){
	r = red;
	g = green;
	b = blue;
}

/*
 * draw
 * the function that draws the planet
 * Parameters:
 * 	none
 * Returns:
 * 	none
 */
void Planet::draw(){
	glPushMatrix();
	this->ctm_multiply();
	glScalef(s, s, s);
	glColor3f(r, g, b);
	spherer = gluNewQuadric();

		glEnable(GL_TEXTURE_2D);
		gluQuadricDrawStyle(spherer, GLU_FILL);
		gluQuadricTexture(spherer, GL_TRUE);
		gluQuadricNormals(spherer, GLU_SMOOTH);
		glShadeModel(GL_FLAT);
		if(planet_num == 0){
			glBindTexture(GL_TEXTURE_2D, texture[6]);
		}else if (planet_num == 1){
			glBindTexture(GL_TEXTURE_2D, texture[7]);
		}else{
			glBindTexture(GL_TEXTURE_2D, texture[8]);
		}

		gluSphere(spherer, radius, splices ,stacks);


	glPopMatrix();
}
