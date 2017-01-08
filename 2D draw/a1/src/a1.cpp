 /*
Project: CP411A1

Author: Khaja Ali Ahmed
Student ID: 110425760


*/

#define _USE_MATH_DEFINES
// OpenGL and C headers
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>




void mainMenu(GLint menuOption);

// C++ headers for file I/O
#include <fstream>
#include <iostream>
using namespace std;

// function prototype
//#define some constant here
#define  MaxNumObj 100

// global variables
GLsizei winWidth = 640, winHeight = 480; // variables and initial for width and height of display window
GLint tool = 0, type = 1, style = 1, selected = 0, highlight = 0, selection = 0, move = 0, //highlight is new variable I made for the index within the list for the "ring" around the selected object
xbegin, ybegin, ptCtr = 0, numObj = 0, xOld=0, yOld=0; // xOld and yOld are new variables I made to keep track of the distance the mouse moves while moving objects

GLfloat red = 1.0, green = 0.0, blue = 0.0;

// data structures, you define the structures for the drawing objects, you can use the data structure given below
class modelObject {
public:
GLint x1, y1, x2, y2, t, s, radius; // t is for types of object: 1 for rectangle, 2 for cicle; s for drawing styles: 1 for filled, 2 for outline, radius is circle radius if t=2, if t=1 radius=0=default value
GLfloat r, g, b; // RGB color
} list[100]; // this is an array data structure, you can also use link list for storing objects


// function section
// initial function to set up OpenGL state variable other than default.
void init(void) {
glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to white
glMatrixMode(GL_PROJECTION);
gluOrtho2D(0.0, winWidth, winHeight, 0.0);
glColor3f(red, green, blue);
glFlush();
}

/* Drawing a point for testing */
void setPixel(GLint x, GLint y) {
glPointSize(1.0);
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();

}
// Functions from class for midpoint method of drawing circle

void circlePlotPoints(GLfloat xc, GLfloat yc, GLfloat x, GLfloat y) {
setPixel(xc + x, yc + y); setPixel(xc - x, yc + y);
setPixel(xc + x, yc - y); setPixel(xc - x, yc - y);
setPixel(xc + y, yc + x); setPixel(xc - y, yc + x);
setPixel(xc + y, yc - x); setPixel(xc - y, yc - x);
}

void circlePlotPointsFill (GLfloat xc, GLfloat yc, GLfloat x, GLfloat y, GLfloat r) {
glLineWidth(2.0);
glBegin(GL_TRIANGLE_FAN);
glVertex2f(xc, yc); // center of circle
int num_triangles = 100;
for(int i = 0; i <= num_triangles;i++) {
	glVertex2f(
            xc + (r * cos(i *  (2*M_PI) / num_triangles)),
	    yc + (r * sin(i * (2*M_PI) / num_triangles))
	);
}
glEnd();
}
void circleMidpoint(GLfloat xc, GLfloat yc, GLfloat r, GLfloat s) {
GLint p = 1 - r; // Initial value of midpoint parameter.
GLint x = 0, y = r; // Set coordinates for top point of circle.
/* Plot the initial point in each circle quadrant. */
if (s == 1){
	circlePlotPointsFill(xc, yc, x, y, r);
}else{
	circlePlotPoints(xc, yc, x, y);
}
while (x < y) {
x++;
if (p < 0) { p += 2 * x + 1; }
else { y--; p += 2 * (x - y) + 1; }
circlePlotPoints(xc, yc, x, y);
}
}




// this function draw an object

void plotObject(GLint x1, GLint y1, GLint x2, GLint y2, GLint t, GLint s) {
	glLineWidth(3.0);
	if (t == 1){ // draw rectangle

		if (s == 1){
			glBegin(GL_FILL);
			glRecti(x1, y1, x2, y2);
			glEnd();
		}
		else {
			glBegin(GL_LINE_LOOP);
			glVertex2i(x1, y1);
			glVertex2i(x1, y2);
			glVertex2i(x2, y2);
			glVertex2i(x2, y1);
			glEnd();
		}

	}
	if ( t == 2){ // draw circle

		int radius = sqrt(pow((x1-x2),2) + pow(y1-y2,2));
		circleMidpoint(x1, y1, radius, s);

	}
	if ( t== 3){ //draw line
		glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}

}




