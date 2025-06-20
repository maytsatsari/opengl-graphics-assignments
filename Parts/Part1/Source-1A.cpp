

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(900, 900, "Πρώτη άσκηση 2023", NULL, NULL);


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

	// Dark blue background
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");
	

	
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	
	
	
	static const GLfloat shape_1_buffer[] = {
		 
		//center square
        -1.5f, -1.5f, 0.0f, //  0   
    	-1.5f, 1.5f, 0.0f, //   1
    	1.5f, -1.5f, 0.0f, // 2
   		1.5f, 1.5f, 0.0f, //3
    	-1.5f, 1.5f, 0.0f, //4
    	//top right square
    	1.5f, 1.5f, 0.0f, //5 
    	1.5f,4.5f,0.0f,   //6
    	4.5f,1.5f,0.0f, // 7
    	4.5f, 4.5f, 0.0f, //8
    	1.5f, 4.5f, 0.0f, //9
    	//bottom left square
    	-4.5f, -4.5f, 0.0f, //10 
    	-1.5f,-4.5f,0.0f, //11
    	-4.5f,-1.5f,0.0f, //12
    	-1.5f, -1.5f, 0.0f, //13
    	-1.5f, -4.5f, 0.0f,  //14
    	 //top left square
    	-4.5f, 1.5f, 0.0f, // 15  
    	-4.5f,4.5f,0.0f,  // 16
    	-1.5f,1.5f,0.0f,  // 17
    	-1.5f, 4.5f, 0.0f, //18
    	-4.5f, 4.5f, 0.0f,  //19
    	//bottom right square
    	1.5f, -4.5f, 0.0f, //20  
    	1.5f,-1.5f,0.0f,  //21
    	4.5f,-4.5f,0.0f,  //22
    	4.5f, -1.5f, 0.0f, //23
    	1.5f, -1.5f, 0.0f //24
 	
    	
	};

	

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_1_buffer), shape_1_buffer, GL_STATIC_DRAW);
	

	int i = 1;
	float patternSwitchRate = 1.0f; //set the initial switching rate

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε

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

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
	        patternSwitchRate += 0.2f; //increase switching rate when U is pressed
	    }

	    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	        patternSwitchRate -= 0.2f; //decrease switching rate when D is pressed
	    }

	    //Make sure the switching rate is not negative or too small
	    if (patternSwitchRate < 0.1f) {
	        patternSwitchRate = 0.1f;
	    }

		usleep((int)(patternSwitchRate * 1000000)); //convert the switching rate to microseconds

        if (i == 1 || i == 3 || i == 5 || i == 7) {
            // Draw the triangle 
            glDrawArrays(GL_TRIANGLES, 0, 3); //starts from buffer's element 0
            glDrawArrays(GL_TRIANGLES, 2, 3); //starts from buffer's element 2
            sleep(1);
        }

        if (i == 2) {
            // 2nd attribute buffer : vertices
            glEnableVertexAttribArray(0);

            // Draw the triangle 
            glDrawArrays(GL_TRIANGLES, 5, 3); //starts from buffer's element 5
            glDrawArrays(GL_TRIANGLES, 7, 3); //starts from buffer's element 7
            sleep(1);
        }

        if (i == 4) {
            // 3rd attribute buffer : vertices
            glEnableVertexAttribArray(0);

            // Draw the triangle 
            glDrawArrays(GL_TRIANGLES, 10, 3); //starts from buffer's element 10
            glDrawArrays(GL_TRIANGLES, 12, 3); //starts from buffer's element 12
            sleep(1);
        }

         if (i == 6) {
            // 4th attribute buffer : vertices
            glEnableVertexAttribArray(0);

            // Draw the triangle 
            glDrawArrays(GL_TRIANGLES, 15, 3); //starts from buffer's element 15
            glDrawArrays(GL_TRIANGLES, 17, 3); //starts from buffer's element 17
            sleep(1);
        }

         if (i == 8) {
            // 5th attribute buffer : vertices
            glEnableVertexAttribArray(0);

            // Draw the triangle 
            glDrawArrays(GL_TRIANGLES, 20, 3); //starts from buffer's element 20
            glDrawArrays(GL_TRIANGLES, 22, 3); //starts from buffer's element 22
            sleep(1);
        }

        
        i++; // increase i

        
        if (i > 8) {
            i = 1;
        }  

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } while (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



