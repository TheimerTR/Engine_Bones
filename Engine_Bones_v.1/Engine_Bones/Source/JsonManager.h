#ifndef JSONVAR_H__
#define JSONVAR_H__

#include "External/Parson/parson.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4.h"
#include "External/MathGeoLib/include/Math/Quat.h"

#include "Globals.h"

using namespace std;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class JSArray;

class JsonManager
{
private:
	JSON_Object* node = nullptr;
	JSON_Value* value = nullptr;

public:
	JsonManager();
	JsonManager(const char* buffer);
	JsonManager(JSON_Object* jsObj);
	~JsonManager();

	uint toSerialize(char** buffer) const;

	void addNumber(const string name, const double numb);
	double getNumber(const string name);

	void addBool(const string name, const bool boolean);
	bool getBool(const string name);

	void addString(const string name, const string str);
	string getString(const string name);

	void addFloat2(const string name, const float2 ft2);
	float2 getFloat2(const string name);

	void addFloat3(const string name, const float3 ft3);
	float3 getFloat3(const string name);

	void addFloat4(const string name, const float4 ft4);
	float4 getFloat4(const string name);

	void addQuat(const string name, const Quat qt);
	Quat getQuat(const string name);

	JSArray addArray(const string name);
	JSArray getArray(const string name) const;
};

class JSArray
{
private:
	JSON_Array* jsArray;
	uint index = 0;

public:
	JSArray();
	JSArray(JSON_Array* jsArray);

	uint GetSize() const;
	JsonManager addNode();
	JsonManager getNode(uint index) const;
};

#endif //JSONVAR_H__