// this function draws the list of objects
void drawList() {

glClear(GL_COLOR_BUFFER_BIT); // Clear display window.
for (int i = 0; i <= numObj; i++) {
	glColor3f(list[i].r, list[i].g, list[i].b);
	plotObject(list[i].x1, list[i].y1, list[i].x2, list[i].y2, list[i].t, list[i].s);
}
glFlush();
}
// delete object given index
void Delete (GLint j)   // j is an index for the selected object
{
	GLint i;

	for ( i = j; i<=numObj; i++)
	{
		list[i].x1 = list[i+1].x1;
		list[i].y1 = list[i+1].y1;
		list[i].x2 = list[i+1].x2;
		list[i].y2 = list[i+1].y2;
		list[i].r = list[i+1].r;
		list[i].g = list[i+1].g;
		list[i].b = list[i+1].b;
		list[i].t = list[i+1].t;
		list[i].s = list[i+1].s;
	}
	numObj--;

	glutPostRedisplay();
	return;
}
// draw the highlighted part when an object is selected
void drawSelected(GLint i){ // i is the index of the selected object
if(selection ==1){
	list[numObj].x1 = list[i].x1;	// we use the same dimensions as the object we selected
	list[numObj].y1 = list[i].y1;
	list[numObj].x2 = list[i].x2;
	list[numObj].y2 = list[i].y2;
	list[numObj].radius = list[i].radius; // because I modified list struct to include radius
	list[numObj].t = list[i].t;
	list[numObj].s = 2;		// object must be outline
	list[numObj].r = 0;
	list[numObj].g = 0;		//making object black so user can see
	list[numObj].b = 0;
	numObj++;

	selected = i;			// index of selected object
	highlight = numObj-1;
	glutPostRedisplay();
}

	return;
}

//Function to delete highlight object. (ie the outline around the selected object)
void deleteHighlight(){

	if (highlight != 0){		// highlight is the index of the outline, highlight = 0 means nothing was selected, so there is nothing to delete
		GLint temp = highlight;
		highlight = 0;			// reset highlight to zero for the next time user wants to select an object
		Delete(temp);
	}
	return;
}
void select (GLint button, GLint state, GLint x, GLint y){
	for (int i = numObj; i >=0; i--) {
		if (list[i].t==1){																		//for rectangles
			if (list[i].x1 < x && x < list[i].x2 && list[i].y1 < y && y < list[i].y2){         // for an object to be selected, the mouse coordinates must be within the object's dimensions
				deleteHighlight();																// delete the old highlight outline object if one exists
				drawSelected(i);
				return;
			}
		}
		if (list[i].t==2){																		// for circles
			GLfloat d = sqrt(pow(x-list[i].x1,2) + pow(y-list[i].y1,2));						// use pythagorean's theorem to see if the given coordinate is inside the circle
			if((d<=list[i].radius)){															// if d <= r it is inside circle
				deleteHighlight();
				drawSelected(i);
				return;
			}
		}
	}
}
// call this function to deselect the currently selected object, this deletes the highlight and sets selected object index to 0,
void deselect(){
	if (highlight != 0) {
		Delete(highlight);
		selected = 0;
	}
	glutPostRedisplay();
	return;
}
// bring object to front of screen, to do this, a new object with same dimensions is drawn and the selected object is deleted
void bringToFront(GLint i){
	list[numObj].x1 = list[i].x1;
	list[numObj].x2 = list[i].x2;
	list[numObj].y1 = list[i].y1;
	list[numObj].y2 = list[i].y2;
	list[numObj].r = list[i].r;
	list[numObj].b = list[i].b;
	list[numObj].g = list[i].g;
	list[numObj].s = list[i].s;
	list[numObj].t = list[i].t;
	list[numObj].radius = list[i].radius;
	numObj++;
	deleteHighlight();
	Delete(i);
	selection = 0;

	glutPostRedisplay();
	return;
}
// sends the selected object to the back of the screen, to do this, each object in list is shifted to the right by 1, and at position 0, selected object is inserted, and then old selected object is deleted
void sendToBack(GLint i){
	GLint k;
	for (k=numObj-1; k>=0; k--){
		list[k+1].x1=list[k].x1; // shift everything to the right by 1
		list[k+1].x2 = list[k].x2;
		list[k+1].y1 = list[k].y1;
		list[k+1].y2 = list[k].y2;
		list[k+1].r = list[k].r;
		list[k+1].b = list[k].b;
		list[k+1].g = list[k].g;
		list[k+1].s = list[k].s;
		list[k+1].t = list[k].t;
		list[k+1].radius = list[k].radius;
	}
	list[0].x1=list[selected+1].x1;	//selected object is inserted to position 0
	list[0].x2 = list[selected+1].x2;
	list[0].y1 = list[selected+1].y1;
	list[0].y2 = list[selected+1].y2;
	list[0].r = list[selected+1].r;
	list[0].b = list[selected+1].b;
	list[0].g = list[selected+1].g;
	list[0].s = list[selected+1].s;
	list[0].t = list[selected+1].t;
	list[0].radius = list[selected+1].radius;
	numObj++;
	highlight++;
	deleteHighlight();
	Delete(selected+1);			//delete the object that we sent back
	selection = 0;

	glutPostRedisplay();
	return;
}


