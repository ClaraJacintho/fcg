#include "Object.h"




Object::Object(string name, const char *file_name, const char *mtl_basepath /*= NULL*/) : model(file_name, mtl_basepath, true)
{


    this->name    = name;
    this->scale.x = 1.0f;
    this->scale.y = 1.0f;
    this->scale.z = 1.0f;
    this->pos.x   = 0.0f;
    this->pos.y   = 0.0f;
    this->pos.z   = 0.0f;
    this->rad.x   = 0.0f;
    this->rad.y   = 0.0f;
    this->rad.z   = 0.0f;

}

Object::~Object()
{
    //dtor
}
