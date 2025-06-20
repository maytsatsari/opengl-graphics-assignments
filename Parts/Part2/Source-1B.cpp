

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <time.h>

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


glm::mat4 View;
glm::mat4 Model;
glm::mat4 Projection;

float zoom1=40;
glm::mat4 ViewNEW;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
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
	window = glfwCreateWindow(950, 950, "Εργασία 1Β", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//grey background
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 30.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(0.0f, 0.0f, 2.5f), // and looks at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	
	//half values 
	GLfloat len = 5.0f, wid=2.5f, heig=2.5f;


	static const GLfloat cube[] = {
	
		 
		 
		 -5.0f, -2.5f, 0.0f,  //1st
		 -5.0f, -2.5f, 5.0f,
		 5.0f, -2.5f, 0.0f,
		 5.0f, -2.5f, 5.0f,
		 -5.0f, -2.5f, 5.0f,

		 -5.0f, -2.5f, 5.0f,  
		 -5.0f, 2.5f, 5.0f,
		 5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 5.0f,
		 -5.0f, 2.5f, 5.0f,

		 -5.0f, 2.5f, 0.0f,  
		 -5.0f, 2.5f, 5.0f,
		 -5.0f, -2.5f, 0.0f,
		 -5.0f, -2.5f, 5.0f,
		 -5.0f, 2.5f, 5.0f,

		 5.0f, -2.5f, 0.0f,  
		 5.0f, -2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,
		 5.0f, 2.5f, 5.0f,
		 5.0f, -2.5f, 5.0f,

		 -5.0f, -2.5f, 0.0f,  
		 -5.0f, 2.5f, 0.0f,
		 5.0f, -2.5f, 0.0f,
		 5.0f, 2.5f, 0.0f,
		 -5.0f, 2.5f, 0.0f,

		 -5.0f, 2.5f, 0.0f,  
		 -5.0f, 2.5f, 5.0f,
		 5.0f, 2.5f, 0.0f,
		 5.0f, 2.5f, 5.0f,
		 -5.0f, 2.5f, 5.0f, 
		 
		 
		 -2.5f, 14.0f, 5.0f,//PANW
		 -2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 5.0f,
		 2.5f, 14.0f, 0.0f,
		 -2.5f, 14.0f, 0.0f,


		 -2.5f, 0.0f, 5.0f,
		 -2.5f, 0.0f, 0.0f,  //PLAINH dejia
		 -2.5f, 14.0f, 5.0f,
		 -2.5f, 14.0f, 0.0f,
		 -2.5f, 0.0f, 0.0f,

		 2.5f, 0.0f, 0.0f,
		 2.5f, 14.0f, 0.0f,  //VB1WC1 mprostini
		 -2.5f, 0.0f, 0.0f,
		 -2.5f, 14.0f, 0.0f,		 
		 2.5f, 14.0f, 0.0f, 



		 2.5f, 0.0f, 0.0f,
		 2.5f, 0.0f, 5.0f, //PLAHNH aristerh VTB1Z
		 2.5f, 14.0f, 0.0f,
		 2.5f, 14.0f, 5.0f,
		 2.5f, 0.0f, 5.0f,

		  -2.5f, 0.0f, 5.0f,
		 -2.5f, 14.0f, 5.0f, //UA1TZ, pisw megalh
		 2.5f, 0.0f, 5.0f,
		 2.5f, 14.0f, 5.0f,
		 -2.5f, 14.0f, 5.0f,


		  -2.5f, 0.0f, 0.0f,  //2nd
		 -2.5f, 0.0f, 5.0f,
		 2.5f, 0.0f, 0.0f,
		 2.5f, 0.0f, 5.0f, //katv WUVT
		 -2.5f, 0.0f, 5.0f,
	 	

	};

	GLfloat a=0.5f;
	GLfloat b=1.0f;
	// One color for each vertex
	static const GLfloat color[] = {

		0.609f,  0.115f,  0.436f,a,
		0.609f,  0.115f,  0.436f,a,
		0.609f,  0.115f,  0.436f,a,
		0.609f,  0.115f,  0.436f,a,
		0.609f,  0.115f,  0.436f,a,

		0.014f,  0.184f,  0.576f,a,
		0.014f,  0.184f,  0.576f,a,
		0.014f,  0.184f,  0.576f,a,
		0.014f,  0.184f,  0.576f,a,
		0.014f,  0.184f,  0.576f,a,

		0.771f,  0.328f,  0.970f,a,
		0.771f,  0.328f,  0.970f,a,
		0.771f,  0.328f,  0.970f,a,
		0.771f,  0.328f,  0.970f,a,
		0.771f,  0.328f,  0.970f,a,

		0.971f,  0.572f,  0.833f,a,
		0.971f,  0.572f,  0.833f,a,
		0.971f,  0.572f,  0.833f,a,
		0.971f,  0.572f,  0.833f,a,
		0.971f,  0.572f,  0.833f,a,

		0.279f,  0.317f,  0.505f,a,
		0.279f,  0.317f,  0.505f,a,
		0.279f,  0.317f,  0.505f,a,
		0.279f,  0.317f,  0.505f,a,
		0.279f,  0.317f,  0.505f,a,

		0.783f,  0.290f,  0.734f,a,
		0.783f,  0.290f,  0.734f,a,
		0.783f,  0.290f,  0.734f,a,
		0.783f,  0.290f,  0.734f,a,
		0.783f,  0.290f,  0.734f,a,
		

		0.583f,  0.771f,  0.014f,b, //2nd's colors
		0.583f,  0.771f,  0.014f,b,
		0.583f,  0.771f,  0.014f,b,
		0.583f,  0.771f,  0.014f,b,
		0.583f,  0.771f,  0.014f,b,

		0.310f,  0.747f,  0.185f,b,
		0.310f,  0.747f,  0.185f,b,
		0.310f,  0.747f,  0.185f,b,
		0.310f,  0.747f,  0.185f,b,
		0.310f,  0.747f,  0.185f,b,

		0.410f,  0.947f,  0.385f,b,
		0.410f,  0.947f,  0.385f,b,
		0.410f,  0.947f,  0.385f,b,
		0.410f,  0.947f,  0.385f,b,
		0.410f,  0.947f,  0.385f,b,

		0.676f,  0.977f,  0.133f,b,
		0.676f,  0.977f,  0.133f,b,
		0.676f,  0.977f,  0.133f,b,
		0.676f,  0.977f,  0.133f,b,
		0.676f,  0.977f,  0.133f,b,

		0.543f,  0.021f,  0.978f,b,
		0.543f,  0.021f,  0.978f,b,
		0.543f,  0.021f,  0.978f,b,
		0.543f,  0.021f,  0.978f,b,
		0.543f,  0.021f,  0.978f,b,

		0.714f,  0.505f,  0.345f,b,
		0.714f,  0.505f,  0.345f,b,
		0.714f,  0.505f,  0.345f,b,
		0.714f,  0.505f,  0.345f,b,
		0.714f,  0.505f,  0.345f,b,

		
		
	};



	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shader
		glUseProgram(programID);

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 30.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f) 
		);
		
		


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

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


		glDrawArrays(GL_TRIANGLES, 0,3); // 3 indices 	
		glDrawArrays(GL_TRIANGLES, 2,3 );

		glDrawArrays(GL_TRIANGLES, 5,3 );
		glDrawArrays(GL_TRIANGLES, 7,3 );

		glDrawArrays(GL_TRIANGLES, 10,3 );
		glDrawArrays(GL_TRIANGLES, 12,3 );

		glDrawArrays(GL_TRIANGLES, 15,3 );
		glDrawArrays(GL_TRIANGLES, 17,3 );

		glDrawArrays(GL_TRIANGLES, 20,3 );
		glDrawArrays(GL_TRIANGLES, 22,3 );

		glDrawArrays(GL_TRIANGLES, 25,3 );
		glDrawArrays(GL_TRIANGLES, 27,3 );

		glDrawArrays(GL_TRIANGLES, 30,3 );
		glDrawArrays(GL_TRIANGLES, 32,3 );

		glDrawArrays(GL_TRIANGLES, 33,3); 		
		glDrawArrays(GL_TRIANGLES, 35,3 );

      // glDrawArrays(GL_TRIANGLES,0,36 );
		glDrawArrays(GL_TRIANGLES, 37,3 );
		glDrawArrays(GL_TRIANGLES, 39,3 );

		glDrawArrays(GL_TRIANGLES, 42,3 );
		glDrawArrays(GL_TRIANGLES, 44,3 );

		glDrawArrays(GL_TRIANGLES, 47,3 );
		glDrawArrays(GL_TRIANGLES, 49,3 );

		glDrawArrays(GL_TRIANGLES, 52,3 );
		glDrawArrays(GL_TRIANGLES, 54,3 );

/*		glDrawArrays(GL_TRIANGLES, 57,3 );
		glDrawArrays(GL_TRIANGLES, 59,3 ); 

		glDrawArrays(GL_TRIANGLES, 63,3 );
		glDrawArrays(GL_TRIANGLES, 65,3 );  */

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(1.05f,1.0f,1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(0.9f,1.0f,1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(1.0f,1.05f,1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(1.0f,0.9f,1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(1.0f,1.0f,1.05f));
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			Model = glm::scale(Model, glm::vec3(1.0f,1.0f,0.9f));
		}

		


		camera_function();
		
		glm::mat4 MVP = Projection * View * Model;
		glm::mat4 MVPNEW = Projection * ViewNEW * Model;  


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNEW[0][0]);

	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}




