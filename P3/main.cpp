#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

typedef struct camera {
	float beta;
	float alfa;
	float radius;
	bool inverted;
} * Camera;
Camera cam;

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


void drawBase(float x, float y, float z, float r, float ang1, float ang2) {
	glColor3f(0,1,0);
	glVertex3f(0,y,0);
	x = r*cos(ang1);
	z = r*sin(ang1);
	glVertex3f(x,y,z);
	x = r*cos(ang2);
	z = r*sin(ang2);
	glVertex3f(x,y,z);
}

void drawAround(int step, float r, float ang, float h) {
	float x,y,z;
	glColor3f(1,0,1);
	// right triangle
	x = r*cos(ang*step);
	y = h/2;
	z = r*sin(ang*step);
	glVertex3f(x,y,z);
	x = r*cos(ang*(step+1));
	z = r*sin(ang*(step+1));
	glVertex3f(x,y,z);
	x = r*cos(ang*step);
	y = 0 - h/2;
	z = r*sin(ang*step);
	glVertex3f(x,y,z);
	// left triangle
	x = r*cos(ang*(step+1));
	y = h/2;
	z = r*sin(ang*(step+1));
	glVertex3f(x,y,z);
	y = 0 - h/2;
	glVertex3f(x,y,z);
	x = r*cos(ang*step);
	z = r*sin(ang*step);
	glVertex3f(x,y,z);
}

void drawCylinder(float radius, float height, int slices) {

// put code to draw cylinder in here
	glBegin(GL_TRIANGLES);
	float step = M_PI*2 / slices;
	float xCenter = 0;
	float zCenter = 0;
	float x,y,z;
		for(int i = 0; i < slices; i++) {
			y = height / 2;
			drawBase(x,y,z,radius,step*(i+1),step*i);
			y = 0 - y;
			drawBase(x,y,z,radius,step*i,step*(i+1));
			drawAround(i,radius,step,height);
		}
	glEnd();

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	float x = cam->radius*cos(cam->beta)*sin(cam->alfa);
	float y = cam->radius*sin(cam->beta);
	float z = cam->radius*cos(cam->beta)*cos(cam->alfa);
	gluLookAt(x,y,z,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {
	switch(key) {
	  case GLUT_KEY_UP:
		if (cam->beta > M_PI/2) {
			cam->inverted = true;
			cam->alfa = M_PI - cam->alfa;
		}
		if (cam->beta < ((0 - M_PI) / 2)) {
			cam->inverted = false;
			cam->alfa = M_PI - cam->alfa;
		}
		if (cam->inverted) {
			if (cam->beta - 0.1 < (0 - M_PI) / 2)
		}
		else
			cam->beta += 0.1;
		// need to limit beta from -pi/2 to pi/2
		break;
	  case GLUT_KEY_DOWN:
	  	if (cam->beta < (0- M_PI)/2)
	  		cam->beta += 0.1;
	  	else
	  		cam->beta -= 0.1;
	  	break;
	  case GLUT_KEY_LEFT:
	  	cam->alfa = cam->alfa + 0.1;
	  	break;
	  case GLUT_KEY_RIGHT:
	  	cam->alfa = cam->alfa - 0.1;
	  	break;
	}
	glutPostRedisplay();
}

void init() {
	cam = (Camera) malloc(sizeof(struct camera));
	cam->beta = 0;
	cam->alfa = 0;
	cam->radius = 5;
	cam->inverted = false;
}


int main(int argc, char **argv) {
	init();
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
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
