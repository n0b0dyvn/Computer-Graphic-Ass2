

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include "supportClass.h"
#include "Mesh.h"
#include "Tga.h"
#include <iostream>
using namespace std;

#define		DEG2RAD(x) (x*PI)/180.0f

int ww = 640;
int wh = 480;

float	fRoomSize = 15;

int		screenWidth = 600;
int		screenHeight= 600;

bool	bAnimated = false;
bool	bWireFrame = false;

bool	b4View = false;

float	YPlanePos = 0;
float	wheelRadius = 0.6;
float	wheelWidth = 0.2;
float	frame1Height = 0.7;
float	frame3Height = 0.8;
float	ball1Radius = 0.35;
float	ball2Radius = 0.15;
float	ball2SlideZ = 0.15;

float	fRotateRadius = 2.5;

Mesh	planeY;
Mesh	wheel;
Mesh	axis;
Mesh	frame1;
Mesh	frame2;
Mesh	frame3;
Mesh	ball1;
Mesh	ball2;
Mesh	ball3;


//Control camera
float camera_angle;
float camera_height;
float camera_dis;
float camera_X, camera_Y, camera_Z;
float camera_lookAt;	

void myDisplay();


Texture   tex_yneg;
Texture   tex_ypos;

Texture   tex_xpos;
Texture   tex_xneg;

Texture   tex_zpos;
Texture   tex_zneg;

Texture	  wheelTex;
Texture   tyreTex;

void drawTexWheel();
void drawTexFloor();

void load_oneTex(char* tex,Texture* f){
	bool status = LoadTGA(&(*f), tex );

    if(status)
    {
		glGenTextures( 1, &(*f).texID );
        glBindTexture( GL_TEXTURE_2D, f->texID );

        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (*f).width, 
			(*f).height , 0, 
				GL_RGB, GL_UNSIGNED_BYTE, (*f).imageData);

		if((*f).imageData)
			free((*f).imageData);
    }
	else {
		std::cout  << "Texture "<< tex << " not loaded yet!";
		//std::cout  << "Texture not loaded yet!";

		/*TCHAR pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pwd);
		MessageBox(NULL,pwd,pwd,0);*/
	}
}


void loadTextures(void)	
{
	load_oneTex("marble.tga",&tex_yneg);
//	load
	load_oneTex("tyre.tga",&tyreTex);
	load_oneTex("wheel.tga",&wheelTex);
	load_oneTex("lobbyzpos.tga",&tex_zpos);
	load_oneTex("lobbyzneg.tga",&tex_zneg);
	load_oneTex("lobbyxpos.tga",&tex_xpos);
	load_oneTex("lobbyxneg.tga",&tex_xneg);
	load_oneTex("lobbyypos.tga",&tex_ypos);
}

void mySetupCameraVolume(int nType)
{
	
	if(nType == 4)
	{
		glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
		glLoadIdentity();						// reset projection matrix

		// calculate aspect ratio of window
		gluPerspective(60.0f,(GLfloat)screenWidth/(GLfloat)screenHeight,1.0f,1000.0f);
	}
	else if(nType == 1)
	{
		glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
		glLoadIdentity();						// reset projection matrix
		glOrtho(-5, 5, -5, 5, -1000, 1000);
	}
	else if(nType == 2)
	{
		glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
		glLoadIdentity();						// reset projection matrix
		glOrtho(-5, 5, 0, 10, -1000, 1000);
	}
	else if(nType == 3)
	{

	}
}

void changeCameraPos()
{
	camera_X = sin(DEG2RAD(camera_angle))*camera_dis+camera_lookAt;
	camera_Y = camera_height+camera_lookAt;
	camera_Z = cos(DEG2RAD(camera_angle))*camera_dis+camera_lookAt;

	myDisplay();
}

