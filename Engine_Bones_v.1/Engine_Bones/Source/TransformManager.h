#ifndef TRANSFORMMANAGER_H_
#define TRANSFORMMANAGER_H_

class Transform {

private:
	float3 position; 
	Quat rotation; 
	float3 scale; 
	float4x4 transform;

public: 
	void SetPosition(float3 position) { this->position = position; UpdateMatrix(); };
	void SetRotation(Quat rotation) { this->rotation = rotation; UpdateMatrix(); };
	void SetScale(float3 scale) { this->scale = scale; UpdateMatrix(); };

	float3 inline GetPosition() const { return this->position; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };

private: 

	void UpdateMatrix() {
		transform = float4x4::FromTRS(position, rotation, scale);
	}
	

};


#endif // TRANSFORMMANAGER_H_
