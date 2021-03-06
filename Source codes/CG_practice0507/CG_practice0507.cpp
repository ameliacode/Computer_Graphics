#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <InitShader.h>

#include <vec.h>
#include <mat.h>

#include <iostream>

#define BUFFER_OFFSET( offset ) ((GLvoid*) (offset))

int NumTimesToSubdivide;
int NumTriangles;
int NumVertices;

vec3* points;
vec3* colors;
int index = 0;

vec3 baseColors[] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 1.0f)
};
GLuint vao;
GLuint buffer;
GLuint simpleProgram;

void init();
void display();
void idle();

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);
void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli);
void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4);

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(512, 512, "practice0507", NULL, NULL);
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

	glfwSetKeyCallback(window, gasketKeyCallback);
	glfwSetMouseButtonCallback(window, gasketMouseButtonCallback);

	///////////////////////////////////////////////////////////////////////////
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		display();
		idle();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	free(points);
	free(colors);
	return 0;
}

void init()
{
	NumTimesToSubdivide = 5;
	NumTriangles = 4 * pow(3, NumTimesToSubdivide);
	NumVertices = 3 * NumTriangles;

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);

	setGasketVertices(0, 1, 2, 3);

	simpleProgram = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(simpleProgram);

	GLuint vPosition = glGetAttribLocation(simpleProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(simpleProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle()
{

}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli)
{
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
}

void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli)
{
	if (cnt > 0)
	{
		vec3 ab = (a + b) / 2.0f;
		vec3 ac = (a + c) / 2.0f;
		vec3 bc = (b + c) / 2.0f;
		divide_triangle(a, ab, ac, cnt - 1, coli);
		divide_triangle(c, ac, bc, cnt - 1, coli);
		divide_triangle(b, bc, ab, cnt - 1, coli);
	}
	else
	{
		triangle(a, b, c, coli);
	}
}

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS)
	{
		std::cout << "Key 1 is pressed\n";
		setGasketVertices(1, 2, 3, 0);
	}
}

void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "MOUSE BUTTON LEFT\n";
	}
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4)
{
	index = 0;

	vec3 base[4] =
	{
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.942809f, -0.333333f),
		vec3(-0.816497f, -0.471405f, -0.333333f),
		vec3(0.816497f, -0.471405f, -0.333333f)
	};

	divide_triangle(base[0], base[1], base[2], NumTimesToSubdivide, col1);
	divide_triangle(base[3], base[2], base[1], NumTimesToSubdivide, col2);
	divide_triangle(base[0], base[3], base[1], NumTimesToSubdivide, col3);
	divide_triangle(base[0], base[2], base[3], NumTimesToSubdivide, col4);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
}