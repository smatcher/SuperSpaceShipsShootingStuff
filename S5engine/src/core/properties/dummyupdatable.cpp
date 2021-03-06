#include "core/properties/dummyupdatable.h"
#include "core/scenegraph/node.h"

DummyUpdatable::DummyUpdatable(double speed) : IUpdatable(QString("DummyUpdatable"))
{
	this->angle = 90;
	this->speed = speed;
}

void DummyUpdatable::update(double elapsed_time)
{
	//std::cout << "Updating DummyUpdatable" << std::endl;
	Node* parentNode = this->node();
	parentNode->rotate(Vector3f(0.0, 1.0, 0.0), angle*elapsed_time*speed);
}