void mySpecialKeyboard(int theKey, int mouseX, int mouseY)
{
	switch (theKey)
	{
	case GLUT_KEY_UP:
		camera_height+=0.1;
		break;
	case GLUT_KEY_DOWN:
		camera_height-=0.1;
		break;
	case GLUT_KEY_RIGHT:
		camera_angle+=1;
		break;
	case GLUT_KEY_LEFT:
		camera_angle-=1;
		break;
	default:
		break;
	}
	changeCameraPos();
}

void myKeyboard(unsigned char key, int x, int y)
{
	float	fRInc;
	float	fAngle;
	
	float X=wheel.slideX;
	float Z=wheel.slideZ;
    switch(key)
    {
	case 'X':
		ball1.rotateY += 2;
		if(ball1.rotateY >= 90)
			ball1.rotateY = 90;
		break;
	case 'x':
		ball1.rotateY -= 2;
		if(ball1.rotateY <= -90)
			ball1.rotateY = -90;
		break;
	case 'Y':
		wheel.rotateY += 2;
		if(wheel.rotateY > 360)
			wheel.rotateY -= 360;
		if(wheel.rotateY < 0)
			wheel.rotateY += 360;
		break;
	case 'y':
		wheel.rotateY -= 2;
		if(wheel.rotateY > 360)
			wheel.rotateY -= 360;
		if(wheel.rotateY < 0)
			wheel.rotateY += 360;
		break;
	case 'F': //forward;
	case 'f':
		wheel.rotateX += 2;
		if(wheel.rotateX > 360)
			wheel.rotateX -= 360;
		if(wheel.rotateX < 0)
			wheel.rotateX += 360;
		fRInc = wheelRadius*(2*2*PI/360);
		
		X += fRInc*sin(wheel.rotateY*2*PI/360);
		Z += fRInc*cos(wheel.rotateY*2*PI/360);
		if (X <= fRoomSize/2 - wheelRadius/2 && X >= - fRoomSize + wheelRadius/2 && Z <= fRoomSize/2 - wheelRadius/2 && Z >= - fRoomSize + wheelRadius/2) 
		{
			wheel.slideX = X;
		wheel.slideZ = Z;
			}
		break;
	//code here
	case 'B': //forward;
	case 'b':
		wheel.rotateX -= 2;
		if(wheel.rotateX > 360)
			wheel.rotateX -= 360;
		if(wheel.rotateX < 0)
			wheel.rotateX += 360;
		fRInc = wheelRadius*(2*2*PI/360);
		X -= fRInc*sin(wheel.rotateY*2*PI/360);
		Z -= fRInc*cos(wheel.rotateY*2*PI/360);
		if (X <= fRoomSize/2 - wheelRadius/2 && X >= - fRoomSize + wheelRadius/2 && Z <= fRoomSize/2 - wheelRadius/2 && Z >= - fRoomSize + wheelRadius/2) 
		{
			wheel.slideX = X;
		wheel.slideZ = Z;
			}
		break;
	case 'R': //forward;
	case 'r':
		wheel.rotateY=wheel.slideX=wheel.slideZ=0;
		bAnimated = false;
		break;
	case 'o':
	case 'O':
		if (!bAnimated){
			wheel.rotateY=wheel.slideZ=0;
			wheel.slideX = -fRotateRadius;
		}
		bAnimated=!bAnimated;
		break;
	case 'v':
	case 'V':
		b4View=!b4View;
		changeCameraPos();
		break;
	case '+':
		camera_dis+=0.1;
		changeCameraPos();
		break;
	case '-':
		camera_dis-=0.1;
		changeCameraPos();
		break;
	case 'w':
		bWireFrame=!bWireFrame;
		break;
	case 'l':
		if (camera_lookAt>=0)
			camera_lookAt-=0.1;
		
		changeCameraPos();
		break;
	case 'L':
		if (camera_lookAt<fRoomSize  )
			camera_lookAt+=0.1;
		changeCameraPos();
		break;
	}
    glutPostRedisplay();
}

