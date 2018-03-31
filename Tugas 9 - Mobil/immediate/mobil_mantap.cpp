/*
Author : Arno Alexander
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
} Point;

typedef vector<Point> Polygon;

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
		glVertex2f(polygon[i].x, polygon[i].y);
	}
	glEnd();
}

// future scroll callback function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

// future key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

int main() {
	/*init*/
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, "Mobil Mantap", NULL, NULL);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSwapInterval(1); //vsync
	glfwSetTime(0.0);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*draw*/
  // variabel roda
  Color warna_roda = {0.5f, 0.6f, 0.1f};
  Point pusat_roda_1 = {-0.6f, -0.6f};
  Point pusat_roda_2 = {0.6f, -0.6f};
  GLfloat radius_roda = 0.15f;
  Color warna_efek_roda = {0.9f, 1.0f, 0.5f};
  Point pusat_efek_roda_1 = {-0.6f, -0.5f};
  Point pusat_efek_roda_2 = {0.6f, -0.7f};
  GLfloat radius_efek_roda = 0.04f;
  GLfloat angle = 0.0f;
  GLfloat angle_velocity = 0.01f;
  //variabel mobil
  Color warna_mobil = {0.6f, 0.1f, 0.9f};
  Polygon mobil = {
    {-0.25f, 0.25f},
    {-0.75f, -0.15f},
    {-0.8f, -0.45f},
    {0.8f, -0.45f},
    {0.75f, -0.15f},
    {0.5f, -0.1f},
    {0.25f, 0.25f}
  };
  Color warna_jendela = {0.0f, 0.125f, 0.9f};
  Polygon jendela1 = {
    {-0.24f, 0.24f},
    {-0.005f, 0.24f},
    {-0.005f, -0.05f},
    {-0.6f, -0.05f}
  };
  Polygon jendela2 = {
    {0.24f, 0.24f},
    {0.005f, 0.24f},
    {0.005f, -0.05f},
    {0.45f, -0.05f}
  };
  // loop
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		draw_circle(warna_roda, pusat_roda_1, radius_roda);
    draw_circle(warna_roda, pusat_roda_2, radius_roda);
    draw_circle(
      warna_efek_roda,
      {
        (pusat_efek_roda_1.x - pusat_roda_1.x) * cos(angle) + (pusat_efek_roda_1.y - pusat_roda_1.y) * sin(angle) + pusat_roda_1.x,
        -(pusat_efek_roda_1.x - pusat_roda_1.x) * sin(angle) + (pusat_efek_roda_1.y - pusat_roda_1.y)* cos(angle) + pusat_roda_1.y
      },
      radius_efek_roda
    );
    draw_circle(
      warna_efek_roda,
      {
        (pusat_efek_roda_2.x - pusat_roda_2.x) * cos(angle) + (pusat_efek_roda_2.y - pusat_roda_2.y) * sin(angle) + pusat_roda_2.x,
        -(pusat_efek_roda_2.x - pusat_roda_2.x) * sin(angle) + (pusat_efek_roda_2.y - pusat_roda_2.y)* cos(angle) + pusat_roda_2.y
      },
      radius_efek_roda
    );
    draw_polygon(warna_mobil, mobil);
    draw_polygon(warna_jendela, jendela1);
    draw_polygon(warna_jendela, jendela2);

    angle += angle_velocity;
    if (angle > 2*PI) angle -= 2*PI;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*end*/
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
