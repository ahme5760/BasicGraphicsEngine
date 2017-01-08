/**
 *	CP411 Assignment 3.
 *	Khaja Ali Ahmed
 *	110425760
 *
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "World.hpp"
#include "Camera.hpp"
#include "Light.hpp"


GLint winWidth = 800, winHeight = 800;

GLfloat red = 1.0, green = 1.0, blue = 1.0;  //color
GLint moving = 0, xBegin = 0, coordinate = 1, type = 4, selected=0; // choose 0 because we're only dealing with cube
bool isShading = false;
bool isSolar = false;
bool OPENGL = false;


//Declare a world containing all objects to draw.
World myWorld;
Camera myCamera;
Light myLight;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myCamera.setProjectionMatrix();

	myWorld.draw_world(); // draw all objects in the world
	myLight.draw();

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
}

void mouseMotion(GLint x, GLint y) {
	GLfloat rx, ry, rz, theta;
	if (moving) {
		if (coordinate == 1 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = myWorld.list[selected]->getMC().mat[0][0];
			ry = myWorld.list[selected]->getMC().mat[1][0];
			rz = myWorld.list[selected]->getMC().mat[2][0];
			myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
		}

		else if (coordinate == 1 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = myWorld.list[selected]->getMC().mat[0][1];
			ry = myWorld.list[selected]->getMC().mat[1][1];
			rz = myWorld.list[selected]->getMC().mat[2][1];
			myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
		}

		else if (coordinate == 1 && type == 3) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = myWorld.list[selected]->getMC().mat[0][2];
			ry = myWorld.list[selected]->getMC().mat[1][2];
			rz = myWorld.list[selected]->getMC().mat[2][2];
			myWorld.list[selected]->rotate_mc(rx, ry, rz, theta * 0.5);
		}

		else if (coordinate == 1 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;

			myWorld.list[selected]->scale_change(theta * 0.02);
		}

		else if (coordinate == 2 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;

			rx = 1;
			ry = 0;
			rz = 0;

			myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);


		}

		else if (coordinate == 2 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 1;
			rz = 0;
			myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);
		}
		else if (coordinate == 2 && type == 3) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = 0;
			ry = 0;
			rz = 1;
			myWorld.list[selected]->rotate_origin(rx, ry, rz, theta * 0.5);
		}

		else if (coordinate == 2 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myWorld.list[selected]->translate(theta * 0.02, 0, 0);

		}

		else if (coordinate == 2 && type == 5) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myWorld.list[selected]->translate(0, theta * 0.02, 0);

		}

		else if (coordinate == 2 && type == 6) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myWorld.list[selected]->translate(0, 0, theta * 0.02);

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

		glutPostRedisplay();
	}

}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);  // Set display-window color to black
	glColor3f(red, green, blue);
	myCamera.setProjectionMatrix();
	myLight.translate(1, 1, 1);

}



void MCTransMenu(GLint transOption) {
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
			myWorld.list[1]->rotate_mc(rx, ry, rz, -0.1);
			myWorld.list[2]->rotate_origin(0,1,0, -0.5);
			myWorld.list[3]->rotate_origin_new(0,1,0, -0.5);// dont know how to rotate moon around earth, instead i rotate around sun


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
	isSolar = false;
	isShading = false;
	OPENGL = false;
	glDisable( GL_LIGHTING );
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHT0);
	myWorld.reset();
	glutPostRedisplay();
}

void LightMenu(GLint transOption) {
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
	   moving = 0; xBegin = 0; coordinate = 1; type = 4; selected=0;
   }
   break;
   case 2: exit(0);
   }
	glutPostRedisplay();
}

void VCTransMenu(GLint transOption) {
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

	glutIdleFunc(NULL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	isSolar = false;

	if(option == 1){ // my backface
		isShading = false;
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

	} else if(option == 2){
		isShading = true;
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);


	} else if(option == 3){

		isShading = false;
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

	}
	else if(option == 4){

		isShading = false;
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
		moving = 0;
		isSolar = false;
		glutIdleFunc(move);
	}

	else if(option == 6){ //solar system
		isSolar = true;
		myLight.On = false;

		myWorld.list[1]->scale_change(-0.6);
		myWorld.list[2]->scale_change(-0.9);
		myWorld.list[3]->scale_change(-0.95);
		myWorld.list[2]->translate(1, 0, 1);
		myWorld.list[3]->translate(1.2, 0, 1.2);

		glutIdleFunc(move);

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


	glutCreateMenu(mainMenu);      // Create main pop-up menu.
	glutAddMenuEntry(" Reset ", 1);
	glutAddSubMenu(" Model Transformations ", MCTrans_Menu);
	glutAddSubMenu(" WC Transformations ", WCTrans_Menu);
	glutAddSubMenu(" View Transformations ", VCTrans_Menu);
	glutAddSubMenu(" Light Transformations ", Light_Menu);
	glutAddSubMenu(" A3 Menu", A3_Menu);
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