void drawFloor()
{
	//code here
	
	//
	glPushMatrix();
		glTranslated(planeY.slideX, planeY.slideY, planeY.slideZ);
		if (bWireFrame)
			planeY.DrawWireframe();
		else
			drawTexFloor();
	glPopMatrix();
}

void drawWheel()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		//////////////////////////////////
		glTranslated(0, wheel.slideY, 0);
		glRotated(wheel.rotateX, 1, 0, 0);
		glRotated(wheel.rotateZ, 0, 0, 1);
		
		if(bWireFrame)
			wheel.DrawWireframe();
		else
			drawTexWheel();
		
		//glutSolidSphere(1, 20, 20); // if you finish CreateCyclinder in Mesh.cpp
									// comment or delete this line of code.
	glPopMatrix();
}

void drawAxis()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(axis.slideX, axis.slideY, 0);
		glRotated(axis.rotateZ, 0, 0, 1);
		
		if(bWireFrame)
			axis.DrawWireframe();
		else
			axis.DrawColor();
	glPopMatrix();
}

void drawFrame1()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(frame1.slideX, frame1.slideY, frame1.slideZ);
		//glRotated(axis.rotateZ, 0, 0, 1);
		
		if(bWireFrame)
			frame1.DrawWireframe();
		else
			frame1.DrawColor();
	glPopMatrix();
}

void drawFrame2()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(frame2.slideX, frame2.slideY, frame2.slideZ);
		//glRotated(axis.rotateZ, 0, 0, 1);
		
		if(bWireFrame)
			frame2.DrawWireframe();
		else
			frame2.DrawColor();
	glPopMatrix();
}

void drawFrame3()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(frame3.slideX, frame3.slideY, frame3.slideZ);
		//glRotated(axis.rotateZ, 0, 0, 1);
		
		if(bWireFrame)
			frame3.DrawWireframe();
		else
			frame3.DrawColor();
	glPopMatrix();
}

void drawBall1()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(ball1.slideX, ball1.slideY, ball1.slideZ);
		glRotated(ball1.rotateY, 0, 1, 0);
		
		if(bWireFrame)
			ball1.DrawWireframe();
		else
			ball1.DrawColor();
	glPopMatrix();
}

void drawBall2()
{
	glPushMatrix();
		glTranslated(wheel.slideX, 0, wheel.slideZ);
		glRotated(wheel.rotateY, 0, 1, 0);
		
		glTranslated(ball2.slideX, ball2.slideY, ball2.slideZ);
		glRotated(ball1.rotateY, 0, 1, 0);
		
		if(bWireFrame)
			ball2.DrawWireframe();
		else
			ball2.DrawColor();
	glPopMatrix();
}


// Testing

