
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>




const GLint width = 800, height = 600;
GLuint VAO, VBO, shader;


//Vertex Shader
static const char* vshader = "						    \n\
#version 330											\n\
														\n\
layout (location = 0)	 in vec3 pos;					\n\
void main()												\n\
{														\n\
														\n\
	gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);        \n\														\n\
}";	

static const char* fshader = "                          \n\
#version 330											\n\
														\n\
out vec4 colour;										\n\
void main()												\n\
{														\n\
														\n\
	colour = vec4(1.0,0.0,0.0,1.0);			        \n\														\n\
}";														


void Addshader(GLuint program, const char* code, GLenum type)
{
	//create shader
	GLuint theshader = glCreateShader(type);

	const GLchar* thecode[1];
	thecode[0] = code;

	GLint codelenght[1];
	codelenght[0] = strlen(code);

	//assign code to shader
	glShaderSource(theshader, 1, thecode, codelenght);
	//compile
	glCompileShader(theshader);



	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//check errors
	glGetShaderiv(theshader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theshader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s' \n", type, eLog);
		return;

	}

	//add shader to program
	glAttachShader(program, theshader);
}

void CompileShader()
{
	//Crear program store id
	shader = glCreateProgram();

	//check error
	if (!shader)
	{
		printf("Shader init failed");
		return;
	}

	//call addshader
	Addshader(shader, vshader, GL_VERTEX_SHADER);
	Addshader(shader, fshader, GL_FRAGMENT_SHADER);


	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//link to program
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	//check errors
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s' \n", eLog);
		return;

	}

	//validate the program 
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s' \n", eLog);
		return;

	}


}






void Createtriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f,-1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	//Obtenemos el ID para nuestro VAO --> http://docs.gl/gl3/glGenVertexArrays
	glGenVertexArrays(1, &VAO);
	//Asignamos ese id --> http://docs.gl/gl3/glBindVertexArray
	glBindVertexArray(VAO);

	/*A partir de aquí, todas las operaciones que hagamos con un VAO van a ser con el que tiene ese id que
hemos generado previamente*/

//Generamos el ID del VBO --> http://docs.gl/gl3/glGenBuffers
	glGenBuffers(1, &VBO);
	//Asignamos ese id --> http://docs.gl/gl3/glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Le especificamos a OpenGL qué estructura tiene nuestro vector de datos -->http://docs.gl/es2/glVertexAttribPointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}






int main()
{

	//init glfw
	if (!glfwInit())
	{
		printf("GLFW init fail");
		glfwTerminate();
		return 1;
	}

	//set up glfw window properties

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//allow forwars compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create window
	GLFWwindow* mainwindow = glfwCreateWindow(width, height, "Test", NULL, NULL);

	if (!mainwindow)
	{
		printf("Window init error");
		glfwTerminate();
		return 1;
	}

	//buffer size info
	int bufferwidth, bufferheigth;
	glfwGetFramebufferSize(mainwindow, &bufferwidth, &bufferheigth);

	glfwMakeContextCurrent(mainwindow);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("glew init fail");
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
		return 1;
	}

	//viewport size
	glViewport(0, 0, bufferwidth, bufferheigth);


	Createtriangle();
	CompileShader();




	//loop windows

	while (!glfwWindowShouldClose(mainwindow))
	{
		//handle user input
		glfwPollEvents();

		//clear window
		glClearColor(0.0f,0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		glUseProgram(0);
		glfwSwapBuffers(mainwindow);
	}



	return 0;
}