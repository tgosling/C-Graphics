#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->scale = 1.0f;
	this->isWireframe = false;

	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);

	return;
}

cMeshObject::~cMeshObject()
{
	return;
}