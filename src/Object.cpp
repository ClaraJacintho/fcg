#include "Object.h"

Object::Object(string name, const char *file_name, const char *mtl_basepath /*= NULL*/) : model(file_name, mtl_basepath, true)
{
    ///TO DO: Actually call this functions here instead of in main
    ///ComputeNormals(&(this->model));
    ///BuildTrianglesAndAddToVirtualScene(&(this->model));

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
void Object::setPos(float x, float y, float z){
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Object::setScale(float x, float y, float z){
    this->rad.x = x;
    this->rad.y = y;
    this->rad.z = z;
}
