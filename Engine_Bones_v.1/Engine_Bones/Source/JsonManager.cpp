#include "JsonManager.h"

JsonManager::JsonManager()
{
	value = json_value_init_object();
	node = json_value_get_object(value);
}

JsonManager::JsonManager(const char* buffer)
{
    value = json_parse_string(buffer);
    if (value)
    {
        node = json_value_get_object(value);
    }
}

JsonManager::JsonManager(JSON_Object* jsObj) : node(jsObj)
{
}

JsonManager::~JsonManager()
{
    if (value != nullptr)
    {
        json_value_free(value);
    }
}

uint JsonManager::toSerialize(char** buffer) const
{
    uint ptSize = (uint)json_serialization_size_pretty(value);
    *buffer = new char[ptSize];
    JSON_Status ptBuffer = json_serialize_to_buffer_pretty(value, *buffer, ptSize);

    if (ptBuffer == JSONFailure)
    {
        LOG(LogTypeCase::L_ERROR, "Faild to serialzie!");
        RELEASE_ARRAY(buffer);
        ptSize = 0;
    }

    return ptSize;
}

void JsonManager::addNumber(const string name, const double numb)
{
   json_object_set_number(node, name.c_str(), numb);
}

double JsonManager::getNumber(const string name)
{
    return json_object_get_number(node, name.c_str());
}

void JsonManager::addBool(const string name, const bool boolean)
{
    json_object_set_boolean(node, name.c_str(), boolean);
}

bool JsonManager::getBool(const string name)
{
    return json_object_get_boolean(node, name.c_str());
}

void JsonManager::addString(const string name, const string str)
{
    json_object_set_string(node, name.c_str(), str.c_str());
}

string JsonManager::getString(const string name)
{
    return json_object_get_string(node, name.c_str());
}

void JsonManager::addFloat2(const string name, const float2 ft2)
{
    JSON_Array* arr = json_object_get_array(node, name.data());

    if (arr != nullptr) 
    {
        json_array_clear(arr);
        
    }
    else 
    {
        JSON_Value* val = json_value_init_array();
        arr = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }

    json_array_append_number(arr, ft2.x);
    json_array_append_number(arr, ft2.y);
}

float2 JsonManager::getFloat2(const string name)
{
    float2 ft2;
    JSON_Array* arr = json_object_get_array(node, name.data());

    ft2.x = json_array_get_number(arr, 0);
    ft2.y = json_array_get_number(arr, 1);

    return ft2;
}

void JsonManager::addFloat3(const string name, const float3 ft3)
{
    JSON_Array* arr = json_object_get_array(node, name.data());

    if (arr != nullptr)
    {
        json_array_clear(arr);

    }
    else
    {
        JSON_Value* val = json_value_init_array();
        arr = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }

    json_array_append_number(arr, ft3.x);
    json_array_append_number(arr, ft3.y);
    json_array_append_number(arr, ft3.z);
}

float3 JsonManager::getFloat3(const string name)
{
    float3 ft3;
    JSON_Array* arr = json_object_get_array(node, name.data());

    ft3.x = json_array_get_number(arr, 0);
    ft3.y = json_array_get_number(arr, 1);
    ft3.z = json_array_get_number(arr, 1);

    return ft3;
}

void JsonManager::addFloat4(const string name, const float4 ft4)
{
    JSON_Array* arr = json_object_get_array(node, name.data());

    if (arr != nullptr)
    {
        json_array_clear(arr);

    }
    else
    {
        JSON_Value* val = json_value_init_array();
        arr = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }

    json_array_append_number(arr, ft4.x);
    json_array_append_number(arr, ft4.y);
    json_array_append_number(arr, ft4.z);
    json_array_append_number(arr, ft4.w);
}

float4 JsonManager::getFloat4(const string name)
{
    float4 ft4;
    JSON_Array* arr = json_object_get_array(node, name.data());

    ft4.x = json_array_get_number(arr, 0);
    ft4.y = json_array_get_number(arr, 1);
    ft4.z = json_array_get_number(arr, 1);
    ft4.w = json_array_get_number(arr, 1);

    return ft4;
}

void JsonManager::addQuat(const string name, const Quat qt)
{
    JSON_Array* arr = json_object_get_array(node, name.data());

    if (arr == nullptr) 
    {
        JSON_Value* val = json_value_init_array();
        arr = json_value_get_array(val);
        json_object_dotset_value(node, name.data(), val);
    }
    else 
    {
        json_array_clear(arr);
    }

    json_array_append_number(arr, qt.x);
    json_array_append_number(arr, qt.y);
    json_array_append_number(arr, qt.z);
    json_array_append_number(arr, qt.w);
}

Quat JsonManager::getQuat(const string name)
{
    JSON_Array* arr = json_object_get_array(node, name.data());
    Quat qt;
    qt.x = json_array_get_number(arr, 0);
    qt.y = json_array_get_number(arr, 1);
    qt.z = json_array_get_number(arr, 2);
    qt.w = json_array_get_number(arr, 3);

    return qt;
}

JSArray JsonManager::addArray(const string name)
{
    json_object_set_value(node, name.c_str(), json_value_init_array());
    return JSArray(json_object_get_array(node, name.c_str()));
}

JSArray JsonManager::getArray(const string name) const
{
    if (json_object_has_value_of_type(node, name.c_str(), JSONArray))
    {
        return JSArray(json_object_get_array(node, name.c_str()));
    }
}

JSArray::JSArray()
{
    jsArray = json_value_get_array(json_value_init_array());
}

JSArray::JSArray(JSON_Array* jsArray) : jsArray(jsArray)
{
    index = json_array_get_count(jsArray);
}

uint JSArray::GetSize() const
{
    return index;
}

JsonManager JSArray::addNode()
{
    json_array_append_value(jsArray, json_value_init_object());
    index++;

    return JsonManager(json_array_get_object(jsArray, index - 1));
}

JsonManager JSArray::getNode(uint index) const
{
    return JsonManager(json_array_get_object(jsArray, index));
}
