#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "MyCube.h"


unsigned int width = 512;
unsigned int height = 512;

GLuint simpleShader;
GLuint flatShader;
GLuint phongShader;

MyCube cube;

float time = 0.0f;

void init();
void display();
void idle();

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(width, height, "CG_pratice0604", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); /* Make the window's context current */

	if (glewInit() != GLEW_OK) std::cout << "Error\n";

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	init();

	///////////////////////////////////////////////////////////////////////////
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);

		display();
		idle();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void init()
{
	simpleShader = InitShader("vshader.glsl", "fshader.glsl");	// for only camera view and projection 
	flatShader = InitShader("vFlatShading.glsl", "fFlatShading.glsl");	// for flat shading
	phongShader = InitShader("vPhongShading.glsl", "fPhongShading.glsl");	// for phong shading
	cube.Init();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint myShader = phongShader;
	glUseProgram(myShader);

	// camera view matrix
	glm::mat4 myView = glm::mat4(1.0f);
	myView = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//myView = glm::lookAt(glm::vec3(2.0f*cos(time), 2.0f, -2.0f*sin(time)), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	// orthogonal projection matrix
	glm::mat4 myOrtho = glm::mat4(1.0f);
	myOrtho = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
	//myOrtho = glm::ortho(-5.0f*sin(time), 5.0f*sin(time), -5.0f*sin(time), 5.0f*sin(time), -5.0f*sin(time), 5.0f*sin(time));
	
	// perspective projection matrix
	glm::mat4 myPerspective = glm::mat4(1.0f);
	myPerspective = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 10.0f);
	//myPerspective = glm::perspective(glm::radians(30.0f*sin(time) + 30.0f), 1.0f, 0.01f, 10.0f);
	
	// for material properties
	glm::vec4 ma = glm::vec4(0.1, 0.1, 0.1, 1);	// ambient
	glm::vec4 md = glm::vec4(1, 0, 0, 1);		// diffse
	glm::vec4 ms = glm::vec4(1, 1, 0, 1);		// specular

	// for light properties
	glm::vec4 ld = glm::vec4(1, 1, 1, 1);		// diffuse
	glm::vec4 la = glm::vec4(1, 1, 1, 1);		// ambient light

	// for location
	//glm::vec4 ll = glm::vec4(0, 10, 0, 1);		// light
	glm::vec4 locLight = glm::vec4(10.0f*sin(time), 1.0f, 10.0f*cos(time), 1.0f);		// light
	glm::vec4 locEye = glm::vec4(0, 0, 0, 1);		// eye

	int gloss = 10;

	// for camera view and projection
	GLuint uModelView = glGetUniformLocation(myShader, "uModel");
	GLuint uOrtho = glGetUniformLocation(myShader, "uOrtho");
	GLuint uPerspective = glGetUniformLocation(myShader, "uPerspective");
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &myView[0][0]);
	glUniformMatrix4fv(uOrtho, 1, GL_FALSE, &myOrtho[0][0]);
	glUniformMatrix4fv(uPerspective, 1, GL_FALSE, &myPerspective[0][0]);

	// for shading
	GLuint uMAmb = glGetUniformLocation(myShader, "uMAmb");
	GLuint uMDif = glGetUniformLocation(myShader, "uMDif");
	GLuint uMSpec = glGetUniformLocation(myShader, "uMSpec");
	GLuint uLAmb = glGetUniformLocation(myShader, "uLAmb");
	GLuint uLDif = glGetUniformLocation(myShader, "uLDif");
	GLuint uLocLgt = glGetUniformLocation(myShader, "uLocLgt");
	GLuint uLocEye = glGetUniformLocation(myShader, "uLocEye");
	GLuint uGloss = glGetUniformLocation(myShader, "uGloss");
	glUniform4fv(uMAmb, 1, &ma[0]);
	glUniform4fv(uMDif, 1, &md[0]);
	glUniform4fv(uMSpec, 1, &ms[0]);
	glUniform4fv(uLAmb, 1, &la[0]);
	glUniform4fv(uLDif, 1, &ld[0]);
	glUniform4fv(uLocLgt, 1, &locLight[0]);
	glUniform4fv(uLocEye, 1, &locEye[0]);
	glUniform1i(uGloss, gloss);

	cube.Draw(myShader);
}

void idle()
{
	time += 0.02f;
}