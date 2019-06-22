#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

typedef struct operation {
	float x;
	float y;
	float z;
} * Operation;

float scaling;
Operation rotation;
Operation translation;

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

/* A ---- B
   |      |
   |      |
   C ---- D
*/
void drawPyramid() {
  float aX, aZ, bX, bZ, cX, cZ, dX, dZ, eY;
  aX = cX = -1;
  bX = dX = 1;
  aZ = bZ = -1;
  cZ = dZ = 1;
  eY = 3;
  // base
  glColor3f(0,0,0.1);
  glBegin(GL_TRIANGLES);
    glVertex3f(cX,0,cZ);
    glVertex3f(aX,0,aZ);
    glVertex3f(bX,0,bZ);
    glVertex3f(bX,0,bZ);
    glVertex3f(dX,0,dZ);
    glVertex3f(cX,0,cZ);
  glEnd();

  // faces
  glBegin(GL_TRIANGLES);
    glColor3f(0,0,0.25);
    glVertex3f(cX,0,cZ);
    glVertex3f(0,eY,0);
    glVertex3f(aX,0,aZ);
    glColor3f(0,0,0.5);
    glVertex3f(aX,0,aZ);
    glVertex3f(0,eY,0);
    glVertex3f(bX,0,bZ);
    glColor3f(0,0,0.75);
    glVertex3f(bX,0,bZ);
    glVertex3f(0,eY,0);
    glVertex3f(dX,0,dZ);
    glColor3f(0,0,1);
    glVertex3f(dX,0,dZ);
    glVertex3f(0,eY,0);
    glVertex3f(cX,0,cZ);
  glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	float p[4] = {0.0f, 1.0f, 0.0f, 1.0f};

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(1,0,0);
	glutWireSphere(0.1,10,10);
	glPopMatrix();
	gluLookAt(10.0,0.0,5.0,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	glPushMatrix();
	glTranslatef(p[0],p[1],p[2]);
	glColor3f(0,1,0);
	glutWireSphere(1,10,10);
	glPopMatrix();

// put the geometric transformations here
/*
// put drawing instructions here
  drawPyramid();*/


	// End of frame
	glutSwapBuffers();
	glutPostRedisplay();
}


// Initialize global variables
void init() {
	rotation = (Operation) malloc(sizeof(struct operation));
	rotation->x = 0.0f;
    rotation->y = 0.0f;
    rotation->z = 0.0f;

    translation = (Operation) malloc(sizeof(struct operation));
    translation->x = 0.0f;
    translation->y = 0.0f;
    translation->z = 0.0f;

    scaling = 1;
}

void specialKeyboard(int key_code, int x, int y) {
  switch (key_code) {
    // Rotate over x axis
    case GLUT_KEY_F1:
      rotation->x += 30.0f;
      break;
    case GLUT_KEY_F2:
      rotation->x -= 30.0f;
      break;
    // Rotate over y axis
    case GLUT_KEY_F3:
      rotation->y += 30.0f;
      break;
    case GLUT_KEY_F4:
      rotation->y -= 30.0f;
      break;
    // Rotate over z axis
    case GLUT_KEY_F5:
      rotation->z += 30.0f;
      break;
    case GLUT_KEY_F6:
      rotation->z -= 30.0f;
      break;
    // Translation over z axis
    case GLUT_KEY_UP:
      translation->z -= 0.1;
      break;
    case GLUT_KEY_DOWN:
      translation->z += 0.1;
      break;
    // Translation over x axis
    case GLUT_KEY_LEFT:
      translation->x -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      translation->x += 0.1;
      break;
    // Scaling over y axis
    case GLUT_KEY_PAGE_UP:
      scaling += 0.1;
      break;
    case GLUT_KEY_PAGE_DOWN:
      scaling -= 0.1;
      break;
  }
  glutPostRedisplay();
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

	
// put here the registration of the keyboard callbacks
    glutSpecialFunc(specialKeyboard);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
