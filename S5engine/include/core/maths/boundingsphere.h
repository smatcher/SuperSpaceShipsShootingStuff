// BoundingSphere.h

#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "core/maths/boundingvolume.h"

class BoundingSphere : public BoundingVolume
{
protected:
	float radius;

public:
	BoundingSphere(const Vector3f& center=Vector3f(0.0, 0.0, 0.0), float radius=1.0);
	BoundingSphere(const BoundingSphere& ref);
	virtual ~BoundingSphere();

	BoundingSphere& operator=(const BoundingSphere& ref);

	virtual bool pointIsInside(const Vector3f& point, const Vector3f& object_position=Vector3f(0.0, 0.0, 0.0)) const;
	virtual bool pointIsInside(const Vector3d& point, const Vector3d& object_position=Vector3d(0.0, 0.0, 0.0)) const;

	// Check if the volume intersects or is inside a given frustum.
	// object_position : position of the bounded object
	virtual bool isInsideFrustum(	const Frustum& frustum,
									const Vector3f& object_position = Vector3f(0.0, 0.0, 0.0)) const;

	void setRadius(float radius) {this->radius = radius;}
	float getRadius() const {return radius;}

	virtual Type getType() {return SPHERE;}
};

#endif // BOUNDING_SPHERE_H
