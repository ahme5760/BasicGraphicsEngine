/**
 *	CP411 Assignment 2.
 *	Khaja Ali Ahmed
 *	110425760
 *
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "World.hpp"
#include "Camera.hpp"

GLint winWidth = 800, winHeight = 800;

GLfloat red = 1.0, green = 1.0, blue = 1.0;  //color
GLint moving = 0, xBegin = 0, coordinate = 1, type = 4, selected;

//Declare a world containing all objects to draw.
World myWorld;
Camera myCamera;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myCamera.setProjectionMatrix();
	myWorld.draw_world(); // draw all objects in the world
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
		glutPostRedisplay();
	}

}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);  // Set display-window color to black
	glColor3f(red, green, blue);
	myCamera.setProjectionMatrix();
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

void mainMenu(GLint option) {
   switch (option) {
   case 1: {moving = 0; xBegin = 0; coordinate = 1; type = 4; selected=0;
   myWorld.list[0] = new Cube();
   myWorld.list[1] = new pyramid();
   myWorld.list[2] = new House();
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

void colorSubMenu(GLint colorOption) {
    switch (colorOption) {
   case 1:
   {red = 1.0; green = 0.0; blue = 0.0;} break;
   case 2:
   {red = 0.0; green = 1.0; blue = 0.0;} break;
   case 3:
  {red = 0.0; green = 0.0; blue = 1.0;}break;
    }
    glColor3f(red, green, blue);
	glutPostRedisplay();
}

void ObjSubMenu(GLint objectOption) {
	switch (objectOption) {
	case 1: {
		selected = 0;
	}break;

	case 2: {
		selected = 1;
	}break;

	case 3: {
		selected = 2;
	}
	}
	glutPostRedisplay();
}

void menu() {
	GLint WCTrans_Menu, VCTrans_Menu, MCTrans_Menu, subMenuColor, Object_Menu;
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

	subMenuColor = glutCreateMenu(colorSubMenu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);

	Object_Menu = glutCreateMenu(ObjSubMenu);
	glutAddMenuEntry(" Cube ", 1);
	glutAddMenuEntry(" Pyramid ", 2);
	glutAddMenuEntry(" House ", 3);

	glutCreateMenu(mainMenu);      // Create main pop-up menu.
	glutAddMenuEntry(" Reset ", 1);
	glutAddSubMenu(" Object Selection ", Object_Menu);
	glutAddSubMenu(" Model Transformations ", MCTrans_Menu);
	glutAddSubMenu(" WC Transformations ", WCTrans_Menu);
	glutAddSubMenu(" View Transformations ", VCTrans_Menu);
	glutAddSubMenu(" Colors ", subMenuColor);
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

