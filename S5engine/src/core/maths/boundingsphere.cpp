// boundingsphere.cpp

#include "core/maths/boundingsphere.h"
#include "core/maths/frustum.h"
#include "core/maths/constants.h"

BoundingSphere::BoundingSphere(const Vector3f& center, float radius) : BoundingVolume(center)
{
	this->radius=radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& ref) : BoundingVolume()
{
	*this = ref;
}

BoundingSphere::~BoundingSphere()
{
}

BoundingSphere& BoundingSphere::operator=(const BoundingSphere& ref)
{
	BoundingVolume::operator=(ref);
	this->radius = ref.radius;
	return *this;
}

bool BoundingSphere::pointIsInside(const Vector3f& point, const Vector3f& object_position) const
{
	return (((point - (object_position + center)).norm()) - EPSILON < radius);
}

bool BoundingSphere::pointIsInside(const Vector3d& point, const Vector3d& object_position) const
{
	return (((point - (object_position + center)).norm()) - EPSILON < radius);
}

// Check if the volume intersects or is inside a given frustum.
// object_position : position of the bounded object
bool BoundingSphere::isInsideFrustum(const Frustum& frustum, const Vector3f& object_position) const
{
	return frustum.isInside(center + object_position, radius);
}