void drawTexFloor(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	//1 -> fRoomSize/2, 0, fRoomSize/2
		//2 -> fRoomSize/2, 0, -fRoomSize/2
		//3 -> -fRoomSize/2, 0, -fRoomSize/2
		//4 -> -fRoomSize/2, 0, fRoomSize/2
		//5 -> -fRoomSize/2, 15, fRoomSize/2
		//6 -> fRoomSize/2, 15, -fRoomSize/2
		//7	-> -fRoomSize/2, 15, -fRoomSize/2
		//8 -> -fRoomSize/2, 15, fRoomSize/2

	
	//Face 1

	glBindTexture(GL_TEXTURE_2D, tex_yneg.texID);
	glBegin(GL_QUADS);

		
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(fRoomSize/2, 0, fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-fRoomSize/2, 0, fRoomSize/2); 
	glEnd();
		

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void drawTexWall(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	//1 -> fRoomSize/2, 0, fRoomSize/2
		//2 -> fRoomSize/2, 0, -fRoomSize/2
		//3 -> -fRoomSize/2, 0, -fRoomSize/2
		//4 -> -fRoomSize/2, 0, fRoomSize/2
		//5 -> -fRoomSize/2, 15, fRoomSize/2
		//6 -> fRoomSize/2, 15, -fRoomSize/2
		//7	-> -fRoomSize/2, 15, -fRoomSize/2
		//8 -> -fRoomSize/2, 15, fRoomSize/2

	
		
	//Face 2

	glBindTexture(GL_TEXTURE_2D, tex_zpos.texID);
	glBegin(GL_QUADS);

		
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(fRoomSize/2, 0, fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-fRoomSize/2, 0, fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-fRoomSize/2, fRoomSize, fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f(fRoomSize/2, fRoomSize, fRoomSize/2); 
	glEnd();
		
	//Face 3

	glBindTexture(GL_TEXTURE_2D, tex_xneg.texID);
	glBegin(GL_QUADS);

		
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f( fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(fRoomSize/2, 0, fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(fRoomSize/2,  fRoomSize, fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f(fRoomSize/2,  fRoomSize, -fRoomSize/2); 
	glEnd();
		
	//Face 4

	glBindTexture(GL_TEXTURE_2D, tex_zneg.texID);
	glBegin(GL_QUADS);

		
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(fRoomSize/2,  fRoomSize, -fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-fRoomSize/2,  fRoomSize, -fRoomSize/2); 
	glEnd();
		
	//Face 5

	glBindTexture(GL_TEXTURE_2D, tex_xpos.texID);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-fRoomSize/2, 0, fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-fRoomSize/2, 0, -fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-fRoomSize/2,  fRoomSize, -fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f( -fRoomSize/2,  fRoomSize, fRoomSize/2); 
		
	glEnd();
		
	//Face 6

	glBindTexture(GL_TEXTURE_2D, tex_ypos.texID);
	glBegin(GL_QUADS);

		
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(fRoomSize/2,  fRoomSize, fRoomSize/2); 
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-fRoomSize/2,  fRoomSize, fRoomSize/2); 
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-fRoomSize/2,  fRoomSize, -fRoomSize/2); 
		glTexCoord2f(0.0, 1.0);
		glVertex3f(fRoomSize/2,  fRoomSize, -fRoomSize/2); 
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawTexWheel(){
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		int nSegment=20;
		int numVerts = 2*nSegment + 4;
		int numFaces = 3*nSegment;
		float fRadius = wheelRadius;
		float fHeight = wheelWidth;
		glBindTexture(GL_TEXTURE_2D, wheelTex.texID);
		glBegin(GL_TRIANGLE_FAN);
			float y=fHeight/2;
			glTexCoord2f(0.5, 0.5);
			glVertex3f(0, y, 0);
			for (int i = 0; i <= nSegment; i++)
			{
					float deg = 360*i/nSegment * PI / 180;
					float x=fRadius*sin(deg);
					float z=fRadius*cos(deg); 

					glTexCoord2f(0.5*sin(deg)+0.5, 0.5*cos(deg)+0.5);
					glVertex3f(x, y, z);

			}
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
			y=-y;
			glTexCoord2f(0.5, 0.5);
			glVertex3f(0, y, 0);
			for (int i = 0; i <= nSegment; i++)
			{
					float deg = -360*i/nSegment * PI / 180;
					float x=fRadius*sin(deg);
					float z=fRadius*cos(deg); 

					glTexCoord2f(0.5*sin(deg)+0.5, 0.5*cos(deg)+0.5);
					glVertex3f(x, y, z);

			}
		glEnd();

		y=-y;
		glBindTexture(GL_TEXTURE_2D, tyreTex.texID);
		
			for (int i = 0; i < nSegment; i++)
			{
				glBegin(GL_QUADS);
						float deg = 360*i/nSegment * PI / 180;
						float deg_1 = 360*(i+1)/nSegment * PI / 180;
						float x=fRadius*sin(deg);
						float z=fRadius*cos(deg); 
						float x_1=fRadius*sin(deg_1);
						float z_1=fRadius*cos(deg_1); 
						float x_tex = 0;
						float y_tex = 0;
						glTexCoord2f(0, 0);
						glVertex3f(x, y, z);
						glTexCoord2f(0, 1);
						glVertex3f(x, -y, z);
						glTexCoord2f(1, 1);
						glVertex3f(x_1, -y, z_1);
						glTexCoord2f(1, 0);
						glVertex3f(x_1, y, z_1);
					glEnd();

			}
			glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}



void drawBlending(){
	glPushMatrix();
	glScalef(1,1,1);
	glRotatef(180,0,0,1);
		glColor3f(1,1,1);
		drawWheel();
		drawAxis();
	
		drawFrame1();
	
		drawFrame2();
	
		drawFrame3();
	
		drawBall1();
	
		drawBall2();
		glRotatef(-180,0,0,1);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,0.85);
		drawFloor();
	glDisable(GL_BLEND);
	glPopMatrix();
}









void DisplayOneView(int nType, int left, int right, int top, int bottom)
{
	mySetupCameraVolume(nType);
	glViewport(left, top, right - left, bottom - top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	switch (nType)
	{
	case 1:
		gluLookAt(0, camera_dis, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		break;
	case 2:
		gluLookAt(0, 0.0, camera_dis, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	case 3:
		gluLookAt(camera_dis, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	case 4:
		gluLookAt(camera_X, camera_Y, camera_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	default:
		break;
	}
	//gluLookAt(camera_X, camera_Y, camera_Z, 0.0, camera_lookAt, 0.0, 0.0, 1.0, 0.0);
	
	//drawFloor();
	glColor3f(1,1,1);
	drawTexWall();
	//drawFloor();
	glColor3f(1,1,1);
	drawWheel();
	
	drawAxis();
	
	drawFrame1();
	
	drawFrame2();
	
	drawFrame3();
	
	drawBall1();
	
	drawBall2();
	
	
	drawBlending();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glDisable(GL_LIGHTING);

	if(b4View == false)
	{
		DisplayOneView(4, 0, screenWidth, 0, screenHeight);
	}
	else
	{
		DisplayOneView(1, 0, screenWidth/2, 0, screenHeight/2);
		DisplayOneView(2, 0, screenWidth/2, screenHeight/2, screenHeight);
		DisplayOneView(3, screenWidth/2, screenWidth, screenHeight/2, screenHeight);
		DisplayOneView(4, screenWidth/2, screenWidth, 0, screenHeight/2);
	}

	glFlush();
    glutSwapBuffers();
}

void myInit()
{
	float	fHalfSize = 3;

	//cameraX = cameraY = cameraZ = 0.0f;
	camera_angle = 0.0;
	camera_height = 2.0;
	camera_lookAt = 0;
	camera_dis = fRoomSize/2 - 1;
	camera_X = sin(DEG2RAD(camera_angle))*camera_dis;
	camera_Y = camera_height;
	camera_Z = cos(DEG2RAD(camera_angle))*camera_dis;

	glShadeModel (GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	loadTextures();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
	//load all textures
	loadTextures();

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void myDelay()
{
	for(int i = 0; i<100000;i++)
		for(int j = 0; j<30; j++)
			sin(100.0f);
}

void myIdle()
{
	if(bAnimated == false)
		return;
	
	float	fRInc;
	float	fAngleInc;

	wheel.rotateY += 0.0001;
	if(wheel.rotateY > 360)
		wheel.rotateY -= 360;
	if(wheel.rotateY < 0)
		wheel.rotateY += 360;

	fRInc = fRotateRadius*(2*2*PI/360);
	fAngleInc = fRInc/wheelRadius;
	fAngleInc = fAngleInc*360/(2*PI);

	wheel.rotateX += fAngleInc;
	if(wheel.rotateX > 360)
		wheel.rotateX -= 360;
	if(wheel.rotateX < 0)
		wheel.rotateX += 360;
	fRInc = wheelRadius*(fAngleInc*2*PI/360);
	wheel.slideX += fRInc*sin(wheel.rotateY*2*PI/360);
	wheel.slideZ += fRInc*cos(wheel.rotateY*2*PI/360);

	wheel.rotateY += 2;
	if(wheel.rotateY > 360)
		wheel.rotateY -= 360;
	if(wheel.rotateY < 0)
		wheel.rotateY += 360;
	
	myDelay();

	myDisplay();
}

void reshape (int w, int h)
{
	
	screenWidth = w;
	screenHeight = h;
   
	mySetupCameraVolume(4);

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Simple Robot Ass2"); // open the screen window

	cout << "Press 'X' to rotate head counterclockwise." << endl;
	cout << "Press 'x' to rotate head clockwise." << endl;
	cout << "Press 'X' to rotate wheel counterclockwise." << endl;
	cout << "Press 'x' to rotate wheel clockwise." << endl;
	cout << "Press 'F' or 'f' to move forward." << endl;
	cout << "Press 'B' or 'b' to move backward." << endl;
	cout << "Press 'R' or 'r' to reset." << endl;
	cout << "Press 'O' or 'o' to switch animation." << endl;
	cout << "Press 'V' or 'v' to switch between 1 and 4 views." << endl;
	cout << "Press '+' to increase camera distance." << endl;
	cout << "Press '-' to decrease camera distance." << endl;
	cout << "Press 'up arrow' to increase camera height." << endl;
	cout << "Press 'down arrow' to decrease camera height." << endl;
	cout << "Press '<-' to rotate camera clocewise." << endl;
	cout << "Press '->' to rotate camera counterclockwise." << endl;
	cout << "Press 'L' to increase camera look at." << endl;
	cout << "Press 'l' to decrease camera look at." << endl;



	/*planeY.CreateYPlane(20, 20, 10, 10);
	planeY.slideY = YPlanePos;
	planeY.SetColor(7);*/

	wheel.CreateCylinder(20, wheelWidth, wheelRadius);
	wheel.rotateZ = 90;
	wheel.rotateY = 0;
	wheel.slideY = YPlanePos + wheelRadius;
	//wheel.slideX = 5;
	//wheel.slideZ = 5;
	//wheel.SetColor(0);
	
	axis.CreateCylinder(20, wheelWidth, wheelRadius/6);
	axis.rotateZ = 90;
	axis.slideZ = 0;
	axis.slideY = YPlanePos + wheelRadius;
	axis.slideX = wheelWidth;
	axis.SetColor(1);

	frame1.CreateCuboid(wheelWidth/2, frame1Height, wheelWidth/2);
	frame1.slideY = YPlanePos + frame1Height + wheelRadius - wheelWidth/2;
	frame1.slideX = 2*wheelWidth;
	frame1.SetColor(2);

	frame2.CreateCuboid(3*wheelWidth/2, wheelWidth/2, wheelWidth/2);
	frame2.slideY = YPlanePos + wheelWidth/2 + wheelRadius - wheelWidth/2 + frame1Height*2 - wheelWidth;
	frame2.SetColor(3);


	frame3.CreateCylinder(20, frame3Height, wheelRadius/6);
	frame3.slideY = YPlanePos + frame3Height/2 + frame1Height*2 + wheelRadius - wheelWidth/2;
	frame3.SetColor(4);

	ball1.CreateSphere(20, 20, ball1Radius);
	ball1.slideY = YPlanePos + ball1Radius + frame1Height*2 + frame3Height+wheelRadius- wheelWidth/2;;
	ball1.SetColor(5);

	ball2.CreateSphere(20, 20, ball2Radius);
	ball2.slideY = YPlanePos + ball2Radius + frame1Height*2 + frame3Height+wheelRadius- wheelWidth/2 + ball1Radius - ball2Radius;
	ball2.slideZ = ball1Radius;
	ball2.SetColor(6);

	myInit();
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);
    glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialKeyboard);
	glutReshapeFunc(reshape);
	
  
	glutMainLoop();
	return 0;
}

