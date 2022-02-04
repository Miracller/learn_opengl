#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"

#pragma region Model Data
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion

#pragma region Camera Declare
//Instantiate Camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
// 官方的欧拉角计算方法
//Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(270.0f), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(-15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion



#pragma region Input Declare
float lastX;
float lastY;
bool firstMouse = true;


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);

	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.speedZ = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.speedX = 1.0f;
	}	
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.speedX = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.speedY = 1.0f;
	}	
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.speedY = -1.0f;
	}
	else
	{
		camera.speedZ = 0;
		camera.speedX = 0;
		camera.speedY = 0;
	}

}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse == true)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;
	
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(deltaX, deltaY);
	//printf("%f \n", deltaX);
}
#pragma endregion

unsigned int LoadImageToGPU(const char* filename, GLint internalRGB, GLenum format, int textureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);
	
	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("load image failed");
	}
	stbi_image_free(data);

	return TexBuffer;
}

int main(int argc, char* argv[]) {

	std::string exePath = argv[0];
	exePath = exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj";
	//std::cout << exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\nanosuit.obj" << std::endl;

#pragma region Light Declare
	std::vector<std::string> lightPName;
	std::vector<LightPoint> lightP_Set;

	LightDirectional lightD(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(glm::radians(90.0f), glm::radians(0.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));

	LightPoint lightP0(glm::vec3(10.0f, 10.0f, 0), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));
	LightPoint lightP1(glm::vec3(0, 10.0f, 10.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));
	LightPoint lightP2(glm::vec3(10.0f, 0, 10.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));
	LightPoint lightP3(glm::vec3(1.0f, 12.0f, 1.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));

	lightP_Set.push_back(lightP0);
	lightP_Set.push_back(lightP1);
	lightP_Set.push_back(lightP2);
	lightP_Set.push_back(lightP3);

	for (int i = 0; i < lightP_Set.size(); i++)
	{
		std::string tempStr = "lightP" + std::to_string(i);
		lightPName.push_back(tempStr);
		//lightP_Set.push_back();  //string -> const char* : .c_str()
		//std::cout << lightPName[i] << std::endl;
	}


	LightSpot lightS(glm::vec3(0, 10.0f, 0), glm::vec3(glm::radians(90.0f), glm::radians(0.0f), 0),
		glm::vec3(1.0f, 1.0f, 1.0f));


#pragma endregion

	

	#pragma region Open a Window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Open GLFW Window
		GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
		if (window == NULL) {
			printf("Open window failed!");
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);

		//Init glew
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			printf("Init GLEW failed.");
			glfwTerminate();
			return -1;
		}
	
		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);
	#pragma endregion

	#pragma region Init Shader
		Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
	#pragma endregion

	#pragma region Init Material 
		//Material* myMaterial = new Material(myShader,
		//	LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),
		//	LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	64.0f,
		//	LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB, 2)
		//);
		Material* myMaterial = new Material(myShader,
			glm::vec3(1.0f, 1.0f, 1.0f),
			64.0f,
			LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB, 2)
		);
	#pragma endregion

	#pragma region Init and Load Models to VAO, VBO
		//Mesh cube(vertices);
		Model model(exePath);
		//unsigned int VAO;
		//glGenVertexArrays(1, &VAO);
		//glBindVertexArray(VAO);
		//
		//unsigned int VBO;
		//glGenBuffers(1, &VBO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
		////glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);



	#pragma endregion

	#pragma region Init and Load Textures
		//unsigned int TexBufferA;
		//TexBufferA = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);	
		//unsigned int TexBufferB;
		//TexBufferB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 0);
	#pragma endregion

	#pragma region Prepare MVP Matrices
		//Mt * Mr * Ms
		glm::mat4 modelMat;
		glm::mat4 viewMat;
		glm::mat4 projMat;
		projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	#pragma endregion

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// Process Input
		processInput(window);//处理用户输入
		 
		// Clear Screen
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		viewMat = camera.GetViewMatrix();
		
		for (int i = 0; i < 1; i++)
		{
			// Set Model matrix 
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);

			// If you want, Set View and Projection Matrices here

			// material ball
			// Set Material(3 parts, 1) -> Shader Program  
			myShader->use();
			// Set Material(2) -> Textures
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
			//glActiveTexture(GL_TEXTURE0 + 1);
			//glBindTexture(GL_TEXTURE_2D, myMaterial->specular);
			// emission
			//glActiveTexture(GL_TEXTURE0 + 2);
			//glBindTexture(GL_TEXTURE_2D, myMaterial->emission);
			// Set Material(3) -> Uniforms
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 3);
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(myShader->ID, "objColor"), 1.0f, 1.0f, 1.0f); //coral color
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			
#pragma region Light setting
			glUniform3f(glGetUniformLocation(myShader->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);  // light position 
			glUniform3f(glGetUniformLocation(myShader->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);  // light color
			glUniform3f(glGetUniformLocation(myShader->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);  // light color

			for (int index = 0; index < lightP_Set.size(); index++)
			{
				glUniform3f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".pos").c_str()), lightP_Set[index].position.x, lightP_Set[index].position.y, lightP_Set[index].position.z);  // light position 
				glUniform3f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".color").c_str()), lightP_Set[index].color.x, lightP_Set[index].color.y, lightP_Set[index].color.z);  // light color
				glUniform3f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".dirToLight").c_str()), lightP_Set[index].direction.x, lightP_Set[index].direction.y, lightP_Set[index].direction.z);  // light color
				glUniform1f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".constant").c_str()), lightP_Set[index].constant);
				glUniform1f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".linear").c_str()), lightP_Set[index].linear);
				glUniform1f(glGetUniformLocation(myShader->ID, (lightPName[index] + ".quadratic").c_str()), lightP_Set[index].quadratic);
			}

			glUniform3f(glGetUniformLocation(myShader->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);  // light position 
			glUniform3f(glGetUniformLocation(myShader->ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);  // light color
			glUniform3f(glGetUniformLocation(myShader->ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);  // light color
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.constant"), lightS.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.linear"), lightS.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.quadratic"), lightS.quadratic);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyInner"), lightS.cosPhyInner);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyOuter"), lightS.cosPhyOuter);
#pragma endregion

			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyInner"), light.cosPhyInner);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyOuter"), light.cosPhyOuter);

			//glUniform3f(glGetUniformLocation(myShader->ID, "lightColor"), sin(glfwGetTime()*2.0f), sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f));
			glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			
			//myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			//myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
			//myMaterial->shader->SetUniform3f("material.specular", myMaterial->specular);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

			// extend test: emission
			//myMaterial->shader->SetUniform1f("offset", glfwGetTime());
			//myMaterial->shader->SetUniform1i("material.emission", 2);

			//glUniform3f(glGetUniformLocation(myShader->ID, "material.ambient"), myMaterial->ambient);
			//glUniform1f(glGetUniformLocation(myShader->ID, "material.shininess"), myMaterial->shininess);

			// Set Model
			//glBindVertexArray(VAO);
			
			//cube.Draw(myMaterial->shader);
			model.Draw(myMaterial->shader);
			// Drawcall
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	
		// Clear up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();//获取用户输入
		camera.UpdateCameraPos();
	}

	// Exit program
	glfwTerminate();
	return 0;
}