void mouseDraw(GLint button, GLint action, GLint xMouse, GLint yMouse) {

if (button == GLUT_LEFT_BUTTON) {

	if (action == GLUT_DOWN && button != GLUT_RIGHT_BUTTON && numObj < MaxNumObj -1) {

		if (selection == 1 && move==1){// if user clicks while in select and move mode, we set the mouse position as long as it is inside object
			if (list[selected].x1 < xMouse && xMouse < list[selected].x2 && list[selected].y1 < yMouse && yMouse < list[selected].y2){
				xOld=xMouse;
				yOld=yMouse;
			}

		}else{
			list[numObj].x1 = xMouse;
			list[numObj].y1 = yMouse;
			list[numObj].x2 = xMouse;
			list[numObj].y2 = yMouse;
			list[numObj].t = type;
			list[numObj].s = style;
			list[numObj].r = red;
			list[numObj].g = green;
			list[numObj].b = blue;
		}
		glutPostRedisplay();
	} else if (action == GLUT_UP && button != GLUT_RIGHT_BUTTON) {

		if (xMouse != list[numObj].x1 && list[numObj].y1 != yMouse ){ // make sure this only gets called as long as mouse has moved positions, this way we dont have to deal with objects getting drawn everytime user clicks left mouse
			list[numObj].x2 = xMouse;
			list[numObj].y2 = yMouse;
			if (type == 2){
				list[numObj].radius = sqrt(pow((list[numObj].x1-list[numObj].x2),2) + pow(list[numObj].y1-list[numObj].y2,2)); // calculate radius
			}
			numObj++;
		}
		glutPostRedisplay();

	}
	if (selection ==1 && action == GLUT_UP  && button != GLUT_RIGHT_BUTTON){ // logic for when user is in selection mode and selects object

		select(button, action, xMouse, yMouse);
		glutPostRedisplay();
	}

}
glFlush();
}

// this function takes the mouse position while moving mouse, use this for intermediate drawing
void Motion(GLint x, GLint y) {

	if (selection == 1 && move==1){
		if(list[selected].t==1){ // when mouse is inside object, in move mode, (mouse button held down) we find the difference between the current position of the mouse with the old position found earlier and re-draw the shape
			if (list[selected].x1 < x && x < list[selected].x2 && list[selected].y1 < y && y < list[selected].y2 && xOld!=0 && yOld!=0){
				list[selected].x1 = (x-xOld)+list[selected].x1;
				list[selected].y1 = (y-yOld)+list[selected].y1;
				list[selected].x2 = (x-xOld)+list[selected].x2;
				list[selected].y2 = (y-yOld)+list[selected].y2;
				xOld=x;
				yOld=y;
			}
		}
		if (list[selected].t==2){
			GLfloat d = sqrt(pow(x-list[selected].x1,2) + pow(y-list[selected].y1,2));
			if((d<=list[selected].radius)){
				list[selected].x1 = (x-xOld)+list[selected].x1;
				list[selected].y1 = (y-yOld)+list[selected].y1;
				list[selected].x2 = (x-xOld)+list[selected].x2;
				list[selected].y2 = (y-yOld)+list[selected].y2;
				list[selected].radius = sqrt(pow(x-list[selected].x1,2) + pow(y-list[selected].y1,2));
				xOld=x;
				yOld=y;
			}
		}

	}// add the (x, y) coordinates of the second point to the intermediate rectangle
	else{
	list[numObj].x2 = x;
	list[numObj].y2 = y;
	}

// redisplay the object list after the above insertion. It will give a rubber band effect
glutPostRedisplay();
glFlush();
}

