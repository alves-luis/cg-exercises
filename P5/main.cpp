#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;



void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawTree(float x, float z) {
	glPushMatrix();
	glTranslatef(x,0,z);
	glRotatef(-90,1,0,0);
	glColor3f(0.3f,0.2f,0.0f);
	glutSolidCone(3,20,50,50);
	glTranslatef(0,0,15);
	glColor3f(0.6f,1.0f,0.1f);
	glutSolidCone(10,30,50,50);
	glPopMatrix();
}

void drawTorus() {
	glPushMatrix();
	glColor3f(0.7f,0.1f,0.5f);
	glutSolidTorus(3,5,50,50);
	glPopMatrix();
}

void drawTeapot(float rad, float ang, float colorR, float colorG, float colorB) {
	glPushMatrix();
	glColor3f(colorR,colorG,colorB);
	glRotatef(ang,0,1,0);
	glTranslatef(rad,1,0);
	glutSolidTeapot(2);
	glPopMatrix();
}

void drawCircleTeapot(float rad, float colorR, float colorG, float colorB) {
	float babyStep = 360.0f / 8;
	for(int i = 0; i < 8; i++) {
		drawTeapot(rad,i*babyStep,colorR,colorG,colorB);
	}
}

void drawRandomTrees(int numOfTrees) {
	srand(35);
	for(int i = 0; i < numOfTrees; i++) {
		int x = rand() % 200;
		int z = rand() % 200;
		if (x > 100)
			x = 0 - (x % 100);
		if (z > 100)
			z = 0 - (z % 100);
		if (x < 35 && x > -35 && z < 35 && z > -35) {
			x = 35 + (rand() % 65);
			z = 35 + (rand() % 65);
		}
		drawTree(x*1.0f,z*1.0f);
	}

}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	drawCircleTeapot(15,0.3f,0.2f,0.7f);
	drawCircleTeapot(35,0.6f,0.1f,0.0f);
	drawTorus();
	drawRandomTrees(50);
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
