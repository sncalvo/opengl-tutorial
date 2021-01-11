#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	unsigned int ID;
	Texture(const char* texturePath);
	void activateTextureAs(GLenum textureUnit);
};

#endif
