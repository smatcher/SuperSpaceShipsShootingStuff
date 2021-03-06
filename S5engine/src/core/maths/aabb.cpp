// aabb.cpp

#include "core/maths/aabb.h"
#include "core/maths/frustum.h"

AABB::AABB(const Vector3f& center, const Vector3f& half_dimensions) : BoundingVolume(center)
{
	setHalfDimensions(half_dimensions);
}

AABB::AABB(const AABB& ref) : BoundingVolume()
{
	*this = ref;
}

AABB::~AABB()
{
}

AABB& AABB::operator=(const AABB& ref)
{
	BoundingVolume::operator=(ref);

	this->half_dimensions = ref.half_dimensions;

	for(int i=0 ; i<8 ; i++)
		this->corners[i] = ref.corners[i];
	return *this;
}

// Update the corners, thanks to the center and the dimensions
void AABB::updateCorners()
{
	// + + +
	corners[0] = Vector3f(center.x+half_dimensions.x,center.y+half_dimensions.y,center.z+half_dimensions.z);
	// - + +
	corners[1] = Vector3f(center.x-half_dimensions.x,center.y+half_dimensions.y,center.z+half_dimensions.z);
	// + - +
	corners[2] = Vector3f(center.x+half_dimensions.x,center.y-half_dimensions.y,center.z+half_dimensions.z);
	// - - +
	corners[3] = Vector3f(center.x-half_dimensions.x,center.y-half_dimensions.y,center.z+half_dimensions.z);
	// + + -
	corners[4] = Vector3f(center.x+half_dimensions.x,center.y+half_dimensions.y,center.z-half_dimensions.z);
	// - + -
	corners[5] = Vector3f(center.x-half_dimensions.x,center.y+half_dimensions.y,center.z-half_dimensions.z);
	// + - -
	corners[6] = Vector3f(center.x+half_dimensions.x,center.y-half_dimensions.y,center.z-half_dimensions.z);
	// - - -
	corners[7] = Vector3f(center.x-half_dimensions.x,center.y-half_dimensions.y,center.z-half_dimensions.z);
}

// Overloaded, to keep the corners' positions updated
void AABB::moveTo(const Vector3f& center)
{
	this->center=center;

	updateCorners();
}

bool AABB::pointIsInside(const Vector3f& point, const Vector3f& object_position) const
{
	const Vector3f absolute_center = object_position + center;

	if(point.x-absolute_center.x > half_dimensions.x || point.x-absolute_center.x < -half_dimensions.x)
		return false;
	if(point.y-absolute_center.y > half_dimensions.y || point.y-absolute_center.y  <  -half_dimensions.y)
		return false;
	if(point.z-absolute_center.z > half_dimensions.z || point.z-absolute_center.z < -half_dimensions.z)
		return false;

	return true;
}

bool AABB::pointIsInside(const Vector3d& point, const Vector3d& object_position) const
{
	const Vector3d absolute_center = object_position + center;

	if(point.x-absolute_center.x > half_dimensions.x || point.x-absolute_center.x < -half_dimensions.x)
		return false;
	if(point.y-absolute_center.y > half_dimensions.y || point.y-absolute_center.y < -half_dimensions.y)
		return false;
	if(point.z-absolute_center.z > half_dimensions.z || point.z-absolute_center.z < -half_dimensions.z)
		return false;

	return true;
}

// Check if the volume intersects or is inside a given frustum.
// object_position : position of the bounded object
bool AABB::isInsideFrustum(const Frustum& frustum, const Vector3f& object_position) const
{
	// Frustum's planes
	const Planef* planes[6] = {
		&frustum.getNearPlane(),
		&frustum.getFarPlane(),
		&frustum.getLeftPlane(),
		&frustum.getRightPlane(),
		&frustum.getTopPlane(),
		&frustum.getBottomPlane()
	};

	int nb_in=0, nb_out=0;

	// For each plane...
	for(int i=0 ; i < 6 ; i++)
	{
		// Reset counters for corners in and out
		nb_out = nb_in = 0;

		// For each corner of the box...
		//for(int j=0 ; j < 8 && ((nb_in == 0) || (nb_out == 0)) ; j++)
		for(int j=0 ; j < 8 ; j++)
		{
			Planef::PointPosition pos = planes[i]->classifyPoint(object_position + corners[j]);
			if(pos == Planef::POINT_BACK)
				nb_out++;
			else
				nb_in++;
		}

		// If all corners are out, the box is outside the frustum
		if (nb_in == 0)
			return false;
		// If some corners are out and others are in, the box is intersecting
		else if(nb_out != 0)
			return true;
	}
	return true;
}

void AABB::setHalfDimensions(const Vector3f& half_dimensions)
{
	this->half_dimensions = half_dimensions;

	updateCorners();
}
