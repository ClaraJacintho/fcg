#ifndef SPHERE_H
#define SPHERE_H
#include "Object.h"

class Sphere:public Object
{
    public:
        float radius;
        Sphere(int i, string name, const char *file_name):Object(1, i, name,file_name){
            radius = 0.5;
        }

        ~Sphere()
        {
            //dtor
        };

    void setScale(float new_radius){
            this->scale = glm::vec3(new_radius, new_radius, new_radius);
            this->radius = new_radius/2;
        }
};

#endif // SPHERE_H
