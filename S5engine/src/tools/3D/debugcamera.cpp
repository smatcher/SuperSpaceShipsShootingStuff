#include "tools/3D/debugcamera.h"
#include "core/maths/transform.h"
#include "core/log/log.h"
#include <GL/glu.h>
#include <QMatrix4x4>

#include <QDateTime>

const double DebugCamera::COEFF_ROTATION = 0.3;
const double DebugCamera::COEFF_TRANSLATION = 10;
const double DebugCamera::DEFAULT_ROTATION_X = 30.0;
const double DebugCamera::DEFAULT_ROTATION_Y = 135.0;

// Constructeur
DebugCamera::DebugCamera()
: zoom(1.0), mode(MODE_NONE), original_mouse_x(0), original_mouse_y(0),
rotation_x(DEFAULT_ROTATION_X), rotation_y(DEFAULT_ROTATION_Y),
original_rotation_x(rotation_x), original_rotation_y(rotation_y)
{
	for(int i=0 ; i<6 ; i++)
		keys[i] = false;

	target = Vector3f(-3,3,-3);
}

// Destructeur
DebugCamera::~DebugCamera()
{
}

void DebugCamera::updateKeyboard(Key key, bool pressed)
{
	keys[key] = pressed;
}

// Mise   jour de la caméra
void DebugCamera::updateMouse(bool left_pressed, bool right_pressed, bool middle_pressed,
						 int x, int y)
{
	zoom = right_pressed?20:1; // debugcamera.cpp:39
	// Cas où l'on passe du mode NONE   un autre :
	if(mode == MODE_NONE)
	{
		if(left_pressed)	// Passage au mode ROTATION :
		{
			// Enregistrement de l'état actuel (début de la rotation)
			mode = MODE_ROTATION;
			original_mouse_x = x;
			original_mouse_y = y;
		}
		return;
	}

	// MODE ROTATION :
	if(mode == MODE_ROTATION)
	{
		// Bouton appuyé => on met   jour de combien on tourne
		if(left_pressed)
		{
			rotation_x = original_rotation_x + COEFF_ROTATION*(y - original_mouse_y);
			rotation_y = original_rotation_y + COEFF_ROTATION*(x - original_mouse_x);
		}
		// Bouton relâché : on enregistre nos modifs et on repasse au mode NONE
		else
		{
			original_rotation_x = rotation_x;
			original_rotation_y = rotation_y;
			mode = MODE_NONE;
		}
		return;
	}
}

int DebugCamera::getNbProjections()
{
	return 1;
}

Viewpoint::Style DebugCamera::getStyle()
{
	return MONO;
}

Matrix4d DebugCamera::getViewProjection(double aspect, int projection_nb)
{
	Matrix4d projection = getProjection(aspect, projection_nb);
	Transformd viewTrans;
	viewTrans.move(Vector3d(-target.x, -target.y, -target.z));
	viewTrans.rotate(Vector3d(0.0,1.0,0.0),rotation_y);
	viewTrans.rotate(Vector3d(1.0,0.0,0.0),rotation_x);
	Matrix4d view(viewTrans);
	debug("MATRIX_STACK", "get viewprojection : view" << QMatrix4x4(view.values));
	debug("MATRIX_STACK", "get viewprojection : viewprojection" << QMatrix4x4((projection*view).values));
	return projection * view;
}

const Matrix4d& DebugCamera::getProjection(double aspect, int projection_nb)
{
	Matrix4d mat;

	const float yfov = 70;
	const float znear = 0.01;
	const float zfar = 1000;
	const float h = 1.0f/tan(yfov*M_PI/360);
	float neg_depth = znear-zfar;

	mat[0] = h / aspect;
	mat[1] = 0;
	mat[2] = 0;
	mat[3] = 0;

	mat[4] = 0;
	mat[5] = h;
	mat[6] = 0;
	mat[7] = 0;

	mat[8] = 0;
	mat[9] = 0;
	mat[10] = (zfar + znear)/neg_depth;
	mat[11] = -1;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 2.0f*(znear*zfar)/neg_depth;
	mat[15] = 0;

	return mat;
}

void DebugCamera::setProjection(double aspect, double scale, int projection_nb)
{
	Matrix4d mat = getProjection(aspect, projection_nb);
	glLoadMatrixd(mat);
	/*
	gluPerspective(70,aspect,0.01,1000);
	*/
	glScaled(scale, scale, 1);
}

// Mise   jour de la matrice modelview
void DebugCamera::applyTransform(int projection_nb)
{
	// Compute elapsed time
	static QDateTime lastTime = QDateTime::currentDateTime();
	QDateTime time = QDateTime::currentDateTime();
	double elapsed = double(lastTime.msecsTo(time))/1000.0;
	lastTime = time;

	// Compute translation
	Transformf temp;
	temp.rotate(Vector3f(1,0,0),-rotation_x);
	temp.rotate(Vector3f(0,1,0),-rotation_y);
	if(keys[UP])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,0,-1));
	if(keys[DOWN])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,0,1));
	if(keys[LEFT])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(-1,0,0));
	if(keys[RIGHT])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(1,0,0));
	if(keys[RCTRL])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,-1,0));
	if(keys[RSHIFT])
		target = target + zoom*COEFF_TRANSLATION*elapsed*temp.apply(Vector3f(0,1,0));

	// Apply matrix
	glRotated(rotation_x, 1.0, 0.0, 0.0);
	glRotated(rotation_y, 0.0, 1.0, 0.0);

	glTranslated(-target.x, -target.y, -target.z);

}

void DebugCamera::applyOnlyRotation(int projection_nb)
{
	glRotated(rotation_x, 1.0, 0.0, 0.0);
	glRotated(rotation_y, 0.0, 1.0, 0.0);
}

// Retour aux valeurs par défaut
void DebugCamera::reset()
{
	zoom = 1.0;

	original_target=target=Vector3f(0,0,0);

	original_rotation_x = rotation_x = DEFAULT_ROTATION_X;
	original_rotation_y = rotation_y = DEFAULT_ROTATION_Y;
}

void DebugCamera::setRotation(double rot_x, double rot_y)
{
	original_rotation_x = rotation_x = rot_x;
	original_rotation_y = rotation_y = rot_y;
}

Vector3f DebugCamera::getWorldPosition()
{
	return Vector3f(target);
}

Frustum DebugCamera::getFrustum(int projection_nb)
{
	return Frustum();
}

IRD::Viewport DebugCamera::getViewport(int projection_nb)
{
	IRD::Viewport vp;
	vp.x = 0;
	vp.y = 0;
	vp.width = 1;
	vp.height = 1;
	vp.relative = true;
	return vp;
}
