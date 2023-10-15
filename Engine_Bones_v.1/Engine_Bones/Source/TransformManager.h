#ifndef TRANSFORMMANAGER_H_
#define TRANSFORMMANAGER_H_

struct Transform
{
	float3 position = float3::zero;
	Quat rotation;
	float3 scale = { 1.0f, 1.0f, 1.0f };
	float4x4 transform = float4x4::zero;

	void SetPosition(float3 pos) { this->position = pos; }; //UpdateMatrix(); };
	void SumPosition(float3 pos) { this->position += pos; }; //UpdateMatrix(); };
	void SetRotation(Quat rot) { this->rotation = rot; }; //UpdateMatrix(); };

	void SetScale(float3 sca) { this->scale = sca; }; //UpdateMatrix(); };

	float3 inline GetPosition() const { return this->position; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };
};

class TransformManager 
{

private:

public: 
	//void SetPosition(float3 position, Transforms* trans) { trans->position = position; }; //UpdateMatrix(); };
	//void SetRotation(Quat rotation, Transforms* trans) { trans->rotation = rotation; }; //UpdateMatrix(); };
	//void SetScale(float3 scale, Transforms* trans) { trans->scale = scale; }; //UpdateMatrix(); };

	//float3 inline GetPosition(Transforms* trans) const { return trans->position; };
	//Quat inline GetRotation(Transforms* trans) const { return trans->rotation; };
	//float3 inline GetScale(Transforms* trans) const { return trans->scale; };

private: 

	//void UpdateMatrix(Transforms* trans) 
	//{
	//	trans = float3x3::FromTRS(trans->position, trans->rotation, scale);
	//}
};


#endif // TRANSFORMMANAGER_H_
