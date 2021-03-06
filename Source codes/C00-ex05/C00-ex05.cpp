 #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Angel.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) // glfw 라이브러리 초기화
		return -1;

	window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);	// window 생성
	if (!window)	// window 생성 실패 시 프로그램 종료
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // window context 생성

	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew 라이브러리 초기화 및 초기화 실패 시 에러 메세지
														// glew 라이브러리는 반드시 window context 생성 후 초기화 아니면 에러

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

	float positions[] =		// 사각형을 그리기 위한 4개의 vertex(x, y)에 대한 정보를 담은 1차원 배열
	{					// 2차원이라고 가정 했을 경우 -> 실질적인 각 vertex의 시작점
		-0.5f, -0.5f,	// 0 -> 0
		 0.5f, -0.5f,	// 1 -> 2
		 0.5f,  0.5f,	// 2 -> 4
		-0.5f,  0.5f	// 3 -> 6
	};

	unsigned int indices[] =	// 하나의 사각형을 그리기 위한 삼각형 2개에 대한 정보
	{
		0, 1, 2,	// 0
		2, 3, 0		// 1
	};

	GLuint va;	// vertex array 선언, 메모리 할당, 바인드
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLuint vb;	// vertex buffer 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW);
		// 4*2*sizeof(float) = (vertex 갯수)*(각 vertex의 요소 갯수)*(각 요소 데이터 타입의 크기)

	GLuint ib;	// index buffer, 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*3*sizeof(unsigned int), indices, GL_STATIC_DRAW);
		// 2*3*sizeof(unsigned int) = (삼각형 갯수)*(각 삼각형에 필요한 vertex 갯수)*(각 요소 데이터 타입의 크기)

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");	// shader program 가져오기
	glUseProgram(program);	// 어떤 shader program을 사용할 것인지

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex에 대한 정보를 shader program 안의 어떤 변수와 연결시킬 것인가
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

///////////////////////////////////////////////////////////////////////////
// window 창이 종료될 때까지 무한루프 
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		// 삼각형 정보가 들어있는 indices 배열을 차례로 읽어 3개씩 하나의 삼각형을 위한 index으로 묶고,
		// index에 해당하는 vertex 정보를 positions 배열에서 찾아오는 방식.

	//////////////////////////////////////////////////////////////////////
		glfwSwapBuffers(window);	// front buffer와 back buffer 교체
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}