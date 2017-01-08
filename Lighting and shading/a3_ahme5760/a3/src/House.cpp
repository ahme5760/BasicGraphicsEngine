#include "House.hpp"

House::House()
{  
	vertex[0][0] = 1;vertex[0][1] = 1;vertex[0][2] = 1;
    vertex[1][0] = -1;vertex[1][1] = 1;vertex[1][2] = 1;
    vertex[2][0] = -1;vertex[2][1] = 1; vertex[2][2] = -1;
    vertex[3][0] = 1;vertex[3][1] = 1;vertex[3][2] = -1;
    vertex[4][0] = 0;vertex[4][1] = 2;vertex[4][2] = 0;
    vertex[5][0] = -1;vertex[5][1] = -1;vertex[5][2] = -1;
    vertex[6][0] = -1;vertex[6][1] = 1; vertex[6][2] = -1;
    vertex[7][0] = 1;vertex[7][1] = -1; vertex[7][2] = -1;
    vertex[8][0] = 1;vertex[8][1] = 1;vertex[8][2] = -1;
    vertex[9][0] = -1;vertex[9][1] = -1;vertex[9][2] = 1;
    vertex[10][0] = -1;vertex[10][1] = 1; vertex[10][2] = 1;
    vertex[11][0] = 1;vertex[11][1] = -1;vertex[11][2] = 1;
    vertex[12][0] = 1;vertex[12][1] = 1;vertex[12][2] = 1;


    face[0][0] = 0;face[0][1] = 1;face[0][2] = 2;face[0][3] = 3;
    face[1][0] = 0;face[1][1] = 1;face[1][2] = 4;
    face[2][0] = 1;face[2][1] = 2;face[2][2] = 4;
    face[3][0] = 2;face[3][1] = 3;face[3][2] = 4;
    face[4][0] = 3;face[4][1] = 0;face[4][2] = 4;
    face[5][0] = 5;face[5][1] = 6;face[5][2] = 8;face[5][3] = 7;
    face[6][0] = 8;face[6][1] = 12;face[6][2] = 11;face[6][3] = 7;
    face[7][0] = 12;face[7][1] = 10;face[7][2] = 9;face[7][3] = 11;
    face[8][0] = 9;face[8][1] = 10;face[8][2] = 6;face[8][3] = 5;
    face[9][0] = 10;face[9][1] = 12;face[9][2] = 8;face[9][3] = 6;
    face[10][0] = 11;face[10][1] = 9;face[10][2] = 5;face[10][3] = 7;

}

void House::draw_face(int i)
{

	if (i==0 || i>4){
		glBegin(GL_LINE_LOOP);
			glVertex3fv(&vertex[face[i][0]][0]);
			glVertex3fv(&vertex[face[i][1]][0]);
			glVertex3fv(&vertex[face[i][2]][0]);
			glVertex3fv(&vertex[face[i][3]][0]);
		glEnd();
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_LINE_LOOP);
			glVertex3fv(&vertex[face[i][0]][0]);
			glVertex3fv(&vertex[face[i][1]][0]);
			glVertex3fv(&vertex[face[i][2]][0]);
		glEnd();
	}
}

void House::draw()
{
    glPushMatrix();
    this->ctm_multiply();
	glScalef(s, s, s);
    for (int i = 0; i < 11; i++) {
        draw_face(i);
    }
    glPopMatrix();
}
