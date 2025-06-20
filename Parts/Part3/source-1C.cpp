
#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "stb_image.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
float zoom1=40;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Projection;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 ViewNEW;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

//************************************
// Η LoadShaders είναι black box για σας
//************************************
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void camera_function()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		float rotationn = 0.01;
		rotationn+=0.01;
		Model = glm::rotate(Model,rotationn, glm::vec3(rotationn,0.0f,0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
		float rotationn = 0.1;
		rotationn-=0.15;
		Model = glm::rotate(Model,rotationn, glm::vec3(-rotationn,0.0f,0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		float rotationn2 = 0.01;
		rotationn2+=0.01;
		Model = glm::rotate(Model,rotationn2, glm::vec3(0.0f,0.0f,rotationn2));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
		float rotationn2 =0.1;
		rotationn2-=0.15;
		Model = glm::rotate(Model,rotationn2, glm::vec3(0.0f,0.0f,-rotationn2));
	}
	if (glfwGetKey( window,GLFW_KEY_KP_ADD) == GLFW_PRESS){
		zoom1-=0.2;
	}
	if (glfwGetKey( window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
		zoom1+=0.1;
	}
		ViewNEW = glm::lookAt(
		glm::vec3(0.0f,zoom1, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
}




int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(950, 950, "Εργασία 1Γ – CSG-Boolean Operations", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
		
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	
	GLuint programID = LoadShaders("P1C.vertexshader", "P1C.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, zoom1, 40.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(0.0f, 0.0f, 2.5f), // and looks at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around other way around
	
	
	static const GLfloat figure[] = {
	

		-5.0f, -2.5f, 0.0f,  //katw
		 -5.0f, -2.5f, 5.0f,
		 5.0f, -2.5f, 0.0f, 
		5.0f, -2.5f, 5.0f,
		5.0f, -2.5f, 0.0f, 
		-5.0f, -2.5f, 5.0f,	
		 -5.0f, -2.5f, 5.0f, //mprosta
		 -5.0f, 2.5f, 5.0f,
		 5.0f, -2.5f, 5.0f,
		 5.0f, -2.5f, 5.0f,  
		 -5.0f, 2.5f, 5.0f,
		 5.0f, 2.5f, 5.0f,
		 -5.0f, 2.5f, 0.0f,  //plai aristera
		 -5.0f, 2.5f, 5.0f,
		 -5.0f, -2.5f, 5.0f,
	 	-5.0f, -2.5f, 5.0f,
		 -5.0f, -2.5f, 0.0f,
		 -5.0f, 2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f,  //plai dejia
		 5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,  
		5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,
		 5.0f, 2.5f, 5.0f,
		  -5.0f, 2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f,
		 5.0f, 2.5f, 0.0f, 
	 	-5.0f, -2.5f, 0.0f,  //  pisw
		 -5.0f, 2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f, 		 
		-5.0f, 2.5f, 0.0f,  
		 -5.0f, 2.5f, 5.0f,  //panw
		 5.0f, 2.5f, 0.0f,
		 -5.0f, 2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,
		 5.0f, 2.5f, 5.0f,  
		 
	};

	
		 
	static const GLfloat figure2[] = {

		 -2.5f, 14.0f, 5.0f,//PANW
		 -2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 5.0f,

		 -2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 5.0f,
		 2.5f, 14.0f, 0.0f,

		 -2.5f, 0.0f, 5.0f,
		 -2.5f, 0.0f, 0.0f,  //PLAINH aristera
		 -2.5f, 14.0f, 5.0f,

		 -2.5f, 14.0f, 0.0f,
		 -2.5f, 0.0f, 0.0f,
		 -2.5f, 14.0f, 5.0f,

		 2.5f, 0.0f, 0.0f,
		 2.5f, 14.0f, 0.0f,  //pisw
		 -2.5f, 0.0f, 0.0f,
		 -2.5f, 0.0f, 0.0f,		 
		 -2.5f, 14.0f, 0.0f, 
		 2.5f, 14.0f, 0.0f, 
		 -2.5f, 0.0f, 5.0f, //mprosta
		 -2.5f, 14.0f, 5.0f, 
		 2.5f, 0.0f, 5.0f,   
		 2.5f, 0.0f, 5.0f,
		 2.5f, 14.0f, 5.0f,
		 -2.5f, 14.0f, 5.0f,

		 2.5f, 0.0f, 0.0f,//PLAHNH dejia
		 2.5f, 0.0f, 5.0f,
		 2.5f, 14.0f, 0.0f,


		 2.5f, 14.0f, 5.0f,
		  2.5f, 0.0f, 5.0f, 
		  2.5f, 14.0f, 0.0f,
		
		 -2.5f, 0.0f, 0.0f,  //katv 
		 -2.5f, 0.0f, 5.0f,
		 2.5f, 0.0f, 0.0f, 
		 2.5f, 0.0f, 0.0f,
		 2.5f, 0.0f, 5.0f, 
		 -2.5f, 0.0f, 5.0f, 

		 
	};

	

	static const GLfloat uv_buffer1[] = {

		0.25f, 0.75f,
		0.25f, 0.5f,
		0.75f, 0.75f, 
		0.75f, 0.5f,
		0.75f, 0.75f,		
		0.25f, 0.5f,		
		0.25f,0.5f, //mprosta
		0.25f,0.25f,
		0.75f,0.5f, 
		0.75f,0.5f,
		0.25f,0.25f,
		0.75f,0.25f,
		0.0f, 0.25f,  // plai aristera
		0.25f, 0.25f,
		0.25f, 0.5f,		
		0.25f, 0.5f, 
		0.0f, 0.5f,
		0.0f, 0.25f,		
		1.0f, 0.5f, //plai dejia
		0.75f, 0.5f,
		1.0f, 0.25f,  
		0.75f, 0.5f,
		1.0f, 0.25f,
		0.75f, 0.25f,
		0.25f, 1.0f, //katw
		0.75f, 0.75f,
		0.75f, 1.0f,
		0.25, 0.75f, 
		0.25f, 1.0f,
		0.75f, 0.75f,		
		0.25f, 0.0f, //panw
		0.25f, 0.25f,
		0.75f, 0.0f,
		0.25f, 0.25f,
		0.75f, 0.0f,
		0.75f, 0.25f,
		
		
	};

	static const GLfloat uv_buffer2[] = {

		0.57f, 0.42f,
		0.57f, 0.57f,
		0.42f, 0.42f,

		0.57f, 0.57f,
		0.42f, 0.42f,
		0.42f, 0.57f,

		0.0f, 0.5f, //aristera
		0.42f, 0.5f,
		0.0f, 0.37f,

		0.42f, 0.37f,  
		0.42f, 0.5f,
		0.0f, 0.37f,

		0.42f, 0.37f,
		0.42f, 0.0f,
		0.57f, 0.37f,

		0.57f, 0.37f,
		0.57f, 0.0f,
		0.42f, 0.0f, 

		0.42f, 0.57f,
		0.42f, 0.85f,
		0.57f, 0.57f,

		0.57f, 0.57f,
		0.57f, 0.85f,
		0.42f, 0.85f,

		1.0f, 0.5f,  //dejia 
		0.57f, 0.5f,
		1.0f, 0.37f,

		0.57f, 0.37f,
		0.57f, 0.5f,
		1.0f, 0.37f,

		0.42, 0.5f,  //katw
		0.42f, 0.37f,
		0.57f, 0.5f,  
		0.57f, 0.5f, 
		0.57f, 0.37f,
		0.42, 0.5f,
		
	};
	

	static const GLfloat DIFF2[] = {

		 -2.5f, 14.0f, 5.0f,  //mprosta
		  2.5f, 14.0f, 5.0f,
		  2.5f, 2.5f, 5.0f,

		 2.5f, 2.5f, 5.0f,
		 -2.5f, 2.5f, 5.0f,
		 -2.5f, 14.0f, 5.0f,

		 -2.5f, 14.0f, 0.0f, //dipla aristera
		 -2.5f, 14.0f, 5.0f,
		 -2.5f, 2.5f, 5.0f,

		 -2.5f, 2.5f, 5.0f,
		 -2.5f, 2.5f, 0.0f,
		 -2.5f, 14.0f, 0.0f,

		 2.5f, 14.0f, 0.0f,
		 -2.5f, 14.0f, 0.0f,
		 -2.5f, 2.5f, 0.0f,

		 -2.5f, 2.5f, 0.0f,
		 2.5f, 2.5f, 0.0f,
		 2.5f, 14.0f, 0.0f,

		 2.5f, 14.0f, 5.0f,  //plai dejia
		 2.5f, 14.0f, 0.0f,
		 2.5f, 2.5f, 0.0f,

		 2.5f, 2.5f, 0.0f,
		 2.5f, 2.5f, 5.0f,
		 2.5f, 14.0f, 5.0f,
		 -2.5f, 14.0f, 5.0f,  //panw
		 -2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 5.0f,
		 -2.5f, 14.0f, 5.0f,
		 2.5f, 2.5, 5.0f, 
		 2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,
		-2.5f, 2.5f, 0.0f,
		-2.5f, 2.5, 5.0f,
		2.5f, 2.5, 5.0f,  

	};



	static const GLfloat uv_buffer5[] = {

		0.36f, 0.72f,  //mprosta
		0.6f, 0.72f,
		0.6f, 0.45f,
		0.6f, 0.45f,
		0.36f, 0.45f,
		0.36f, 0.72f,  
		0.0f, 0.27f,  	//plai aristera
		0.36f, 0.27f,
		0.36f, 0.45f,
		0.36f, 0.45f,
		0.0, 0.45f,
		0.0f, 0.27f,
		0.36f, 0.0f,   //pisw
		0.6f, 0.0f,
		0.6f, 0.27f,
		0.6f, 0.27f,
		0.36f, 0.27f,
		0.36f, 0.0f,
		0.6f, 0.27f,  	//plai dejia
		1.0f, 0.27f,
		1.0f, 0.45f,
		1.0f, 0.45f,
		0.6, 0.45f,
		0.6f, 0.27f, 
		0.6f, 0.72f, //panw
		0.6f, 1.0f,
		0.36f, 1.0f, 
		0.36f, 1.0f,
		0.36f, 0.72f,
		0.6f, 0.72f,
		0.6f, 0.27f,  //katw
		0.6f, 0.45f,
		0.36f, 0.45f,
		0.36f, 0.45f,
		0.36f, 0.27f,
		0.6f, 0.27f,

	};

	static const GLfloat DIFF1[] = {
		-5.0f, -2.5f, 0.0f,  //katw
		 -5.0f, -2.5f, 5.0f,
		 5.0f, -2.5f, 0.0f, 
		 5.0f, -2.5f, 5.0f,
		 5.0f, -2.5f, 0.0f, 
		 -5.0f, -2.5f, 5.0f,	
		 -5.0f, 2.5f, 0.0f,  //plai aristera
		 -5.0f, 2.5f, 5.0f,
		 -5.0f, -2.5f, 5.0f,
	 	 -5.0f, -2.5f, 5.0f,
		 -5.0f, -2.5f, 0.0f,
		 -5.0f, 2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f,  //plai dejia
		 5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,  
		 5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,
		 5.0f, 2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f, //pisw tetr aristera
		 2.5f, 2.5f, 0.0f,
		 2.5f, 0.0f, 0.0f,
		 2.5f, 0.0f, 0.0f,
		 5.0f, 0.0f, 0.0f,
		 5.0f, 2.5f, 0.0f,
		 -2.5f, 2.5f, 0.0f, //tetr dejia
		 -5.0f, 2.5f, 0.0f,
		 -5.0f, 0.0f, 0.0f,
		 -5.0f, 0.0f, 0.0f,
		 -2.5f, 0.0f, 0.0f,	
		 -2.5f, 2.5f, 0.0f,	
		 5.0f, 0.0f, 0.0f, //parallil
		-5.0f, 0.0f, 0.0f,
		-5.0f, -2.5f, 0.0f,
		-5.0f, -2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f,
		 5.0f, 0.0f, 0.0f,
		 -5.0f, 2.5f, 5.0f, //mprosta
		 -2.5f, 2.5f, 5.0f,
		 -2.5f, 0.0f, 5.0f,
		 -2.5f, 0.0f, 5.0f,
		 -5.0f, 0.0f, 5.0f,
		 -5.0f, 2.5f, 5.0f,
		 2.5f, 2.5f, 5.0f,
		 5.0f, 2.5f, 5.0f,
		 5.0f, 0.0f, 5.0f,
		 5.0f, 0.0f, 5.0f,
		 2.5f, 0.0f, 5.0f,
		 2.5f, 2.5f, 5.0f,
		 -5.0f, 0.0f, 5.0f,
		  5.0f, 0.0f, 5.0f,
		  5.0f, -2.5f, 5.0f,
		  5.0f, -2.5f, 5.0f,
		 -5.0f, -2.5f, 5.0f,
		 -5.0f, 0.0f, 5.0f,
		 -5.0f, 2.5f, 0.0f,//pano 1o aristero paral
	    -2.5f, 2.5f, 0.0f,
	    -2.5f, 2.5f, 5.0f,
	    -2.5f, 2.5f, 5.0f, 
	    -5.0f, 2.5f, 5.0f,
	    -5.0f, 2.5f, 0.0f,
	     2.5f, 2.5f, 0.0f,//pano deji paral
	     5.0f, 2.5f, 0.0f,
	     5.0f, 2.5f, 5.0f,
	     5.0f, 2.5f, 5.0f,
	     2.5f, 2.5f, 5.0f,
	     2.5f, 2.5f, 0.0f,
	     -2.5f, 0.0f, 0.0f,  //pano mesa kentriko tetragvno
	     2.5f, 0.0f, 0.0f,
	     2.5f, 0.0f, 5.0f,
	     2.5f, 0.0f, 5.0f,
	     -2.5f, 0.0f, 5.0f,
	     -2.5f, 0.0f, 0.0f,
	    -2.5f, 2.5f, 0.0f, //pano mesa aristero paral
	    -2.5f, 0.0f, 0.0f,
	    -2.5f, 0.0f, 5.0f, 
	     -2.5f, 0.0f, 5.0f,
	     -2.5f, 2.5f, 5.0f,
	     -2.5f, 2.5f, 0.0f,
	     2.5f, 0.0f, 0.0f,//dejio plaino mesa
	     2.5f, 2.5f, 0.0f,
	     2.5f, 2.5f, 5.0f,
	     2.5f, 2.5f, 5.0f,
	     2.5f, 0.0f, 5.0f,
	     2.5f, 0.0f, 0.0f,

	    

	};

	static const GLfloat uv_buffer7[] = {

		0.24f, 0.48f,  //katw
		0.24f, 0.24f,
		0.72f, 0.48f,
		0.72f, 0.24f,
		0.72f, 0.48f,
		0.24f, 0.24f,
		0.0f, 0.24f, //plai aristera
		0.24f, 0.24f,
		0.24f, 0.48f,
		0.24f, 0.48f,
		0.0f, 0.48f,
		0.0f, 0.24f,
		1.0f, 0.48f, //plai dejia
		0.72f, 0.48f,
		1.0f, 0.24f,
		0.72f, 0.48f,
		1.0f, 0.24f,
		0.72f, 0.24f,
		0.24f, 0.72f, //pisw
		0.36f, 0.72f,
		0.36f, 0.6f,
		0.36f, 0.6f,
		0.24f, 0.6f,
		0.24f, 0.72f,
		0.6f, 0.72f,
		0.72f, 0.72f,
		0.72f, 0.6f,
		0.72f, 0.6f,
		0.6f, 0.6f,
		0.6f, 0.72f,
		0.24f, 0.6f,
		0.72f, 0.6f,
		0.72f, 0.48f,
		0.72f, 0.48f,
		0.24f, 0.48f,
		0.24f, 0.6f,
		0.24f, 0.0f, //mprosta
		0.36f, 0.0f,
		0.36f, 0.12f,
		0.36f, 0.12f,
		0.12f, 0.12f,
		0.24f, 0.0f,
		0.6f, 0.0f,
		0.72f, 0.0f,
		0.72f, 0.12f,
		0.72f, 0.12f,
		0.6f, 0.12f,
		0.6f, 0.0f,
		0.12f, 0.12f,
		0.72f, 0.12f,
		0.72f, 0.24f,
		0.72f, 0.24f,
		0.24f, 0.24f,
		0.12f, 0.12f,
		0.24f, 1.0f,
		0.36f, 1.0f,
		0.36f, 0.72f,
		0.36f, 0.72f,
		0.24f, 0.72f,
		0.24f, 1.0f,
		0.6f, 1.0f,
		0.72f, 1.0f,
		0.72f, 0.72f,
		0.72f, 0.72f,
		0.6f, 0.72f,
		0.6f, 1.0f,
		0.36f, 0.6f,
		0.6f, 0.6f,
		0.6f, 0.12f,
		0.6f, 0.12f,
		0.36f, 0.12f,
		0.36f, 0.6f,
		0.36f, 0.72f,
		0.36f, 0.6f,
		0.36f, 0.12f,
		0.36f, 0.12f,
		0.36f, 0.0f,
		0.36f, 0.72f,
		0.36f, 0.1f,
		0.36f, 0.72f,
		0.36f, 0.72f,
		0.36f, 0.72f,
		0.36f, 0.1f,
		0.36f, 0.1f,

	};

    static const GLfloat inter[] = {	

    	-2.5f, 2.5f, 5.0f,  
    	2.5f, 2.5f, 5.0f,  
    	2.5f, 0.0f, 5.0f,

	   	2.5f, 0.0f, 5.0f,  
    	-2.5f, 0.0f, 5.0f,  
    	-2.5f, 2.5f, 5.0f,

    	-2.5f, 2.5f, 0.0f,  
    	-2.5f, 2.5f, 5.0f,
    	-2.5f, 0.0f, 5.0f,

    	-2.5f, 0.0f, 5.0f,
    	-2.5f, 0.0f, 0.0f,
    	-2.5f, 2.5f, 0.0f,

    	2.5f, 2.5f, 0.0f,   
    	-2.5f, 2.5f, 0.0f,
    	-2.5f, 0.0f, 0.0f,

    	-2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 0.0f,
    	2.5f, 2.5f, 0.0f,

    	2.5f, 2.5f, 5.0f,  
    	2.5f, 2.5f, 0.0f,
    	2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 5.0f,
    	2.5f, 2.5f, 5.0f,
        -2.5f, 0.0f, 5.0f, 
    	-2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 0.0f,
    	2.5f, 0.0f, 5.0f,
    	-2.5f, 0.0f, 5.0f,
    	2.5f, 2.5f, 5.0f,
    	-2.5f, 2.5f, 5.0f,
    	-2.5f, 2.5f, 0.0f,
    	-2.5f, 2.5f, 0.0f,
    	2.5f, 2.5f, 0.0f,
    	2.5f, 2.5f, 5.0f


    };


	static const GLfloat uv_buffer9[] = {

    0.25f, 0.32f,
    0.75f, 0.32f,
    0.75f, 0.48f,
    0.75f, 0.48f,
    0.25f,0.48f,
    0.25f, 0.32f,
    0.0f, 0.48f,  //plai aristera
    0.25f, 0.48f,
    0.25f, 0.8f,
    0.25f, 0.8f,
    0.0f, 0.8f,
    0.0f, 0.48f,
    0.25f, 0.8f,  //pisw
    0.75f, 0.8f,
    0.75f, 1.0f,
    0.75f, 1.0f,
    0.25f, 1.0f,
    0.25f, 0.8f,
    0.75f, 0.48f,  //plai dejia
    1.0f, 0.48f,
    1.0f, 0.8f,
    1.0f, 0.8f,
    0.75f, 0.8f,
    0.75f, 0.48f,
    0.75f, 0.8f,  //katw
    0.75f, 0.48f,
    0.25f, 0.48f,
    0.25f, 0.48f,
    0.25f, 0.8f,
    0.75f, 0.8f,
    0.75f, 0.0f,
    0.25f, 0.0f,
    0.25f, 0.32f,
    0.25f, 0.32f,
    0.75f, 0.32f,
    0.75f, 0.0f,

	};


	int height,width,numChannels;
	unsigned char* data = stbi_load("texture-model-A.jpg", &width, &height, &numChannels, 0);

	int height1,width1,numChannels1;
	unsigned char* data1 = stbi_load("texture-model-B.jpg", &width1, &height1, &numChannels1, 0);
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	

	do {

		int i;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			i=1;
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			i=2;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			i=3;
		}

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			i=4;
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			i=5;
		}



		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(figure), figure, GL_STATIC_DRAW);


		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer1), uv_buffer1, GL_STATIC_DRAW);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
	
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0,36); 


		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		

		GLuint vertexbuffer2;
		glGenBuffers(1, &vertexbuffer2);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(figure2), figure2, GL_STATIC_DRAW);
		
		GLuint uvbuffer3;
		glGenBuffers(1, &uvbuffer3);
		glBindBuffer(GL_ARRAY_BUFFER,uvbuffer3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer2), uv_buffer2, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);



		glDrawArrays(GL_TRIANGLES, 0,36); 


		
