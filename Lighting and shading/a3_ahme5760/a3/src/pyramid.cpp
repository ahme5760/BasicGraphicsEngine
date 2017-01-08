#include "pyramid.hpp"

pyramid::pyramid()
{  
	vertex[0][0] = 1;vertex[0][1] = 0;vertex[0][2] = 1;
    vertex[1][0] = -1;vertex[1][1] = 0;vertex[1][2] = 1;
    vertex[2][0] = -1;vertex[2][1] = 0; vertex[2][2] = -1;
    vertex[3][0] = 1;vertex[3][1] = 0;vertex[3][2] = -1;
    vertex[4][0] = 0;vertex[4][1] = 1;vertex[4][2] = 0;


    face[0][0] = 0;face[0][1] = 1;face[0][2] = 2;face[0][3] = 3;
    face[1][0] = 0;face[1][1] = 1;face[1][2] = 4;
    face[2][0] = 1;face[2][1] = 2;face[2][2] = 4;
    face[3][0] = 2;face[3][1] = 3;face[3][2] = 4;
    face[4][0] = 3;face[4][1] = 0;face[4][2] = 4;

}

void pyramid::draw_face(int i)
{
	if (i==0){
		glBegin(GL_LINE_LOOP);
			glVertex3fv(&vertex[face[i][0]][0]);
			glVertex3fv(&vertex[face[i][1]][0]);
			glVertex3fv(&vertex[face[i][2]][0]);
			glVertex3fv(&vertex[face[i][3]][0]);
		glEnd();
	}else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_LINE_LOOP);
			glVertex3fv(&vertex[face[i][0]][0]);
			glVertex3fv(&vertex[face[i][1]][0]);
			glVertex3fv(&vertex[face[i][2]][0]);
		glEnd();
	}
}

void pyramid::draw()
{
    glPushMatrix();
    this->ctm_multiply();
	glScalef(s, s, s);
    for (int i = 0; i < 5; i++) {
        draw_face(i);
    }
    glPopMatrix();
}
