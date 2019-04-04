

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#define ANG2RAD M_PI/180.0 

#define COWBOYS 8
#define RAIO_COWBOYS 5
#define INDIOS 16
#define RAIO_INDIOS 25
#define ARVORES 1000
#define STEP_COWBOY 1.0f
#define STEP_INDIO 0.5f


float step = 0.0;

float height = 2.0f;
float x = 0.0f;
float z = 0.0f;


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int t, tw, th;
unsigned char * imageData;

GLuint buffer[1];

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float h(int i, int j) {
	float height = imageData[th*i+j];
	return height/5;
}

float hf(float x, float z) {
	float baseX = tw / 2.0f;
	float baseZ = th / 2.0f;
	x += baseX;
	z += baseZ;
	int x1 = floor(x);
	int z1 = floor(z);
	int x2 = x1 + 1;
	int z2 = z1 + 1;

	float fz = z - z1;
	float fx = x - x1;

	float height_x1_z = h(x1,z1) * (1-fz) + h(x1,z2) * fz;
	float height_x2_z = h(x2,z1) * (1-fz) + h(x2,z2) * fz;

	float height_xz = height_x1_z * (1-fx) + height_x2_z*fx;

	return height_xz;
}

void buildGrid() {

	int minX = 0;
	int minZ = 0;
	int index = 0;
	int numOfPoints = tw * 3 * 2 * (th-1);
	float * vertexBuffer = (float *) (malloc(sizeof(float) * numOfPoints));
	for(int i = 0; i < th-1; i++) {
		for(int j = 0; j < tw; j++) {
			// a
			vertexBuffer[index++] = minX + j*1.0f; // x
			vertexBuffer[index++] = h(i,j);
			vertexBuffer[index++] = minZ + i*1.0f; // z
			// b
			vertexBuffer[index++] = minX + j*1.0f; // x
			vertexBuffer[index++] = h(i,j);
			vertexBuffer[index++] = minZ + (i + 1)*1.0f; // z + 1
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*numOfPoints,vertexBuffer,GL_STATIC_DRAW);
	free(vertexBuffer);
}

void drawPlane() {

	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColor3f(0.5f, 0.5f, 0.2f);
	float moveX = tw / 2.0f;
	float moveZ = th / 2.0f;
	glTranslatef(-moveX,0,-moveZ);
	for(int i = 0; i < th-1; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i*tw*2, 2*tw);
	glPopMatrix();

	/*glColor3f(0.2f,0.8f,0.2f);
	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-100.0, 0.0, 100.0);
		glVertex3f( 100.0, 0.0, 100.0);
		glVertex3f( 100.0, 0.0,-100.0);
		glVertex3f(-100.0, 0.0,-100.0);
	glEnd();*/
}

void drawTree() {

	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 0.5);
	glutSolidCone(0.25f, 4, 5, 1);

	glColor3f(0.0f, 0.5f + rand() * 0.5f/RAND_MAX,0.0f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glutSolidCone(2.0f, 5.0f, 5.0f, 1.0f);
	glPopMatrix();
}


void placeTrees() {

	float r = 35.0;
	float alpha;
	float rr;
	float x,z;

	srand(31457);
	int arvores = 0;

	while (arvores < ARVORES) {

		rr = rand() * 150.0/ RAND_MAX;
		alpha = rand() * 6.28 / RAND_MAX;

		x = cos(alpha) * (rr + r);
		z = sin(alpha) * (rr + r);

		if (fabs(x) < 100 && fabs(z) < 100) {

			glPushMatrix();
			glTranslatef(x,hf(x,z),z);
			drawTree();
			glPopMatrix();
			arvores++;
		}
	}
}


void drawDonut() {

	glPushMatrix();
	glTranslatef(0.0,0.5,0.0);
	glColor3f(1.0f,0.0f,1.0f);
	glutSolidTorus(0.5,1.25,8,16);
	glPopMatrix();
}


void drawIndios() {

	float angulo;
	glColor3f(1.0f,0.0f,0.0f);
	for (int i = 0; i < INDIOS; i++) {
		
		angulo = i * 360.0/INDIOS + step * STEP_INDIO;
		glPushMatrix();
		glRotatef(angulo,0.0,1.0,0.0);
		glTranslatef(0.0,0.0,RAIO_INDIOS);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}


void drawCowboys() {

	float angulo;
	glColor3f(0.0f,0.0f,1.0f);
	for (int i = 0; i < COWBOYS; i++) {
		
		angulo = i * 360.0/COWBOYS + step * STEP_COWBOY;
		glPushMatrix();
		glRotatef(-angulo,0.0,1.0,0.0);
		glTranslatef(RAIO_COWBOYS,0.0,0.0);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}


void drawScene() {

	drawPlane();
	placeTrees();
	drawDonut();
	glPushMatrix();
	// move teapots up so that they are placed on top of the ground plane
	glTranslatef(0.0,1.0,0.0);
	drawCowboys();
	drawIndios();
	glPopMatrix();
}


void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	float py = height + hf(x,z);
	gluLookAt(camX, py, camZ,
		      camX + sin(alpha),py,camZ+cos(alpha),
			  0.0f,1.0f,0.0f);

	drawScene();
	step++;

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


void init() {

// 	Load the height map "terreno.jpg"
	ilGenImages(1,&t);
	ilBindImage(t);
	ilLoadImage((ILstring) "terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	imageData = ilGetData();

	puts("Image loaded!");

// 	Build the vertex arrays
	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1,buffer);
	buildGrid();

	puts("Buffer initialized!");

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	ilInit();

	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