// reshape function for resized the window
void winReshapeFcn(GLint newWidth, GLint newHeight) {
/* Reset viewport and projection parameters */
glViewport(0, 0, newWidth, newHeight);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, GLdouble(newWidth), GLdouble(newHeight), 0.0);
/* Reset display-window size parameters. */
winWidth = newWidth;
winHeight = newHeight;
drawList();
glFlush();
}

// call when user clicks new on menu. this deletes all objects
void clearList(){
	int i=0;
	while(numObj!=0){
		Delete(i);
		selected = 0;
		highlight = 0;

	}
}

void mainMenu (GLint menuOption)
{
   switch (menuOption) {
   case 1: clearList(); numObj =0 ; selection = 0; move = 0; break;
   case 2: exit(0);
   }
   glutPostRedisplay ( );
}

void colorSubMenu (GLint colorOption)
{
    switch (colorOption) {
   case 1:
   {red = 0.0; green = 0.0; blue = 1.0;} break;
   case 2:
   {red = 0.0; green = 1.0; blue = 0.0;} break;
   case 3:
  {red = 1.0; green = 0.0; blue = 0.0;}break;
   case 4:
   {red = 1.0; green = 1.0; blue = 1.0;}
    }
   glutPostRedisplay ( );
}

void shapeSubMenu (GLint shapeOption)
{
    switch (shapeOption) {
   case 1:
   {type = 1;} break;
   case 2:
   {type = 2;} break;
   case 3:
  {type = 3;}
    }
   glutPostRedisplay ( );
}

void stylesSubMenu (GLint stylesOption)
{
    switch (stylesOption) {
   case 1:
   {style = 1;} break;
   case 2:
   {style = 2;}
    }
   glutPostRedisplay ( );
}



void editSubMenu (GLint editOption)
{
    switch (editOption) {
   case 1:
   {selection = 1;} break;
   case 2:
   {selection = 0;
   deselect();} break;

   case 3:
  {deleteHighlight();
  Delete(selected);
  selection =0;
  selected=0;
  } break;
   case 4:
   {if(selection == 1){
	   bringToFront(selected);}} break;

   case 5:
   {if(selection == 1){
	   sendToBack(selected);}} break;
   case 6:
   {move=1;}
    }
   glutPostRedisplay ( );
}


