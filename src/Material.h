#pragma once

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	Shader* shader;
	unsigned int diffuse;
	unsigned int specular;
	glm::vec3 ambient;
	float shininess;
	unsigned int emission;

	Material(Shader* _shader, unsigned int _diffuse, unsigned int _specular, glm::vec3 _ambient, float _shininess, unsigned int _emission);
	Material(Shader* _shader,  glm::vec3 _ambient, float _shininess, unsigned int _emission);
	~Material();
};

