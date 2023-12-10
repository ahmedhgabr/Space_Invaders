#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <iostream>
#include <Windows.h>
#include<mmsystem.h>
#include <string>
#include <string.h>
#pragma comment(lib,"winmm.lib")

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint tex1;
char title[] = "Give Me Space";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 500;

//==============================
//  Animation var
//==============================

//time ( affect the background)
double myTime = 0;

//light
double lightX = 15;
bool changeDir = false;

//player
bool povFlag = false;

//player 
double playerX = 0;
double playerY = 0;
double playerZ = 20;

//alien
double alienY = 1;
bool alienD = true;

//cyrstal
double cyrstalR = 1;
bool cyrstalD = true;

//metal 
double collectY = 2;

//score
int score = 0;
double scoreX = 0;

//obstacles
bool showObstacle1 = true;
bool showObstacle2 = true;
bool showObstacle3 = true;
bool showObstacle4 = true;

//collectable
bool showCollectable1 = true;
bool showCollectable2 = true;
bool showCollectable3 = true;
bool showCollectable4 = true;


/// ///////////////////////////


class Vector
{
public:
	GLdouble x, y, z;
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};



int cameraZoom = 0;

///////////////////////
// camera
////////////////////////
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(const Vector3f& v) const {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(const Vector3f& v) const {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) const {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) const {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() const {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(const Vector3f v) const {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 00.0f, float eyeY = 10.0f, float eyeZ = 40.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	/*void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}*/

	/*void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}*/

	void frontView() {
		eye.x = 0.704500; eye.y = 0.516275; eye.z = 1.992220;
		center.x = 0.733254; center.y = 0.547967; center.z = 0.993137;
		up.x = -0.018030; up.y = 0.999351; up.z = -0.031182;
	}

	void topView() {
		eye.x = 0.814365; eye.y = 2.094872; eye.z = 0.634981;
		center.x = 0.786670; center.y = 1.113113; center.z = 0.446877;
		up.x = 0.018926; up.y = 0.187628; up.z = -0.982058;
	}

	void sideView() {
		eye.x = -0.000570; eye.y = 0.315733; eye.z = 0.507410;
		center.x = 0.996359; center.y = 0.257254; center.z = 0.559495;
		up.x = 0.059884; up.y = 0.997871; up.z = -0.025836;
	}

	void reset() {
		eye.x = 0;
		eye.y = 10;
		eye.z = 40;

		center.x = 0;
		center.y = 0;
		center.z = 0;

	}
	void firstPerson() {
		eye.x = playerX + 1;
		eye.y = 4;
		eye.z = playerZ -1;

		center.x = playerX + 1;
		center.y = 0;
		center.z = playerZ -10  ;


		//glutPostRedisplay();
	}

	void thirdPerson() {
		eye.x = playerX+1;
		eye.y = 8;
		eye.z = playerZ +12;

		center.x = playerX+1;
		center.y = 0;
		center.z = playerZ-5;


		//glutPostRedisplay();
	}

	void pov() {
		if (povFlag) {
			firstPerson();
		}
		else {
			thirdPerson();
		}
	}


	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
		/*printf("%f ", eye.x); printf("%f ", eye.y); printf("%f \n", eye.z);
		printf("%f ", center.x); printf("%f ", center.y); printf("%f \n", center.z);
		printf("%f ", up.x); printf("%f ", up.y); printf("%f \n\n", up.z);*/
	}
};

Camera camera;


//////////////////////////////////////

//lvl var
int lvl = 1;

// Model Variables lvl1
Model_3DS model_player;
Model_3DS model_score;
Model_3DS model_mars;
Model_3DS model_stone;
Model_3DS model_stones;
Model_3DS model_Shuttle;
Model_3DS model_debris;

//Model_3DS model_alien;

// Model Variables lvl2
Model_3DS model_moon; // background
Model_3DS model_cyrstal; // collectable
Model_3DS model_rocket; // target
Model_3DS model_alien; // obstacle

// Textures
GLTexture tex_ground_Moon;
GLTexture tex_ground_Mars;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{

	double red = 1 ;
	double green =1;
	double blue = 1;
	
	if (lvl == 1) {
		 red =  0.5;
		 green =  0.5;
		 blue =  0.5;
	}
	else if (lvl == 2) {
		red = 1;
		green = 1;
		blue = 1;
	}
	GLfloat lightIntensity[] = {red - myTime/100, green - myTime / 100, blue - myTime / 100, 1.0f};
	GLfloat lightPosition[] = { 20.0f,40.0f, 40.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
	//enable lighing and glight0 in main
	//	change intensity in 2nd level
	//	Moving light source :

	double rL = 0;
	double gL = 0;
	double bL = 0;
	double yL = 5;
	if (lvl == 1) {
		 rL = 1;
		 gL = 0.9;
		 bL = 0;
		 yL = 5;
	}
	else if (lvl == 2) {
		rL = 1;
		gL = 0;
		bL = 0;
		yL = 2;
	}
	GLfloat lightIntensity2[] = {rL, gL, bL, 1.0f};
	GLfloat lightPosition2[] = { lightX,yL, -5.0f, 1.0f };
	GLfloat l2Direction[] = { 0.0, -1.0, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity2);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	camera.look();
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	if (lvl == 1) {
		glBindTexture(GL_TEXTURE_2D, tex_ground_Moon.texture[0]);	// Bind the ground texture
	}
	if (lvl == 2) {
		glBindTexture(GL_TEXTURE_2D, tex_ground_Mars.texture[0]);
	}

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}




//=======================================================================
// Draw Functions
//=======================================================================

void drawPlayer(double x,  double z) {	
	glPushMatrix();
	glTranslatef(x, playerY,z );
	glRotatef(180, 0, 1, 0);
	glScalef(0.007, 0.007, 0.007);
	model_player.Draw();
	glPopMatrix();
}

void drawCollectable(double x,  double z) {
	
	if (lvl == 1) {
		glPushMatrix();
		glTranslatef(x, 0, z);
		//glScalef(0.03, 0.03, 0.03);
		model_stone.Draw();
		glPopMatrix();

		/*glPushMatrix();
		glTranslatef(x, 0, z);
		glScalef(0.015, 0.015, 0.015);
		model_stones.Draw();
		glPopMatrix();*/

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Green color with transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLUquadric* quadric = gluNewQuadric();

		glPushMatrix();
		glTranslatef(x, 0.7, z); // Adjust the position of the sphere
		gluSphere(quadric, cyrstalR, 32, 32); // Draw a sphere with radius 1.0
		glPopMatrix();

		gluDeleteQuadric(quadric);
		glDisable(GL_BLEND);
	}
	else if (lvl == 2) {
		
		glPushMatrix();
		glTranslatef(x, 0, z);
		glScalef(0.5, 0.5, 0.5);
		model_cyrstal.Draw();
		glPopMatrix();


		glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Green color with transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLUquadric* quadric = gluNewQuadric();

		glPushMatrix();
		glTranslatef(x, 0.7, z); // Adjust the position of the sphere
		gluSphere(quadric, cyrstalR, 32, 32); // Draw a sphere with radius 1.0
		glPopMatrix();

		gluDeleteQuadric(quadric);
		glDisable(GL_BLEND);
	}
	
}

void drawObstacle(double x,  double z) {

	if (lvl == 1) {
		glPushMatrix();
		glTranslatef(x, 0, z);
		glScalef(0.008, 0.02, 0.008);
		model_debris.Draw();
		glPopMatrix();
	}
	else if (lvl == 2) {
		glPushMatrix();
		glTranslatef(x, alienY, z);
		glScalef(0.03, 0.03, 0.03);
		model_alien.Draw();
		glPopMatrix();
	}
}

void drawTarget(double x, double z) {
	if (lvl == 1) {
		glPushMatrix();
		glTranslatef(x, 0, z);
		//glScalef(0.03, 0.03, 0.03);
		model_Shuttle.Draw();
		glPopMatrix();
	}
	else if (lvl == 2) {
		glPushMatrix();
		glTranslatef(x, 0, z);
		//glRotatef(90.f, 1, 0, 0);
		glScalef(20, 20, 20);
		model_rocket.Draw();
		glPopMatrix();
	}
}


void drawBackground() {
	double x_bg = -myTime-5;
	double z_bg = (x_bg * x_bg) + 10;
	if (lvl == 1) {
		// Draw moon Model in background
		glPushMatrix();
		glTranslatef(-20+myTime, 10, -50- myTime);
		glRotatef(myTime*20, 0, 1, 0);
		glScalef(0.08, 0.08, 0.08);
		model_mars.Draw();
		glPopMatrix();
	}
	else if (lvl == 2) {
		// Draw moon Model in background
		glPushMatrix();
		glTranslatef(-20+myTime, 10, -50-myTime);
		glRotatef(myTime * 30, 0, 1, 0);
		glScalef(0.08, 0.08, 0.08);
		model_moon.Draw();
		glPopMatrix();
	}
	

}

void print(int x, int y, int z, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y, z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	if (lvl > 2) {
		if (score > 0) {
			
			myTime = 0;
			glColor3f(1.0f, 1.0f, 1.0f); // Green color for the text
			glPushMatrix();
			glTranslatef(0.0 - myTime / 5.0, 0.0, 0.0); // Adjust the position of the text
			glScalef(0.05, 0.05, 0.05); // Adjust the size of the text
			char* string = "Winner.Winner.Winner. give the dev a bouns.";
			while (*string) {
				glutStrokeCharacter(GLUT_STROKE_ROMAN, *string++);
			}
			glPopMatrix();
			
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f); // Green color for the text
			glPushMatrix();
			glTranslatef(0.0 - myTime / 5.0, 0.0, 0.0); // Adjust the position of the text
			glScalef(0.005, 0.005, 0.005); // Adjust the size of the text
			char* string = "Game Over. Game Over. Game Over. Game Over. give the dev a bouns.";
			while (*string) {
				glutStrokeCharacter(GLUT_STROKE_ROMAN, *string++);
			}
			glPopMatrix();
		}
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else {
		InitLightSource();
		// Draw Ground
		RenderGround();
		drawBackground();

		// player ( robot)
		drawPlayer(playerX, playerZ);

		//Target ( lvl 1 -> space station , lvl 2 -> rocket)
		drawTarget(15, -10);

		//Collectable ( lvl 1 -> , lvl 2 -> cyrstal)
		if (showCollectable1)
			drawCollectable(-14, -15);
		if (showCollectable2)
			drawCollectable(-3, 5);
		if (showCollectable3)
			drawCollectable(4, -10);
		if (showCollectable4)
			drawCollectable(16, 10);

		//Obstacle (lvl 1 -> metal , lvl 2 -> alien )
		if (showObstacle1)
			drawObstacle(-8, -7);
		if (showObstacle2)
			drawObstacle(-2, 3);
		if (showObstacle3)
			drawObstacle(7, 0);
		if (showObstacle4)
			drawObstacle(14, 8);


		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		//score
		glPushMatrix();
		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "Score: %d", score);
		print(scoreX, 0, -16, (char*)p0s);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1, 0, -16.3);
		glScalef(7, 1, 2);
		model_score.Draw();
		glPopMatrix();
	}
	

	glutSwapBuffers();
}


//=======================================================================
// Animation Function
//=======================================================================
void Anim() {

	// animate moon/mars in background
	myTime += 0.0001;


	//light
	if (changeDir) {
		lightX -= 0.01;
		if (lightX <= 12)
			changeDir = false;
	}
	else {
		lightX += 0.01;
		if (lightX >= 18)
			changeDir = true;
	}

	// animate lvl 2 aliens
	if (alienD) {
		alienY += 0.002;
		if (alienY > 1.2) {
			alienD = false;
		}
	}
	else {
		alienY -= 0.003;
		if (alienY < 0.7) {
			alienD = true;
		}
	}

	// animate lvl 2 collectable
	double lvlVar = 2 / lvl;
	if (cyrstalD) {
		cyrstalR += 0.002 /lvlVar ;
		if (cyrstalR > 1.5 * lvlVar ) {
			cyrstalD = false;
		}
	}
	else {
		cyrstalR -= 0.001 / lvlVar;
		if (cyrstalR < 1 * lvlVar) {
			cyrstalD = true;
		}
	}

	glutPostRedisplay();
	//glutTimerFunc(100, Anim, 0);
}

void handleMove() {
	

	//Collectable collision
	if (showCollectable1 && playerX >= -17 && playerX <= -13 && playerZ >= -17 && playerZ <= -13) {
		showCollectable1 = false;
		score += 20;
		if (lvl == 1) {
			PlaySound(TEXT("sound/rock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if(lvl == 2){
			PlaySound(TEXT("sound/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		
	}
	if (showCollectable2 && playerX >= -6 && playerX <= -3 && playerZ >= 3 && playerZ <= 7) {
		showCollectable2 = false;
		score += 20;
		if (lvl == 1) {
			PlaySound(TEXT("sound/rock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	if (showCollectable3 && playerX >= 2 && playerX <= 4 && playerZ >= -12 && playerZ <= -8) {
		showCollectable3 = false;
		score += 20;
		if (lvl == 1) {
			PlaySound(TEXT("sound/rock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	if (showCollectable4 && playerX >= 14 && playerX <= 16 && playerZ >= 8 && playerZ <= 12) {
		showCollectable4 = false;
		score += 20;
		if (lvl == 1) {
			PlaySound(TEXT("sound/rock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}

	//Obstacle collision
	if (showObstacle1 && playerX >= -11 && playerX <= -8 && playerZ >= -9 && playerZ <= -6) {
		showObstacle1 = false;
		score -= 30;
		playerZ += 5;
		if (lvl == 1) {
			PlaySound(TEXT("sound/crash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/alienCrash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	if (showObstacle2 && playerX >= -5 && playerX <= -2 && playerZ >= 1 && playerZ <= 4) {
		showObstacle2 = false;
		score -= 30;
		playerZ += 5;
		if (lvl == 1) {
			PlaySound(TEXT("sound/crash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/alienCrash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	if (showObstacle3 && playerX >= 4 && playerX <= 7 && playerZ >= -2 && playerZ <= 1) {
		showObstacle3 = false;
		score -= 30;
		playerZ += 5;
		if (lvl == 1) {
			PlaySound(TEXT("sound/crash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/alienCrash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	if (showObstacle4 && playerX >= 11 && playerX <= 14 && playerZ >= 6 && playerZ <= 9) {
		showObstacle4 = false;
		score -= 30;
		playerZ += 5;
		if (lvl == 1) {
			PlaySound(TEXT("sound/crash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 2) {
			PlaySound(TEXT("sound/alienCrash.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}

	//target collision
	if (playerX >= 13 && playerX <= 17 && playerZ >= -12 && playerZ <= -8) {
		playerX = 0;
		playerZ = 20;
		lvl++;
		if(lvl == 2)
		{
			PlaySound(TEXT("sound/nextLevel.wav"), NULL, SND_FILENAME | SND_SYNC);
		}
		else if (lvl == 3 && score > 0) {
			PlaySound(TEXT("sound/win.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (lvl == 3 && score < 0) {
			PlaySound(TEXT("sound/lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		showCollectable1 = true;
		showCollectable2 = true;
		showCollectable3 = true;
		showCollectable4 = true;
		showObstacle1 = true;
		showObstacle2 = true;
		showObstacle3 = true;
		showObstacle4 = true;

	}
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	/*printf("xx %f",playerX );
	printf("zz %f", playerZ);*/
	switch (button)
	{
	case 'w':
		if(playerZ > -18)
		{
			playerZ -= 0.4;
		}
		break;
	case 'a':
		if(playerX >-20)
		{
			playerX -= 0.4;
		}
		//scoreX -= 0.4;
		
		break;
	case 's':
		if (playerZ < 20) {
			playerZ += 0.4;
		}
		break;
		
	case 'd':
		if (playerX < 17) {
			playerX += 0.4;
		}
		//scoreX += 0.4;

		break;
	case 'p':
		camera.reset();
		glLoadIdentity();
		camera.look();
		return;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	
	handleMove();
	//PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
	camera.pov();
	glLoadIdentity();
	camera.look();
	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================

/*void myMotion(int x, int y)
{
	y = HEIGHT - y;
	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}
	cameraZoom = y;
	glLoadIdentity();	//Clear Model_View Matrix
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutPostRedisplay();	//Re-draw scene 
}*/

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		povFlag = true;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		povFlag = false;
	}
	camera.pov();
	glLoadIdentity();	//Clear Model_View Matrix
	camera.look();
	glutPostRedisplay();
}

//=======================================================================
// Reshape Function
//=======================================================================
/*void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}*/

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files

	model_player.Load("Models/robot.3ds");
	model_score.Load("Models/wall/wall.3ds");
	//level 1
	model_mars.Load("Models/Mars/moon.3DS");
	model_stone.Load("Models/rock.3ds");
	model_stones.Load("Models/Rock_Pile_Ironworks.3ds");
	model_Shuttle.Load("Models/Shuttle(rocket)/Transport Shuttle_3ds.3ds");
	model_debris.Load("Models/debris/debris.3ds");

	//level 2 
	model_moon.Load("Models/Moon/moon.3DS");
	model_rocket.Load("Models/rocket1.3ds");
	model_cyrstal.Load("Models/cyrstal.3ds");
	model_alien.Load("Models/Space_Invader.3DS");

	

	//loadBMP()
	// Loading texture files
	tex_ground_Moon.Load("Textures/moonmap2.bmp");
	tex_ground_Mars.Load("Textures/mars-texture.bmp");
	
	loadBMP(&tex, "Textures/SpaceTexture1.bmp", true);
}


//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	//glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	//glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glutIdleFunc(Anim);
	//glutTimerFunc(0, Anim, 0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}