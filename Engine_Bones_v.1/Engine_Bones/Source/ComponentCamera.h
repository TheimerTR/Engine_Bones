#ifndef COMPONENTCAMERA_H_
#define COMPONENTCAMERA_H_

#include "ComponentManager.h"
#include "AssimpManager.h"


class GameObjectManager;

class ComponentCamera : public ComponentManager
{
public:
	
	Frustum frustum;

public:

	ComponentCamera(GameObjectManager* gameObject);
	~ComponentCamera();


private:



};


#endif // COMPONENTMESH_H_