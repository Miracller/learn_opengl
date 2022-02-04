#include "LightSpot.h"

LightSpot::LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color):
	position(_position),
	angles(_angles),
	color(_color)
{
	constant = 1.0f; // distance = 50
	linear = 0.09f;
	quadratic = 0.032f;
	UpdateDirection();
}

void LightSpot::UpdateDirection() {
	direction = glm::vec3(0, 0, 1.0f);	// pointing to z (forward)
	direction = glm::rotateZ(direction, angles.z);  // unity: Z X Y  
	direction = glm::rotateX(direction, angles.x);  // unity: Z X Y  
	direction = glm::rotateY(direction, angles.y);  // unity: Z X Y

	direction = -1.0f * direction;
}


LightSpot::~LightSpot()
{
}