void svgOutput ()
{
    FILE *outFile;
    outFile = fopen("output.xml", "w");
    char tmp[500];
    int objWidth, objHeight; //store calculated height and width

    //write header data for SVG
    sprintf(tmp, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
    fputs(tmp, outFile);
    sprintf(tmp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
    fputs(tmp, outFile);
    sprintf(tmp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
    fputs(tmp, outFile);
    sprintf(
            tmp,
            "<svg width=\"%d\" height=\"%d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n",
            winWidth, winHeight);
    fputs(tmp, outFile);

    int topX, topY;
    for (int i = 0; i < numObj; i++) {

    	if (list[i].t == 1) {
			if (list[i].x2 >= list[i].x1)
			{
				objWidth = list[i].x2 - list[i].x1;
				topX = list[i].x1;
			} else
			{
				objWidth = list[i].x1 - list[i].x2;
				topX = list[i].x2;
			}

			if (list[i].y2 >= list[i].y1)
			{
				objHeight = list[i].y2 - list[i].y1;
				topY = list[i].y1;
			} else
			{
				objHeight = list[i].y1 - list[i].y2;
				topY = list[i].y2;
			}
			if (list[i].t == 1){
				if (list[i].s == 1) {
					sprintf(
							tmp,
							"<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:3;stroke:rgb(%d,%d,%d)\"/>\n",
							topX, topY, objWidth, objHeight,
							(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255),
							(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
					fputs(tmp, outFile);
				}
				if (list[i].s == 2){
					sprintf(
					tmp,
					"<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:3;stroke:rgb(%d,%d,%d)\"/>\n",
					topX, topY, objWidth, objHeight,
					(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
					fputs(tmp, outFile);
				}

			}
    	}
		if (list[i].t == 2){
			if (list[i].s == 1){
				sprintf(tmp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:3;stroke:rgb(%d,%d,%d)\"/>\n",
						(int) list[i].x1, (int) list[i].y1, (int) list[i].radius,
						(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255),
						(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}
			if (list[i].s == 2){
				sprintf(tmp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:3;stroke:rgb(%d,%d,%d)\"/>\n",
						(int) list[i].x1, (int) list[i].y1, (int) list[i].radius,
						(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}

		}
		if (list[i].t == 3){
			sprintf(tmp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke-width:2;stroke:rgb(%d,%d,%d)\"/>\n",
			(int) list[i].x1, (int) list[i].y1, (int) list[i].x2, (int) list[i].y2,
			(int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
			fputs(tmp, outFile);
		}


    }
    fclose(outFile);

}

void svgInput(){
	FILE *in;
	char int_chr;
	char prev;
	clearList();
	numObj = 0;
	selection = 0;
	move = 0;

	glutPostRedisplay();

	in = fopen("output.xml", "r");


	do {
		prev = int_chr;
		int_chr = fgetc(in);

		if (int_chr == 'r' && prev == '<') {
			//parse rect

			numObj++;
			list[numObj].t = 1;
			float r1, g1, b1, r2, g2, b2;

			fscanf(in, "ect x=\" %d \" y=\" %d \" width=\" %d \" height=\" %d \" style=\"fill:rgb( %f , %f , %f );stroke-width:3;stroke:rgb( %f , %f , %f )",
					&list[numObj].x1, &list[numObj].y1, &list[numObj].x2,
					&list[numObj].y2,
					&r1, &g1, &b1,
					&r2, &g2, &b2);
			list[numObj].x2 += list[numObj].x1;
			list[numObj].y2 += list[numObj].y1;
			if (r1 == 255 && b1 == 255 && g1 == 255 && !(r2 == 255 && b2 == 255 && g2 == 255)){
				list[numObj].r = r2;
				list[numObj].g = g2;
				list[numObj].b = b2;
				list[numObj].s = 2;

			}else{
				list[numObj].r = r1;
				list[numObj].g = g1;
				list[numObj].b = b1;
				list[numObj].s = 1;
			}

			//convert colors back to float
			list[numObj].r = list[numObj].r / 255;
			list[numObj].g = list[numObj].g / 255;
			list[numObj].b = list[numObj].b / 255;

			printf("rect x1=%d, y1=%d, x2=%d, y2=%d r=%f g=%f b=%f s=%d t=%d\n", list[numObj].x1, list[numObj].y1, list[numObj].x2, list[numObj].y2, list[numObj].r, list[numObj].g, list[numObj].b, list[numObj].s, list[numObj].t);
		}
		if (int_chr == 'c' && prev == '<'){
			numObj++;
			list[numObj].t = 2;
			float r1, g1, b1, r2, g2, b2;

			fscanf(in, "ircle cx=\" %d \" cy=\" %d \" r=\" %d \" style=\"fill:rgb( %f , %f , %f );stroke-width:3;stroke:rgb( %f , %f , %f )",
					&list[numObj].x1, &list[numObj].y1, &list[numObj].radius,
					&r1, &g1, &b1,
					&r2, &g2, &b2);
			list[numObj].x2 = list[numObj].x1 + list[numObj].radius;
			list[numObj].y2 = list[numObj].y1;
			if (r1 == 255 && b1 == 255 && g1 == 255 && !(r2 == 255 && b2 == 255 && g2 == 255)){
				list[numObj].r = r2;
				list[numObj].g = g2;
				list[numObj].b = b2;
				list[numObj].s = 2;

			}else{
				list[numObj].r = r1;
				list[numObj].g = g1;
				list[numObj].b = b1;
				list[numObj].s = 1;
			}

			//convert colors back to float
			list[numObj].r = list[numObj].r / 255;
			list[numObj].g = list[numObj].g / 255;
			list[numObj].b = list[numObj].b / 255;

			printf("circle x1=%d, y1=%d, x2=%d, y2=%d r=%f g=%f b=%f s=%d t=%d\n", list[numObj].x1, list[numObj].y1, list[numObj].x2, list[numObj].y2, list[numObj].r, list[numObj].g, list[numObj].b, list[numObj].s, list[numObj].t);
		}
		if (int_chr == 'l' && prev == '<') {
			//parse rect

			numObj++;
			list[numObj].t = 3;

			fscanf(in, "ine x1=\" %d \" y1=\" %d \" x2=\" %d \" y2=\" %d \" style=\"stroke-width:2;stroke:rgb( %f , %f , %f )",
					&list[numObj].x1, &list[numObj].y1, &list[numObj].x2,
					&list[numObj].y2,
					&list[numObj].r, &list[numObj].g, &list[numObj].b);

			//convert colors back to float
			list[numObj].r = list[numObj].r / 255;
			list[numObj].g = list[numObj].g / 255;
			list[numObj].b = list[numObj].b / 255;

			printf("line x1=%d, y1=%d, x2=%d, y2=%d r=%f g=%f b=%f t=%d\n", list[numObj].x1, list[numObj].y1, list[numObj].x2, list[numObj].y2, list[numObj].r, list[numObj].g, list[numObj].b, list[numObj].t);

		}

	} while  (int_chr != EOF);

	    fclose(in);
	    numObj++;

	}


#define GL_BGR_EXT 0x80E0
int SaveBitmap(const char *ptrcFileName, int nX, int nY, int nWidth, int nHeight)
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    unsigned char *ptrImage = (unsigned char*) malloc(sizeof(unsigned char)*nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight);


     FILE *ptrFile = fopen(ptrcFileName, "wb");

	//read pixels from framebuffer
	glReadPixels(nX, nY, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, ptrImage );


	// set memory buffer for bitmap header and informaiton header
	 memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));

   // configure the headers with the give parameters

	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(bf) + sizeof(bi) + nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;
	bf.bfOffBits = sizeof(bf) + sizeof(bi);
	bi.biSize = sizeof(bi);
	bi.biWidth = nWidth + nWidth%4;
	bi.biHeight = nHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biSizeImage = nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;


// to files
   fwrite(&bf, sizeof(bf), 1, ptrFile);
	fwrite(&bi, sizeof(bi), 1, ptrFile);
	fwrite(ptrImage, sizeof(unsigned char), nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight, ptrFile);

	fclose(ptrFile);
	free(ptrImage);


}

void filesSubMenu (GLint filesOption)
{
    switch (filesOption) {
   case 1:
   {svgOutput();} break;
   case 2:
   {svgInput();} break;
   case 3:

  {SaveBitmap("image.bmp", 0, 0, winWidth, winHeight);}
    }
   glutPostRedisplay ( );
}

// main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);

// declare submenu variables and set menu here

    int shape_subMenu;
    shape_subMenu = glutCreateMenu (shapeSubMenu);
    glutAddMenuEntry ("Rectangle", 1);
    glutAddMenuEntry ("Circle", 2);
    glutAddMenuEntry ("Line", 3);

    int edit_subMenu;
    edit_subMenu = glutCreateMenu (editSubMenu);
    glutAddMenuEntry ("Select", 1);
    glutAddMenuEntry ("Deselect", 2);
    glutAddMenuEntry ("Delete", 3);
    glutAddMenuEntry ("Bring to front", 4);
    glutAddMenuEntry ("Send to back", 5);
    glutAddMenuEntry ("Move", 6);

    int styles_subMenu;
    styles_subMenu = glutCreateMenu (stylesSubMenu);
    glutAddMenuEntry ("Filled", 1);
    glutAddMenuEntry ("Outline", 2);


    int files_subMenu;
    files_subMenu = glutCreateMenu (filesSubMenu);
    glutAddMenuEntry ("Save SVG File", 1);
    glutAddMenuEntry ("Open SVG File", 2);
    glutAddMenuEntry ("Export to Bitmap", 3);

    int color_subMenu;
    color_subMenu = glutCreateMenu (colorSubMenu);
    glutAddMenuEntry ("Red", 3);
    glutAddMenuEntry ("Green", 2);
    glutAddMenuEntry ("Blue", 1);
    glutAddMenuEntry ("White", 4);

    glutCreateMenu (mainMenu); // Create main pop-up menu.
    glutAddMenuEntry (" New ", 1);
    glutAddSubMenu (" Colors ", color_subMenu);
    glutAddSubMenu (" Draw ", shape_subMenu);
    glutAddSubMenu (" Edit ", edit_subMenu);
    glutAddSubMenu (" Styles ", styles_subMenu);
    glutAddSubMenu (" Files ", files_subMenu);
    glutAddMenuEntry (" Quit", 2);


    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("SimpleDraw Sample for Assignment 1 by Khaja");

    init();

    // register call back funtions
    glutDisplayFunc(drawList);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(mouseDraw);
    glutMotionFunc(Motion);


    //add right click menu
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
}
