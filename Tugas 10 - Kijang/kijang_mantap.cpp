/*
Author : Arno Alexander
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glut.h>

using namespace std;

#define WINDOW_DEFAULT_WIDTH 600
#define WINDOW_DEFAULT_HEIGHT 600
#define CIRCLE_DEFAULT_SEGMENTS 400
#define PI 3.14159265358979323846

typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
} Color;

typedef struct {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Point;

typedef vector<Point> Polygon;

//variabel mobil
Color warna1 = {0.6f, 0.1f, 0.9f};
Polygon side1 = {
  {0.5f, -0.5f, -0.5f},
  {0.5f, 0.5f, -0.5f},
  {-0.5f, 0.5f, -0.5f},
  {-0.5f, -0.5f, -0.5f}
};
Color warna2 = {0.6f, 0.1f, 0.85f};
Polygon side2 = {
  {0.5f, -0.5f, 0.5f},
  {0.5f, 0.5f, 0.5f},
  {-0.5f, 0.5f, 0.5f},
  {-0.5f, -0.5f, 0.5f}
};
Color warna3 = {0.6f, 0.1f, 0.8f};
Polygon side3 = {
  {0.5f, -0.5f, -0.5f},
  {0.5f, 0.5f, -0.5f},
  {0.5f, 0.5f, 0.5f},
  {0.5f, -0.5f, 0.5f}
};
Color warna4 = {0.6f, 0.1f, 0.75f};
Polygon side4 = {
  {-0.5f, -0.5f, 0.5f},
  {-0.5f, 0.5f, 0.5f},
  {-0.5f, 0.5f, -0.5f},
  {-0.5f, -0.5f, -0.5f}
};
Color warna5 = {0.6f, 0.1f, 0.7f};
Polygon side5 = {
  {0.5f, 0.5f, 0.5f},
  {0.5f, 0.5f, -0.5f},
  {-0.5f, 0.5f, -0.5f},
  {-0.5f, 0.5f, 0.5f}
};
Color warna6 = {0.6f, 0.1f, 0.65f};
Polygon side6 = {
  {0.5f, -0.5f, -0.5f},
  {0.5f, -0.5f, 0.5f},
  {-0.5f, -0.5f, 0.5f},
  {-0.5f, -0.5f, -0.5f}
};
// variabel transformasi
int rotate_x = 0; int rotate_y = 0;

void draw_circle(Color color, Point center, GLfloat radius) {
	GLint segment_number = GLint(round(GLfloat(CIRCLE_DEFAULT_SEGMENTS) * radius));
	GLfloat segment_angle = GLfloat(2.0 * PI) / GLfloat(segment_number);
	glBegin(GL_POLYGON);
	glColor3f(color.r, color.g, color.b);
	for (GLint i = 0; i < segment_number; i++) {
		glVertex2f(center.x + radius * sin(segment_angle * i), center.y + radius * cos(segment_angle * i));
	}
	glEnd();
}

void draw_polygon(Color color, Polygon polygon) {
  glBegin(GL_POLYGON);
	glColor3f(color.r, color.g, color.b);
	for (GLint i = 0; i < polygon.size(); i++) {
		glVertex3f(polygon[i].x, polygon[i].y, polygon[i].z);
	}
	glEnd();
}

void display();
void keyboard_func(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {
  /*glut init*/
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
  glutCreateWindow("Kijang Mantap");
  glEnable(GL_DEPTH_TEST);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard_func);

  /*main loop*/
  glutMainLoop();

	/*end*/
	return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );

  draw_polygon(warna1, side1);
  draw_polygon(warna2, side2);
  draw_polygon(warna3, side3);
  draw_polygon(warna4, side4);
  draw_polygon(warna5, side5);
  draw_polygon(warna6, side6);

  glFlush();
  glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y) {
  if (key == 'd')
    rotate_y += 5;
  else if (key == 'a')
    rotate_y -= 5;
  else if (key == 'w')
    rotate_x += 5;
  else if (key == 's')
    rotate_x -= 5;

  glutPostRedisplay();
}
