#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

using namespace std;

const double PI = 3.141592653589793;

void draw(GLuint programID, GLuint vertexBuffer, GLint vertexBufferLength) {
	glUseProgram(programID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, vertexBufferLength);
	glDisableVertexAttribArray(0);
}

int main(){
	if (!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "Mobil Mantap", NULL, NULL);
	if(window == NULL){
		fprintf(stderr, "Failed to open GLFW window\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint programIDmobil = LoadShaders( "shader/vertex_shader.glsl", "shader/mobil_fragment_shader.glsl" );
	GLuint programIDroda = LoadShaders( "shader/vertex_shader.glsl", "shader/roda_fragment_shader.glsl" );

	//variabel gambar
	GLfloat g_vertex_buffer_data_mobil[] = {
		0.5f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.75f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.0f,
		0.75f, 0.0f, 0.0f,
		0.75f, -0.5f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		-0.75f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		-0.75f, 0.0f, 0.0f,
		-0.75f, -0.5f, 0.0f,

		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		0.25f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,

		-0.25f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,

		-0.25f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		0.25f, 0.0f, 0.0f,

		-0.25f, 0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.25f, 0.0f, 0.0f,
	};

	GLfloat g_vertex_buffer_data_roda1[] = {
		-0.7f, -0.55f, 0.0f,
		-0.7f, -0.75f, 0.0f,
		-0.5f, -0.75f, 0.0f,

		-0.7f, -0.55f, 0.0f,
		-0.5f, -0.55f, 0.0f,
		-0.5f, -0.75f, 0.0f,

		-0.7f, -0.55f, 0.0f,
		-0.5f, -0.55f, 0.0f,
		-0.6f, -0.5f, 0.0f,

		-0.7f, -0.75f, 0.0f,
		-0.5f, -0.75f, 0.0f,
		-0.6f, -0.8f, 0.0f,
	};

	GLfloat g_vertex_buffer_data_roda2[] = {
		0.7f, -0.55f, 0.0f,
		0.70f, -0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,

		0.70f, -0.55f, 0.0f,
		0.5f, -0.55f, 0.0f,
		0.5f, -0.75f, 0.0f,

		0.7f, -0.55f, 0.0f,
		0.5f, -0.55f, 0.0f,
		0.6f, -0.5f, 0.0f,

		0.7f, -0.75f, 0.0f,
		0.5f, -0.75f, 0.0f,
		0.6f, -0.8f, 0.0f,
	};

	GLfloat translation[] = {0.001, 0.001f};
	GLfloat angle = 0.0f;
	GLfloat angle_velocity = 0.05f;

	// bind buffer
	// mobil
	GLuint vertex_buffer_mobil;
	glGenBuffers(1, &vertex_buffer_mobil);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_mobil);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_mobil), g_vertex_buffer_data_mobil, GL_STATIC_DRAW);
	// roda 1
	GLuint vertex_buffer_roda1;
	glGenBuffers(1, &vertex_buffer_roda1);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_roda1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_roda1), g_vertex_buffer_data_roda1, GL_STATIC_DRAW);
	// roda 2
	GLuint vertex_buffer_roda2;
	glGenBuffers(1, &vertex_buffer_roda2);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_roda2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_roda2), g_vertex_buffer_data_roda2, GL_STATIC_DRAW);

	do{
		// draw to window
    glClear( GL_COLOR_BUFFER_BIT );
		draw(programIDmobil, vertex_buffer_mobil, sizeof(g_vertex_buffer_data_mobil) / sizeof(GLfloat)); // mobil
		draw(programIDroda, vertex_buffer_roda1, sizeof(g_vertex_buffer_data_roda1) / sizeof(GLfloat)); // roda 1
		draw(programIDroda, vertex_buffer_roda2, sizeof(g_vertex_buffer_data_roda2) / sizeof(GLfloat)); // roda 2
		glfwSwapBuffers(window);

		// calculation
		angle += angle_velocity;
		GLfloat new_translation_x = translation[0] * cos(angle) + translation[1] * sin(angle);
		GLfloat new_translation_y = -translation[0] * sin(angle) + translation[1] * cos(angle);
		for (int i = 0; i < sizeof(g_vertex_buffer_data_roda1) / sizeof(GLfloat); i+=3) {
			g_vertex_buffer_data_roda1[i] += new_translation_x;
			g_vertex_buffer_data_roda1[i+1] += new_translation_y;
		}
		for (int i = 0; i < sizeof(g_vertex_buffer_data_roda2) / sizeof(GLfloat); i+=3) {
			g_vertex_buffer_data_roda2[i] += new_translation_x;
			g_vertex_buffer_data_roda2[i+1] += new_translation_y;
		}

		// rebind buffer
		// roda 1
		glGenBuffers(1, &vertex_buffer_roda1);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_roda1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_roda1), g_vertex_buffer_data_roda1, GL_STATIC_DRAW);
		// roda 2
		glGenBuffers(1, &vertex_buffer_roda2);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_roda2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_roda2), g_vertex_buffer_data_roda2, GL_STATIC_DRAW);

		// input events
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
