#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"
#include "core/properties/dummycontrolable.h"
#include "core/properties/physicobject.h"
#include "core/properties/qtlogo.h"
#include "core/properties/camera.h"
#include "core/properties/light.h"
#include "core/properties/soundemitter.h"
#include "core/properties/terrainrenderer.h"
#include "core/managers/rendermanager.h"
#include "core/properties/grid.h"
#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

#include "core/inputs/inputmanager.h"

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alu.h>
#include <AL/alut.h>



int main(int argc, char *argv[])
{
	#ifdef Q_WS_X11
		XInitThreads();
	#endif

	Engine engine(argc, argv,"demo_game/media/");

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();

	QList<InputManager::Control> controls;
	controls.push_back(InputManager::Control(false,"avance"));
	controls.push_back(InputManager::Control(false,"recule"));
	controls.push_back(InputManager::Control(false,"gauche"));
	controls.push_back(InputManager::Control(false,"droite"));
	INPUT_MANAGER.addControls(controls);

	INPUT_MANAGER.addBinding("KB_Z","avance");
	INPUT_MANAGER.addBinding("KB_Up","avance");
	INPUT_MANAGER.addBinding("KB_S","recule");
	INPUT_MANAGER.addBinding("KB_Down","recule");
	INPUT_MANAGER.addBinding("KB_Q","gauche");
	INPUT_MANAGER.addBinding("KB_Left","gauche");
	INPUT_MANAGER.addBinding("KB_D","droite");
	INPUT_MANAGER.addBinding("KB_Right","droite");

	Node* nRot = new Node("Rotating node");
	Node* nQt = new Node("Qt Logo");
	Node* nCam = new Node("Camera");
	Node* nCamFollow = new Node("Camera Follow");
	Node* nHead = new Node("Head");
	Node* nStar = new Node("Star");
	Node* nLight = new Node("Light");
	Node* nLight2 = new Node("Light2");
	Node* nUni = new Node("Unicorn");
	Node* nBall = new Node("Ball");
	Node* nGarg = new Node("Gargoyle");
	Node* nWall = new Node("Wall");
	Node* nDuck = new Node("Duck");
	//Node* nDuckGrid = new Node("Grid");
	Node* nSand = new Node("Sand");
	Node* nTerrain = new Node("Terrain");

	nRot->addProperty(new IProperty());
//	nRot->addProperty(new DummyUpdatable());
//	nQt->addProperty(new QtLogo(engine.getGLW_TEMPORARY()));
//	nQt->addProperty(new Grid(1.0f, 1.0f, 40, 40));
	//nDuckGrid->addProperty(new Grid(1.0f, 1.0f, 40, 40));
	nCam->addProperty(new Camera(70,1,200));
	nCamFollow->addProperty(new Camera(90,1,200));
	nHead->addProperty(new DummyUpdatable());
	nLight->addProperty(new Light());
	//nLight2->addProperty(new Light());

	Texture heightmap = TEXTURE_MANAGER.get("heightmap.bmp");
	nTerrain->addProperty(new TerrainRenderer(heightmap, 10.0f));

	Mesh plane = MESH_MANAGER.get("Plane");
	Mesh cube = MESH_MANAGER.get("Cube");
	Mesh sphere = MESH_MANAGER.get("Sphere_16_32");
	Mesh mesh = MESH_MANAGER.get("duckmesh");
	Material duck = MATERIAL_MANAGER.get("duck");
	Material unicorn = MATERIAL_MANAGER.get("unicorn");
	Material star = MATERIAL_MANAGER.get("star");
	Material ball = MATERIAL_MANAGER.get("ball");
	Material gargoyle = MATERIAL_MANAGER.get("gargoyle");
	Material sand = MATERIAL_MANAGER.get("sand");
	Sample sample = SAMPLE_MANAGER.get("quacking.wav");

	nDuck->addProperty(new MeshRenderer(mesh,duck));
	nStar->addProperty(new MeshRenderer(plane,star));
	nBall->addProperty(new MeshRenderer(sphere,ball));
	//nUni->addProperty(new MeshRenderer(cube,unicorn));
	nGarg->addProperty(new MeshRenderer(cube,gargoyle));
	nWall->addProperty(new MeshRenderer(cube,gargoyle));
	nSand->addProperty(new MeshRenderer(cube,sand));
	nDuck->addProperty(new DummyControlable());
	nDuck->addProperty(new SoundEmitter(sample));
	nDuck->link(nCamFollow);
	nDuck->link(nHead);
	//nDuck->link(nDuckGrid);

	nHead->link(nStar);
	nHead->link(nLight);
	nHead->link(nLight2);
	nHead->link(nUni);

	sg->link(nDuck);
	sg->link(nRot);
	sg->link(nQt);
	sg->link(nBall);
	sg->link(nGarg);
	sg->link(nWall);
	sg->link(nSand);
	sg->link(nTerrain);
	nRot->link(nCam);

	nRot->moveTo(Vector3f(0,0,0));
	nQt->moveTo(Vector3f(0,-0.5,0));
	nQt->rotate(Vector3f(1,0,0),90);
	nCam->moveTo(Vector3f(0,1.0,1.6));
	nCam->rotate(Vector3f(1,0,0),330);
	nCamFollow->moveTo(Vector3f(0,2,3));
	nCamFollow->rotate(Vector3f(1,0,0),345);

	nUni->rotate(Vector3f(0,0,-1),90);
	nUni->rotate(Vector3f(-1,0,0),90);
	nHead->moveTo(Vector3f(0,0.5,0));
	nUni->moveTo(Vector3f(1.5,0,0));
	nStar->moveTo(Vector3f(-1.5,0,0));
	nStar->rotate(Vector3f(0,0,1),270);
	nLight->moveTo(Vector3f(0,0,1));
	nLight2->moveTo(Vector3f(0,0,-1));

	nSand->moveTo(Vector3f(0,-0.5,0));
	nSand->rotate(Vector3f(1,0,0),270);
	nSand->setScale(Vector3f(10,10,0.01));
	nWall->moveTo(Vector3f(0,0,-5));
	nWall->setScale(Vector3f(10,1,0.01));
	nBall->moveTo(Vector3f(0,5,0));
	nGarg->moveTo(Vector3f(-3,0,-5));
	nGarg->setScale(Vector3f(2,1,2));
	nDuck->moveTo(Vector3f(0,0.5,-1.8));
	nDuck->rotate(Vector3f(0,1,0),127);
	//nDuckGrid->rotate(Vector3f(1,0,0),270);

	PhysicObject::Properties prop;
	nSand->addProperty(new PhysicObject(prop));
	nWall->addProperty(new PhysicObject(prop));
	prop.is_kinematic = true;
	nDuck->addProperty(new PhysicObject(prop));

	prop.is_kinematic = false;
	prop.mass = 1.0;
	prop.restitution = 0.85;
	prop.shape = PhysicObject::SPHERE;
	nBall->addProperty(new PhysicObject(prop));

	// Beurk ! Mais je peux le faire alors je me prive pas ^^
	//RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCamFollow->properties().child("Camera")));
	RENDER_MANAGER.setCurrentCamera(static_cast<Camera*>(nCamFollow->properties().child("Camera")));
	RenderManager::Background background;
	background.type = RenderManager::SKYBOX;
	background.color = Vector3f(1,1,0);
	background.textures[0] = TEXTURE_MANAGER.get("stormy_front.tga");
	background.textures[1] = TEXTURE_MANAGER.get("stormy_left.tga");
	background.textures[2] = TEXTURE_MANAGER.get("stormy_back.tga");
	background.textures[3] = TEXTURE_MANAGER.get("stormy_right.tga");
	background.textures[4] = TEXTURE_MANAGER.get("stormy_top.tga");
	background.textures[5] = TEXTURE_MANAGER.get("stormy_bottom.tga");
	RENDER_MANAGER.setBackground(background);

	int ret = engine.start();

	return ret;
}
