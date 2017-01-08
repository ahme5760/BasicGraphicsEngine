/**
 *	CP411 Assignment 4.
 *	Khaja Ali Ahmed
 *	110425760
 *
 */
//#include "MenuControl.hpp"
#include <GL/glew.h>


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "World.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "bezier/Bezier.hpp"

#include "glsl/Angel.h"



//#define GL_MIRRORED_REPEAT 0x8370
GLint winWidth = 800, winHeight = 800;


GLfloat red = 1.0, green = 1.0, blue = 1.0;  //color
GLint moving = 0, xBegin = 0, coordinate, type = 4, selected=0, Ptgen = 1, BCgen = 0, BCRotation = 0; // choose 0 because we're only dealing with cube
bool isShading = false;
bool isSolar = false;
bool OPENGL = false;
GLuint texture[9];
GLUquadricObj *sphere = NULL;
bool textures = false;
bool a4_menu = false;
int planet_num = 0; // 0 for sun, 1 for earth, 2 for moon
//Declare a world containing all objects to draw.
World myWorld;
Camera myCamera;
Light myLight;
Bezier myBezier;

//declare shader program object
GLuint ProgramObject;



// light properties
GLfloat ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat diffuse[] = { .6f, .6f, 1.0f, 1.0f };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat positionSolar[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat position[] = { 1.8, 1.8, 1.5, 1.0 };
GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat local_view[] = { 0.0 };

//Material
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 1, 1, 1, 1 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = { 1, 1, 1, 1 };


struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;

int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)

	char temp; // temporary color storage for bgr-rgb conversion.

	// make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}

	// seek through the bmp header, up to the width/height:

	fseek(file, 18, SEEK_CUR);

	// read the width

	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}

	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}

	size = image->sizeX * image->sizeY * 3;

	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}

	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}

	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}

	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}

	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);

	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}

	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	return 1;
}

Image* loadTexture(char *filename) {
	Image *image1;
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad(filename, image1)) {
		exit(1);
	}
	return image1;
}
void display(void) {

	if (Ptgen == 1) {
		myBezier.displayCPts();
		glFlush();
	}

	else if (BCgen == 1) {
		myBezier.displayBezCurve();
	}

	else if (BCRotation == 1 && a4_menu) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myCamera.setProjectionMatrix();  // change to 3D view

        // draw the coordinate frames
		glLineWidth(3);
		glBegin(GL_LINES);
		//x-axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(2, 0, 0);
		glVertex3f(0, 0, 0);
		//y-axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 2, 0);
		glVertex3f(0, 0, 0);
		//z-axis
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 2);
		glVertex3f(0, 0, 0);
		glEnd();
		glLineWidth(1);

		myLight.draw();
		myBezier.draw();
	}
	else{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			myCamera.setProjectionMatrix();
			myWorld.draw_world(); // draw all objects in the world
			myLight.draw();
			//myBezier.draw();
		}


	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	glEnable ( GL_COLOR_MATERIAL );
	glFlush();
	glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = newWidth;
	winHeight = newHeight;
}

void mouseAction(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		moving = 1;
		xBegin = x;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		moving = 0;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && Ptgen == 1) {
		myBezier.ctrlPts[myBezier.nCtrlPts].x = x - winWidth / 2;
		myBezier.ctrlPts[myBezier.nCtrlPts].y = winHeight / 2 - y;
		myBezier.ctrlPts[myBezier.nCtrlPts].z = 0;

		printf("Point %d set: %f %f \n", myBezier.nCtrlPts, myBezier.ctrlPts[myBezier.nCtrlPts].x, myBezier.ctrlPts[myBezier.nCtrlPts].z);

		if ( myBezier.nCtrlPts < 10) {
			 myBezier.nCtrlPts++;
		}
	}
	glutPostRedisplay();
}