////////////////////////erotima iv//////////////////////////
////////////// a ///////////

		if (i==1) {
        		
    		
				
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			

			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(figure), figure, GL_STATIC_DRAW);


			GLuint uvbuffer;
			glGenBuffers(1, &uvbuffer);
			glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer1), uv_buffer1, GL_STATIC_DRAW);
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Use our shader
			glUseProgram(programID);
				
					
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glDrawArrays(GL_TRIANGLES, 0,36); 	

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					

			GLuint vertexbuffer2;
			glGenBuffers(1, &vertexbuffer2);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
			glBufferData(GL_ARRAY_BUFFER, sizeof(figure2), figure2, GL_STATIC_DRAW);
					
			GLuint uvbuffer3;
			glGenBuffers(1, &uvbuffer3);
			glBindBuffer(GL_ARRAY_BUFFER,uvbuffer3);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer2), uv_buffer2, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);



			glDrawArrays(GL_TRIANGLES, 0,36); 

				
    				
		}  

   		//////////// d ////////////(B-A)
   		if (i==4) {


			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					

			GLuint vertexbuffer4;
			glGenBuffers(1, &vertexbuffer4);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
			glBufferData(GL_ARRAY_BUFFER, sizeof(DIFF2), DIFF2, GL_STATIC_DRAW);


			GLuint uvbuffer4;
			glGenBuffers(1, &uvbuffer4);
			glBindBuffer(GL_ARRAY_BUFFER,uvbuffer4);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer5), uv_buffer5, GL_STATIC_DRAW);
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Use our shader
			glUseProgram(programID);
				
					
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer4);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				glDrawArrays(GL_TRIANGLES, 0,36); 	
		}
		///////////////c/////

		if (i==3) {
					

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					
			GLuint vertexbuffer5;
			glGenBuffers(1, &vertexbuffer5);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
			glBufferData(GL_ARRAY_BUFFER, sizeof(DIFF1), DIFF1, GL_STATIC_DRAW);

			GLuint uvbuffer6;
			glGenBuffers(1, &uvbuffer6);
			glBindBuffer(GL_ARRAY_BUFFER,uvbuffer6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer7), uv_buffer7, GL_STATIC_DRAW);
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Use our shader
			glUseProgram(programID);
				
					
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer6);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glDrawArrays(GL_TRIANGLES, 0,132); 	
				

        	 
    	}  

    	if (i==2) {

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					

			GLuint vertexbuffer10;
			glGenBuffers(1, &vertexbuffer10);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer10);
			glBufferData(GL_ARRAY_BUFFER, sizeof(inter), inter, GL_STATIC_DRAW);


			GLuint uvbuffer8;
			glGenBuffers(1, &uvbuffer8);
			glBindBuffer(GL_ARRAY_BUFFER,uvbuffer8);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer9), uv_buffer9, GL_STATIC_DRAW);
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Use our shader
			glUseProgram(programID);
				
					
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer10);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer8);
			glVertexAttribPointer(
				1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
			glDrawArrays(GL_TRIANGLES, 0,36); 	
				

        	 
    	}  
       				
		glDisableVertexAttribArray(0);

		camera_function();
        glm::mat4 MVP = Projection * View * Model;
		
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		
		
	} 
	// Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

}

