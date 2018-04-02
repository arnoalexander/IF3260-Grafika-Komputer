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
Color warna1 = {1.0f, 0.0f, 0.0f};
Polygon side1 = {
  {-0.75f, -0.5f, -0.5f},
  {0.75f, -0.5f, -0.5f},
  {0.75f, 0.0f, -0.5f},
  {0.5f, 0.0f, -0.5f},
  {0.25f, 0.5f, -0.5f},
  {-0.25f, 0.5f, -0.5f},
  {-0.5f, 0.0f, -0.5f},
  {-0.75f, 0.0f, -0.5f},
};

Color warna2 = {1.0f, 0.0f, 0.0f};
Polygon side2 = {
  {-0.75f, -0.5f, 0.5f},
  {0.75f, -0.5f, 0.5f},
  {0.75f, 0.0f, 0.5f},
  {0.5f, 0.0f, 0.5f},
  {0.25f, 0.5f, 0.5f},
  {-0.25f, 0.5f, 0.5f},
  {-0.5f, 0.0f, 0.5f},
  {-0.75f, 0.0f, 0.5f},
};

Color warna3 = {1.0f, 0.0f, 0.0f};
Polygon side3 = {
  {0.25f, 0.5f, -0.5f},
  {0.5f, 0.0f, -0.5f},
  {0.5f, 0.0f, 0.5f},
  {0.25f, 0.5f, 0.5f}
};

Color warna4 = {1.0f, 0.0f, 0.0f};
Polygon side4 = {
  {0.75f, -0.5f, 0.5f},
  {0.75f, 0.0f, 0.5f},
  {0.75f, 0.0f, -0.5f},
  {0.75f, -0.5f, -0.5f}
};

Color warna5 = {1.0f, 0.0f, 0.0f};
Polygon side5 = {
  {-0.25f, 0.5f, -0.5f},
  {-0.5f, 0.0f, -0.5f},
  {-0.5f, 0.0f, 0.5f},
  {-0.25f, 0.5f, 0.5f}
};

Color warna6 = {1.0f, 0.0f, 0.0f};
Polygon side6 = {
  {-0.75f, -0.5f, 0.5f},
  {-0.75f, 0.0f, 0.5f},
  {-0.75f, 0.0f, -0.5f},
  {-0.75f, -0.5f, -0.5f}
};

Color warna7 = {1.0f, 0.0f, 0.0f};
Polygon side7 = {
  {-0.75f, -0.5f, 0.5f},
  {-0.75f, -0.5f, -0.5f},
  {0.75f, -0.5f, -0.5f},
  {0.75f, -0.5f, 0.5f}
};

Color warna8 = {1.0f, 0.0f, 0.0f};
Polygon side8 = {
  {-0.25f, 0.5f, -0.5f},
  {-0.25f, 0.5f, 0.5f},
  {0.25f, 0.5f, 0.5f},
  {0.25f, 0.5f, -0.5f}
};

Color warna9 = {1.0f, 0.0f, 0.0f};
Polygon side9 = {
  {-0.5f, 0.0f, -0.5f},
  {-0.75f, 0.0f, -0.5f},
  {-0.75f, 0.0f, 0.5f},
  {-0.5f, 0.0f, 0.5f},
};

Color warna10 = {1.0f, 0.0f, 0.0f};
Polygon side10 = {
  {0.5f, 0.0f, -0.5f},
  {0.75f, 0.0f, -0.5f},
  {0.75f, 0.0f, 0.5f},
  {0.5f, 0.0f, 0.5f}
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
  draw_polygon(warna7, side7);
  draw_polygon(warna8, side8);
  draw_polygon(warna9, side9);
  draw_polygon(warna10, side10);

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