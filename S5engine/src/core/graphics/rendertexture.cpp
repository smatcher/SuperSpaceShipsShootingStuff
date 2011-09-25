#include <GL/glew.h>

#include "core/graphics/rendertexture.h"
#include "core/resources/managers.h"

#include <QtOpenGL>

RenderTexture::RenderTexture(QString name, int height, int width, GLenum format, GLenum type) : TextureData(name, "none", NULL)
{
	m_height = height;
	m_width = width;

	m_hasgltex = true;

	m_state = STATE_LOADED;

	glGenTextures(1, &m_gltexture);
	glBindTexture(GL_TEXTURE_2D, m_gltexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	TEXTURE_MANAGER.add(this);
}

bool RenderTexture::unload()
{
	return false;
}
