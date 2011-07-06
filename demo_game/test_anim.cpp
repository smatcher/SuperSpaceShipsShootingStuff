#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/managers/rendermanager.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	Node* nCam = new Node("Camera");
	Node* nLight = new Node("Light");
	Node* nActor = new Node("Actor");
	Node* nBody = new Node("Body");

	nCam->addProperty(new Camera(70,1,200));
	nLight->addProperty(new Light());

	sg->link(nActor);
	sg->link(nCam);

	nActor->link(nBody);

	sg->link(nLight);

	nCam->moveTo(Vector3f(2,0,0));
	nCam->rotate(Vector3f(0,1,0),90);

	nActor->setScale(Vector3f(0.04,0.04,0.04));

	nLight->moveTo(Vector3f(2,0,-1));

	Mesh bob = MESH_MANAGER.get("Bob");
	Material material = MATERIAL_MANAGER.get("bob");

	nBody->addProperty(new MeshRenderer(bob, material));

	// Beurk ! Mais je peux le faire alors je me prive pas ^^
	RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCam->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::COLOR;
	background.color = Vector3f(0.2,0.2,0.2);

	RENDER_MANAGER.setBackground(background);

	int ret = engine.start();

	return ret;
}