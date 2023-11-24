#ifndef COMPONENTCAMERA_H_
#define COMPONENTCAMERA_H_

#include "ComponentManager.h"
#include "AssimpManager.h"


class GameObjectManager;

class ComponentCamera : public ComponentManager
{

public:

	ComponentCamera(GameObjectManager* gameObject);
	~ComponentCamera();

public:

	Frustum frustum;

private:


};


#endif // COMPONENTMESH_H_