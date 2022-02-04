#include "Material.h"

Material::Material(Shader* _shader, unsigned int _diffuse, unsigned int _specular, glm::vec3 _ambient, float _shininess, unsigned int _emission):
	shader(_shader),
	diffuse(_diffuse),
	specular(_specular),
	ambient(_ambient),
	shininess(_shininess),
	emission(_emission)
{
	
}

Material::Material(Shader* _shader, glm::vec3 _ambient, float _shininess, unsigned int _emission):
	shader(_shader),
	ambient(_ambient),
	shininess(_shininess),
	emission(_emission)
{
}


Material::~Material() {

}