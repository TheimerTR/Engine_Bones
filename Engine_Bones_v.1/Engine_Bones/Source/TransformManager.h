#ifndef TRANSFORMMANAGER_H_
#define TRANSFORMMANAGER_H_

class Transform {

private:
	float3 position; 
	Quat rotation; 
	float3 scale; 

public: 
	void SetPosition(float3 position) { this->position = position; };
	void SetRotation(Quat rotation) { this->rotation = rotation; };
	void SetScale(float3 scale) { this->scale = scale; };

	float3 inline GetPosition() const { return this->position; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };

	

};


#endif // TRANSFORMMANAGER_H_
