#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include "core/graphics/texture.h"
#include "core/graphics/shader.h"
#include "core/graphics/material.h"
#include "core/maths/vector3.h"

#include <QMatrix4x4>
#include <QVector2D>

class IRenderable;
class GLWidget;
class Viewpoint;
class Camera;
class SceneGraph;
class RenderTarget;
class RenderTexture;
class FrameBufferObject;

class RenderManager : public Manager<IRenderable>
{
	friend class Singleton<RenderManager>;

public :

	enum BackgroundType
	{
		NO_CLEAR,
		COLOR,
		SINGLE_TEXTURE,
		SKYBOX
	};

	struct Background
	{
		BackgroundType type;
		Texture textures[6];
		Vector3f color;
	};

	enum RenderPassType
	{
		CAST_SHADOW_PASS,
		RECEIVE_SHADOW_PASS,
		FINAL_PASS
	};

	struct RenderPassInfo
	{
		RenderPassType type;
		bool setup_texture_matrices;
		Material forced_material;

		bool isMaterialOverridden() {return forced_material.isValid();}
	};

protected :
	RenderManager();

private :
	GLWidget* m_context;
	Camera* m_camera;
	bool m_cameraChanged;
	bool m_drawDebug;
	Background m_defaultBackground;

	QHash<QString, ShaderProgramData::UniformBase*> m_engine_uniforms;
	// engine uniforms data
	QMatrix4x4* m_inverse_transpose_camera;
	QVector2D* m_screen_size;

	// shadowmap render textures
	RenderTexture* m_shadowmap;
	RenderTexture* m_bloommap;
	RenderTexture* m_colormap;
	FrameBufferObject* m_postprocessfbo;

	QList<RenderTarget*> m_rts;

	void renderTarget(SceneGraph* sg, RenderTarget& target, RenderPassInfo& pass_info);
	void postprocessPass(RenderTexture* texture, Material material);
	void debugDisplayTexture(Texture texture, int x, int y, int width, int height);
	void setupProjection(RenderTarget& target, int projection_nb);
	void applyBackground(RenderTarget& target, int projection_nb);

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void createResources();
	void render(double elapsed_time, SceneGraph* sg);

	void setCurrentCamera(Camera* cam);
	void setDrawDebug(bool draw);

	void setBackground(const Background& background);

	const Camera* getCurrentCamera();

	void addRenderTarget(RenderTarget* rt);

	const QHash<QString, ShaderProgramData::UniformBase*>& getEngineUniforms();
	};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