void mouseMotion(GLint x, GLint y) {
	GLfloat rx, ry, rz, theta;
	if (moving) {
		if (coordinate == 1 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;
			if(isSolar){
				rx = myWorld.list[1]->getMC().mat[0][0];
				ry = myWorld.list[1]->getMC().mat[1][0];
				rz = myWorld.list[1]->getMC().mat[2][0];
				myWorld.list[1]->rotate_mc(rx, ry, rz, theta * 0.5);
				rx = myWorld.list[2]->getMC().mat[0][0];
				ry = myWorld.list[2]->getMC().mat[1][0];
				rz = myWorld.list[2]->getMC().mat[2][0];
				myWorld.list[2]->rotate_mc(rx, ry, rz, theta * 0.5);
				rx = myWorld.list[3]->getMC().mat[0][0];
				ry = myWorld.list[3]->getMC().mat[1][0];
				rz = myWorld.list[3]->getMC().mat[2][0];
				myWorld.list[3]->rotate_mc(rx, ry, rz, theta * 0.5);
			}else{
				rx = myWorld.list[selected]->getMC().mat[0][0];
				ry = myWorld.list[selected]->getMC().mat[1][0];
				rz = myWorld.list[selected]->getMC().mat[2][0];
				myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
			}

		}

		else if (coordinate == 1 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			if(isSolar){
				rx = myWorld.list[1]->getMC().mat[0][1];
				ry = myWorld.list[1]->getMC().mat[1][1];
				rz = myWorld.list[1]->getMC().mat[2][1];
				myWorld.list[1]->rotate_mc(rx, ry, rz, theta * 0.5);

				rx = myWorld.list[2]->getMC().mat[0][1];
				ry = myWorld.list[2]->getMC().mat[1][1];
				rz = myWorld.list[2]->getMC().mat[2][1];
				myWorld.list[2]->rotate_mc(rx, ry, rz, theta * 0.5);

				rx = myWorld.list[3]->getMC().mat[0][1];
				ry = myWorld.list[3]->getMC().mat[1][1];
				rz = myWorld.list[3]->getMC().mat[2][1];
				myWorld.list[3]->rotate_mc(rx, ry, rz, theta * 0.5);
			}else{
				rx = myWorld.list[selected]->getMC().mat[0][1];
				ry = myWorld.list[selected]->getMC().mat[1][1];
				rz = myWorld.list[selected]->getMC().mat[2][1];
				myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
			}

		}

		else if (coordinate == 1 && type == 3) {
			theta = (xBegin - x > 0) ? 1 : -1;
			if(isSolar){
				rx = myWorld.list[1]->getMC().mat[0][2];
				ry = myWorld.list[1]->getMC().mat[1][2];
				rz = myWorld.list[1]->getMC().mat[2][2];
				myWorld.list[1]->rotate_mc(rx, ry, rz, theta * 0.5);
				rx = myWorld.list[2]->getMC().mat[0][2];
				ry = myWorld.list[2]->getMC().mat[1][2];
				rz = myWorld.list[2]->getMC().mat[2][2];
				myWorld.list[2]->rotate_mc(rx, ry, rz, theta * 0.5);
				rx = myWorld.list[3]->getMC().mat[0][2];
				ry = myWorld.list[3]->getMC().mat[1][2];
				rz = myWorld.list[3]->getMC().mat[2][2];
				myWorld.list[3]->rotate_mc(rx, ry, rz, theta * 0.5);

			}else{

				rx = myWorld.list[selected]->getMC().mat[0][2];
				ry = myWorld.list[selected]->getMC().mat[1][2];
				rz = myWorld.list[selected]->getMC().mat[2][2];
				myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
			}

		}

		else if (coordinate == 1 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;
			if(isSolar){
				myWorld.list[1]->scale_change(theta * 0.02);
				myWorld.list[2]->scale_change(theta * 0.02);
				myWorld.list[3]->scale_change(theta * 0.02);
			}else{
				myWorld.list[selected]->scale_change(theta * 0.02);
			}

		}

		else if (coordinate == 2 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;

			rx = 1;
			ry = 0;
			rz = 0;
			if(isSolar){
				myWorld.list[1]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[2]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[3]->rotate_origin(rx, ry, rz, theta * 0.5);
			}else{
				myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);
			}



		}

		else if (coordinate == 2 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 1;
			rz = 0;
			if(isSolar){
				myWorld.list[1]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[2]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[3]->rotate_origin(rx, ry, rz, theta * 0.5);
			}else{
				myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);
			}


		}
		else if (coordinate == 2 && type == 3) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 0;
			rz = 1;
			if(isSolar){
				myWorld.list[1]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[2]->rotate_origin(rx, ry, rz, theta * 0.5);
				myWorld.list[3]->rotate_origin(rx, ry, rz, theta * 0.5);
			}else{
				myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);
			}

		}

		else if (coordinate == 2 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;
			if(isSolar){
				myWorld.list[1]->translate(theta * 0.02, 0, 0);
				myWorld.list[2]->translate(theta * 0.02, 0, 0);
				myWorld.list[3]->translate(theta * 0.02, 0, 0);
			}else{
				myWorld.list[selected]->translate(theta * 0.02, 0, 0);
			}


		}

		else if (coordinate == 2 && type == 5) {
			theta = (xBegin - x < 0) ? 1 : -1;
			if(isSolar){
				myWorld.list[1]->translate(0, theta * 0.02, 0);
				myWorld.list[2]->translate(0, theta * 0.02, 0);
				myWorld.list[3]->translate(0, theta * 0.02, 0);
			}else{
				myWorld.list[selected]->translate(0, theta * 0.02, 0);
			}


		}

		else if (coordinate == 2 && type == 6) {
			theta = (xBegin - x < 0) ? 1 : -1;
			if(isSolar){
				myWorld.list[1]->translate(0, 0, theta * 0.02);
				myWorld.list[2]->translate(0, 0, theta * 0.02);
				myWorld.list[3]->translate(0, 0, theta * 0.02);
			}else{
				myWorld.list[selected]->translate(0, 0, theta * 0.02);
			}


		}

		else if (coordinate == 3 && type == 1) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=1;
			ry=0;
			rz=0;
			myCamera.rotate(rx,ry,rz, theta);


		}
		else if (coordinate == 3 && type == 2) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=0;
			ry=1;
			rz=0;
			myCamera.rotate(rx,ry,rz, theta);
		}

		else if (coordinate == 3 && type == 3) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=0;
			ry=0;
			rz=1;
			myCamera.rotate(rx,ry,rz, theta);
		}

		else if (coordinate == 3 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=1*theta*0.2;
			ry=0;
			rz=0;
			myCamera.translate(rx,ry,rz);

		}

		else if (coordinate == 3 && type == 5) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=0;
			ry=1*theta*0.2;
			rz=0;
			myCamera.translate(rx,ry,rz);

		}
		else if (coordinate == 3 && type == 6) {
			theta = (xBegin - x < 0) ? 1 : -1;
			rx=0;
			ry=0;
			rz=1*theta*0.2;
			myCamera.translate(rx,ry,rz);
		}

		else if (coordinate == 3 && type == 7) {

			theta = (xBegin - x < 0) ? 1 : -1;
			if (myCamera.nearDist + theta*0.2 > 0){
				myCamera.nearDist += theta*0.2;
			}

		}

		else if (coordinate == 3 && type == 8) {
			theta = (xBegin - x < 0) ? 1 : -1;
			if (myCamera.farDist + theta*0.2 > 0){
				myCamera.farDist += theta*0.2;
			}

		}

		else if (coordinate == 3 && type == 9) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myCamera.viewAngle += theta;

		}
		else if (coordinate == 5 && type == 5) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myCamera.viewAngle += theta;

		}

		else if (coordinate == 4 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;

			rx = 1;
			ry = 0;
			rz = 0;

			myLight.rotate_origin(rx, ry, rz, theta * 0.5);


		}

		else if (coordinate == 4 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 1;
			rz = 0;
			myLight.rotate_origin(rx, ry, rz, theta * 0.5);
		}
		else if (coordinate == 4 && type == 3) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 0;
			rz = 1;
			myLight.rotate_origin(rx, ry, rz, theta * 0.5);
		}

		else if (coordinate == 4 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.translate(theta * 0.02, 0, 0);

		}

		else if (coordinate == 4 && type == 5) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.translate(0, theta * 0.02, 0);

		}

		else if (coordinate == 4 && type == 6) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.translate(0, 0, theta * 0.02);

		}
		else if (coordinate == 4 && type == 7) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.Increment(theta * 0.02, 0, 0);

		}
		else if (coordinate == 4 && type == 8) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.Increment(0, 0, theta * 0.02);

		}
		else if (coordinate == 4 && type == 9) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myLight.Increment(0, theta * 0.02, 0);

		}

		position[0] = myLight.getMC().mat[0][3];
		position[1] = myLight.getMC().mat[1][3];
		position[2] = myLight.getMC().mat[2][3];
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		glutPostRedisplay();
	}

}

