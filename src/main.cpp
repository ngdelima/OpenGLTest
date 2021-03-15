/*
	Program: Circles Sum
	Description: Shows two objects in circular motion with modifiable angular velocities,
				and draws a 2D graph where each point drawn has components based on the 
				angular velocity of those objects.
	Author: Nico
*/

//Defines
#define _USE_MATH_DEFINES

//Standard includes
#include<iostream>
#include<vector>
#include<unordered_map>
#include<chrono>
#include<ctime>
#include <math.h>

//OpenGl includes
#include <GL/glut.h>

//Custom includes
#include<Widgets.h>
#include<InputManager.h>
#include<EventManager.h>
#include<EntityManager.h>
#include<CommandManager.h>

/* Global variables */

int winXPos = 700, winYPos = 400;
int winWidth = 500, winHeigth = 500;
int xCenter = 250, yCenter = 250;
int xRectanglePos = xCenter, yRectanglePos = yCenter; 
int rectangleWidth = 50;
float circleRadius = 50;
float indicatorCircleRadius = 5;
int circlePoints = 360;
float xAngularVelocity = 0.0, yAngularVelocity = 0.0;
float angularVelocityStep = M_PI / 4.0;

EventManager eventManager;
InputManager inputManager(eventManager);

MultipointLineWidget multipointLineWidget(xCenter, yCenter, true);
RotatingCircleWidget leftCirclewidget(xCenter/2, yCenter, circleRadius, indicatorCircleRadius, xAngularVelocity);
RotatingCircleWidget aboveCirclewidget(xCenter, (yCenter/2)*3, circleRadius, indicatorCircleRadius, yAngularVelocity);

CommandManager commandManager(&aboveCirclewidget, &leftCirclewidget, &multipointLineWidget);

/* Functions */

void keyDown( unsigned char key, int x, int y )
{
	inputManager.setKey(key, true);
}

void keyUp( unsigned char key, int x, int y )
{
	inputManager.setKey(key, false);
}

void specialKeyDown( int key, int x, int y )
{
	std::cout << "Special Key down " << key << std::endl;
	inputManager.setKey(key, true);
}

void specialKeyUp( int key, int x, int y )
{
	std::cout << "Special Key up " << key << std::endl;
	inputManager.setKey(key, false);
}

void drawSquare(float posX, float posY, float width)
{
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f(posX, posY);
	glVertex2f( posX + width, posY);
	glVertex2f( posX + width, posY + width);
	glVertex2f( posX, posY + width);
	glEnd();
}

void drawSquareCentered(float posX, float posY, float width)
{
	float halfWidth = width / 2;

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex2f( posX - halfWidth, posY - halfWidth);
	glVertex2f( posX + halfWidth, posY - halfWidth);
	glVertex2f( posX + halfWidth, posY + halfWidth);
	glVertex2f( posX - halfWidth, posY + halfWidth);
	glEnd();
}

void drawCircle(float posX, float posY, float rad, int points)
{
	float twicePi = 2.0f * M_PI;

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	for(size_t i = 0; i < points; ++i)
	{
		float xCoordinate = posX + (rad * cos(i *  twicePi / points));
		float yCoordinate = posY + (rad * sin(i *  twicePi / points));
		glVertex2f(xCoordinate, yCoordinate);
	}
	glEnd();
}

void drawCircleFilled(float posX, float posY, float rad, int points)
{
	float twicePi = 2.0f * M_PI;

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	for(size_t i = 0; i < points; ++i)
	{
		float xCoordinate = posX + (rad * cos(i *  twicePi / points));
		float yCoordinate = posY + (rad * sin(i *  twicePi / points));
		glVertex2f(xCoordinate, yCoordinate);
	}
	glEnd();
}

void drawRotatingCircleWidget(const RotatingCircleWidget& widget)
{
	drawCircleFilled(widget.getIndicatorPosX(), widget.getIndicatorPosY(), widget.getIndicatorRadius(), 360);
	drawCircle(widget.getPosX(), widget.getPosY(), widget.getRadius(), 360);
}

void drawMultipointLineWidget(const MultipointLineWidget& widget)
{
	std::vector<Position> points = widget.getPointsVector();
	int xPos = widget.getPosX();
	int yPos = widget.getPosY();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	auto it = points.begin();
	for (;it != points.end(); ++it)
	{
		glVertex2f(it->x, it->y);
	}
	if(widget.getDrawLastPoint())
	{
		it--;
		//TODO: Last two params must come from widget
		drawCircleFilled(it->x, it->y, indicatorCircleRadius, circlePoints);
	}
	glEnd();
}

void drawScene(void)
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw here
	//drawSquareCentered(xRectanglePos,yRectanglePos,rectangleWidth);

	//Update objects
	//leftCirclewidget.setAngularVelocity(xAngularVelocity);
	leftCirclewidget.update();
	//aboveCirclewidget.setAngularVelocity(yAngularVelocity);
	aboveCirclewidget.update();
	multipointLineWidget.addPoint(leftCirclewidget.getIndicatorPosX(), aboveCirclewidget.getIndicatorPosY());

	//Draw objects
	drawRotatingCircleWidget(leftCirclewidget);
	drawRotatingCircleWidget(aboveCirclewidget);
	drawMultipointLineWidget(multipointLineWidget);

	// Flush the pipeline, swap the buffers, and trigger redraw
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Initialize OpenGL's rendering modes
void initRendering()
{
	glShadeModel( GL_FLAT );   // The default value of GL_SMOOTH is usually better
	glEnable( GL_DEPTH_TEST );   // Depth testing must be turned on
}

// Called when the window is resized
//      w, h - width and height of the window in pixels.
/*void resizeWindow(int w, int h)
{

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );   // View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//      aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h )
	{
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else 
	{
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}

	// Now that we know the max & min values for x & y
	//      that should be visible in the window,
	//      we set up the orthographic projection.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );
	//glOrtho( 0, w, 0, h, -1, 1 );
}*/

void InitializeWindow(int xPos, int yPos, int width, int height, const char* title)
{
	//Measures in pixels
	glutInitWindowPosition( xPos, yPos );
	glutInitWindowSize( width, height );
	glutCreateWindow( title );
	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );
	glOrtho( 0, width, 0, height, -1, 1 );
}

/*
	Main
*/

int main( int argc, char** argv )
{
	eventManager.registerListener(commandManager, EVENT_TYPE_INPUT_KEY_DOWN);
	eventManager.registerListener(commandManager, EVENT_TYPE_INPUT_KEY_UP);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	InitializeWindow(winXPos,winYPos,winWidth,winHeigth,"Circles Sum");
	initRendering();

	//Input callbacks
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc( keyDown );
	glutKeyboardUpFunc( keyUp );
	glutSpecialFunc( specialKeyDown );
	glutSpecialUpFunc( specialKeyUp );

	//glutReshapeFunc( resizeWindow );
	glutDisplayFunc( drawScene );
	//Init objects before init loop
	leftCirclewidget.start();
	aboveCirclewidget.start();

	glutMainLoop(  );

	return(0);
}