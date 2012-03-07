#ifndef RT2D_H
#define RT2D_H

#include "core/graphics/rendertexture.h"

class RenderTexture2D : public RenderTexture
{
public:
	RenderTexture2D(QString name, int height, int width, IRD::Texture::Format format);
	~RenderTexture2D();

	virtual void resize(int height, int width);

	virtual bool unload();

	virtual bool isCubemap() {return false;}

	virtual IRD::Texture* getBackTexture(int i= 0);
	virtual void swap();

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0) {}
private:
	IRD::Texture* m_back_texture;
};

#endif // RT2D_H