void init(void) {
	glewInit();

	ProgramObject = InitShader( "vertexshader.txt", "fragmentshader.txt" );
	glUseProgram( ProgramObject );
	glClearColor(0.0, 0.0, 0.0, 1.0);  // Set display-window color to black
	glColor3f(red, green, blue);


	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	myLight.translate(1, 1, 1);

	myCamera.setProjectionMatrix();

	glutPostRedisplay();


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Create two texture
	glGenTextures(9, texture);



	// second texture configure and loading
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image1 = loadTexture("sun.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image1->data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image2 = loadTexture("mandrill.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image2->data);


	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image3 = loadTexture("earth.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image3->data);


	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image4 = loadTexture("moon.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image4->data);


	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image5 = loadTexture("draw.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image5->data);


	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	Image* image6 = loadTexture("checker.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image6->data);

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	Image* image7 = loadTexture("sun.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image7->sizeX, image7->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image7->data);

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	Image* image8 = loadTexture("earthsphere.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image8->sizeX, image8->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image8->data);

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	Image* image9 = loadTexture("moon.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image9->sizeX, image9->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image9->data);



	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glShadeModel(GL_FLAT);


	BCgen = 0;
	BCRotation = 0;
	textures=false;
	glUseProgram(0);
	glutIdleFunc(NULL);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	 Ptgen = 1; glFlush(); glutPostRedisplay();
    //myWorld.list[1] = &mySolar;
    myWorld.list[4] = &myBezier;
}



void MCTransMenu(GLint transOption) {
	a4_menu=true;
	switch (transOption) {
	case 1: {
		coordinate = 1;
		type = 1;
	}
		break;
	case 2: {
		coordinate = 1;
		type = 2;
	}
		break;
	case 3: {
		coordinate = 1;
		type = 3;
	}
		break;
	case 4: {
		coordinate = 1;
		type = 4;
	}
		break;
	}
	glutPostRedisplay();
}

void WCTransMenu(GLint transOption) {
	a4_menu=true;
	switch (transOption) {
	case 1: {
		coordinate = 2;
		type = 1;
	}
		break;
	case 2: {
		coordinate = 2;
		type = 2;
	}
		break;
	case 3: {
		coordinate = 2;
		type = 3;
	}
		break;
	case 4: {
		coordinate = 2;
		type = 4;
	}
		break;
	case 5: {
		coordinate = 2;
		type = 5;
	}
		break;
	case 6: {
		coordinate = 2;
		type = 6;
	}
	break;
	}
	glutPostRedisplay();
}

void move(void){
	GLfloat rx,ry,rz;
	if (!OPENGL&&moving == 0){
		if(!isSolar) {

			rx = myWorld.list[selected]->getMC().mat[0][1];
			ry = myWorld.list[selected]->getMC().mat[1][1];
			rz = myWorld.list[selected]->getMC().mat[2][1];
			myWorld.list[selected]->rotate_mc(rx, ry, rz, -0.05);
//cube

		} else{

//solar system

			rx = myWorld.list[1]->getMC().mat[0][1];
			ry = myWorld.list[1]->getMC().mat[1][1];
			rz = myWorld.list[1]->getMC().mat[2][1];
			myWorld.list[1]->rotate_mc(rx, ry, rz, -5);
			myWorld.list[2]->rotate_mc(myWorld.list[2]->getMC().mat[0][0], myWorld.list[2]->getMC().mat[1][0],myWorld.list[2]->getMC().mat[2][0], -5);
			myWorld.list[2]->rotate_relative(myWorld.list[1]->getMC().mat[0][3], myWorld.list[1]->getMC().mat[1][3],  myWorld.list[1]->getMC().mat[2][3], 0, -1, 0, 0.5);


			myWorld.list[3]->rotate_mc(myWorld.list[3]->getMC().mat[0][0],myWorld.list[3]->getMC().mat[1][0],myWorld.list[3]->getMC().mat[2][0], -0.5);
			myWorld.list[3]->rotate_relative(myWorld.list[2]->getMC().mat[0][3],myWorld.list[2]->getMC().mat[1][3],myWorld.list[2]->getMC().mat[2][3],
					0,1,0,5);


		}

		glutPostRedisplay();
	}
	if (OPENGL){//opengl light and shading
		// Create light components
		float Ra = myLight.Ra;
		float Rd = myLight.Rd;
		float I = myLight.I;
		float posn0 = myLight.getMC().mat[0][3];
		float posn1 = myLight.getMC().mat[1][3];
		float posn2 = myLight.getMC().mat[2][3];

		GLfloat light0_ambient[] = { Ra,Ra,Ra, 1.0f };
		GLfloat light0_diffuse[] = { Rd, Rd, Rd, 1.0f };
		GLfloat emissionLight[] = { I,I,I, 1.0f  };
		GLfloat light0_position[] = {posn0,posn1, posn2, 1.0f};



		// Assign created components to GL_LIGHT0
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_EMISSION, emissionLight);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		float colorOrangeRed[] = { 1.0f, 0.2f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorOrangeRed);
	}
	glutPostRedisplay();


}

void reset(void) {
	glutIdleFunc(NULL);
	glDisable(GL_TEXTURE_2D);
	a4_menu=true;

	type=2;

	glUseProgram(0);
	selected = 4;
	Ptgen = 1;
	BCgen = 0;
	BCRotation = 0;

	isSolar = false;
	isShading = false;
	OPENGL = false;
	textures=false;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	glDisable( GL_LIGHTING );
	glDisable(GL_LIGHT0);
	myWorld.reset();
	myBezier.reset();

	glFlush();
	glutPostRedisplay();
}

void LightMenu(GLint transOption) {
	a4_menu=true;
	coordinate = 4;
	type = transOption;
	if (type == 10) {
		myLight.On = true;
	}
	if (type == 11) {
		myLight.On = false;
	}

	glutPostRedisplay();
}



void mainMenu(GLint option) {

   switch (option) {
   case 1: {
	   reset();
	   moving = 0; xBegin = 0; coordinate = 6; type = 4; selected=0;
   }
   break;
   case 2: exit(0);
   }
	glutPostRedisplay();
}

void VCTransMenu(GLint transOption) {
	a4_menu=true;
	switch (transOption) {
	case 1: {
		coordinate = 3;
		type = 1;
	}
		break;
	case 2: {
		coordinate = 3;
		type = 2;
	}
		break;
	case 3: {
		coordinate = 3;
		type = 3;
	}
		break;
	case 4: {
		coordinate = 3;
		type = 4;
	}
		break;
	case 5: {
		coordinate = 3;
		type = 5;
	}
		break;
	case 6: {
		coordinate = 3;
		type = 6;
	}
	break;
	case 7: {
		coordinate = 3;
		type = 7;
	}
	break;
	case 8: {
		coordinate = 3;
		type = 8;
	}
	break;
	case 9: {
		coordinate = 3;
		type = 9;
	}
	break;
	}
	glutPostRedisplay();
}


void A3Menu(GLint option) {
	myLight.On = true;
	myLight.draw();
	glDisable(GL_TEXTURE_2D);
	glutIdleFunc(NULL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glUseProgram(0);
	isSolar = false;
	Ptgen = 0;
	BCgen = 0;
	BCRotation = 0;
	textures=false;

	if(option == 1){ // my backface
		coordinate=5;
		isShading = false;
		a4_menu=true;
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

	} else if(option == 2){
		coordinate=5;
		isShading = true;
		a4_menu=true;
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);


	} else if(option == 3){
		coordinate=5;
		isShading = false;
		a4_menu=true;
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

	}
	else if(option == 4){
		coordinate=5;
		isShading = false;
		a4_menu=true;
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Create light components
		float Ra = myLight.Ra;
		float Rd = myLight.Rd;
		float I = myLight.I;
		float posn0 = myLight.getMC().mat[0][3];
		float posn1 = myLight.getMC().mat[1][3];
		float posn2 = myLight.getMC().mat[2][3];

		GLfloat light0_ambient[] = { Ra,Ra,Ra, 1.0f };
		GLfloat light0_diffuse[] = { Rd, Rd, Rd, 1.0f };
		GLfloat emissionLight[] = { I,I,I, 1.0f  };
		GLfloat light0_position[] = {posn0,posn1, posn2, 1.0f};



		// Assign created components to GL_LIGHT0
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_EMISSION, emissionLight);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		float colorBlue[] = { 1.0f, 0.33f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorBlue);

		OPENGL = true;
		glutIdleFunc(move);
	  /// light setting

		}
	else if(option == 5){ //cube animation
		coordinate=5;
		textures=false;
		moving = 0;
		isSolar = false;
		a4_menu=true;
		glutIdleFunc(move);
	}

	else if(option == 6){ //solar system
		//reset();
		isSolar = true;
		myLight.On = false;
		a4_menu=false;
		coordinate=5;
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		myWorld.list[1]->scale_change(-0.6);
		myWorld.list[2]->scale_change(-0.9);
		myWorld.list[3]->scale_change(-0.95);
		myWorld.list[2]->translate(1, 0, 1);
		myWorld.list[3]->translate(1.2, 0, 1.2);

		glutIdleFunc(move);

	}
	glutPostRedisplay();

}


void A4Menu(GLint option) {
	myLight.On = true;
	myLight.draw();
	type = option;
	Ptgen = 0;
	BCgen = 0;
	BCRotation = 0;
	isSolar=false;
	textures=false;
	glUseProgram(0);
	glutIdleFunc(NULL);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	switch (option) {
		case 1:  Ptgen = 1; glFlush(); glutPostRedisplay(); a4_menu=false;break;
		case 2:  BCgen = 1; a4_menu=false;break;
		case 3:
			a4_menu=true;

			selected = 4;
			BCRotation = 1;
			myBezier.RotateBezier(myBezier.ctrlPts, myBezier.nCtrlPts, myBezier.T);
			break;

		case 4:
			a4_menu=true;

			selected = 4;
			BCRotation = 1;
			myBezier.setRenderMode(myBezier.MODE_WIRE);

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glDisable(GL_NORMALIZE);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_LIGHT0);
			break;

		case 5:
			a4_menu=true;

			selected = 4;
			BCRotation = 1;
			myBezier.setRenderMode(myBezier.MODE_SOLID);

			glEnable(GL_DEPTH_TEST); // enable OpenGL depth buffer algorithm for hidden surface removal
			glEnable(GL_CULL_FACE);
			glEnable(GL_LIGHTING);
			glEnable( GL_NORMALIZE);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHT0);
			glCullFace(GL_BACK);
			break;

		case 6:

			a4_menu=true;

			selected = 4;
			BCRotation = 1;

			glUseProgram( ProgramObject );
			myBezier.setRenderMode(myBezier.MODE_SOLID);

			myLight.On = true;
			myLight.draw();

			position[0] = myLight.getMC().mat[0][3];
			position[1] = myLight.getMC().mat[1][3];
			position[2] = myLight.getMC().mat[2][3];

			glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glLightfv(GL_LIGHT0, GL_POSITION, position);

			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
			glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_ambient);
			glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

			glEnable(GL_DEPTH_TEST); // enable OpenGL depth buffer algorithm for hidden surface removal
			glEnable(GL_CULL_FACE);
			glEnable(GL_LIGHTING);
			glEnable( GL_NORMALIZE);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHT0);
			glCullFace(GL_BACK);

			break;

		case 7:

			//reset();
			isShading=true;
			a4_menu=true;
			type=7;

			selected = 0;

			textures = true;
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			break;

		case 8:
			//reset();
			a4_menu=false;
			type=8;

			selected = 1;


			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_NORMALIZE);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);



			isSolar = true;
			myLight.On = false;

			myWorld.list[1]->scale_change(-0.6);
			myWorld.list[2]->scale_change(-0.9);
			myWorld.list[3]->scale_change(-0.95);
			myWorld.list[1]->translate(0, 0, 0);
			myWorld.list[2]->translate(2, 0, 0);
			myWorld.list[3]->translate(2.4, 0, 0);

			glutIdleFunc(move);

			break;

		default: break;

	}
	glutPostRedisplay();
}

void RotationMenu(GLint option) {
	switch (option) {
		case 1: myBezier.R = 45; break;
		case 2: myBezier.R = 90; break;
		case 3: myBezier.R = 135; break;
		case 4: myBezier.R = 180; break;
		case 5: myBezier.R = 225; break;
		case 6: myBezier.R = 270; break;
		case 7: myBezier.R = 315; break;
		case 8: myBezier.R = 360; break;
		case 9: myBezier.S = 5; break;
		case 10: myBezier.S = 10; break;
		case 11: myBezier.S = 15; break;
		case 12: myBezier.T = 5; break;
		case 13: myBezier.T = 10; break;
		case 14: myBezier.T = 15; break;
		case 15: myBezier.T = 20; break;
		case 16: myBezier.T = 25; break;
		default: break;
	}
	glutPostRedisplay();
}

void menu() {
	GLint WCTrans_Menu, VCTrans_Menu, MCTrans_Menu;
	MCTrans_Menu = glutCreateMenu(MCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Scale", 4);

	WCTrans_Menu = glutCreateMenu(WCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);

	VCTrans_Menu = glutCreateMenu(VCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);
	glutAddMenuEntry(" Clipping Near ", 7);
	glutAddMenuEntry(" Clipping Far ", 8);
	glutAddMenuEntry(" Angle ", 9);

	GLint Light_Menu = glutCreateMenu(LightMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);
	glutAddMenuEntry(" Point Light Intensity I", 7);
	glutAddMenuEntry(" Point Light Reflection Rd", 8);
	glutAddMenuEntry(" Ambient Reflection Ra",9);
	glutAddMenuEntry(" Show Point Light ", 10);
	glutAddMenuEntry(" Hide Point Light ", 11);


	//A_3 menu
	GLint A3_Menu = glutCreateMenu(A3Menu);
	glutAddMenuEntry(" My Hidden Face Removal", 1);
	glutAddMenuEntry(" My Light & Shading ", 2);
	glutAddMenuEntry(" OpenGL Hidden Face Removal", 3);
	glutAddMenuEntry(" OpenGL Light & Shading", 4);
	glutAddMenuEntry(" Cube Animation ", 5);
	glutAddMenuEntry(" Simple Solar System", 6);

	GLint Rotation_Menu = glutCreateMenu(RotationMenu);
	glutAddMenuEntry(" R->45 ", 1);
	glutAddMenuEntry(" R->90 ", 2);
	glutAddMenuEntry(" R->135 ", 3);
	glutAddMenuEntry(" R->180 ", 4);
	glutAddMenuEntry(" R->225 ", 5);
	glutAddMenuEntry(" R->270 ", 6);
	glutAddMenuEntry(" R->315 ", 7);
	glutAddMenuEntry(" R->360 ", 8);
	glutAddMenuEntry(" S->5 ", 9);
	glutAddMenuEntry(" S->10 ", 10);
	glutAddMenuEntry(" S->15 ", 11);
	glutAddMenuEntry(" T->5 ", 12);
	glutAddMenuEntry(" T->10 ", 13);
	glutAddMenuEntry(" T->15 ", 14);
	glutAddMenuEntry(" T->20 ", 15);
	glutAddMenuEntry(" T->25 ", 16);

	//A_4 menu
	GLint A4_Menu = glutCreateMenu(A4Menu);
	glutAddMenuEntry(" Control Point Selection ", 1);
	glutAddMenuEntry(" Bezier Curve Generation ", 2);
	glutAddSubMenu(" Rotation Configuration ", Rotation_Menu);
	glutAddMenuEntry(" Rotation surface Generation ", 3);
	glutAddMenuEntry(" Wired Mesh ", 4);
	glutAddMenuEntry(" Solid Mesh OpenGL Shading ", 5);
	glutAddMenuEntry(" Solid Mesh GLSL Shading ", 6);
	glutAddMenuEntry(" Cube Texture ",7);
	glutAddMenuEntry(" Solar System Texture ", 8);

	glutCreateMenu(mainMenu);      // Create main pop-up menu.
	glutAddMenuEntry(" Reset ", 1);
	glutAddSubMenu(" Model Transformations ", MCTrans_Menu);
	glutAddSubMenu(" WC Transformations ", WCTrans_Menu);
	glutAddSubMenu(" View Transformations ", VCTrans_Menu);
	glutAddSubMenu(" Light Transformations ", Light_Menu);
	glutAddSubMenu(" A3 Menu", A3_Menu);
	glutAddSubMenu(" A4 Menu", A4_Menu);
	glutAddMenuEntry(" Quit", 2);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Ali Ahmed A2");
	init();
	menu();

	glutDisplayFunc(display);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseAction);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}

