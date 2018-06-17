//
// Created by Maria on 6/16/2018.
//

#ifndef FCG_OBSTACLE_H
#define FCG_OBSTACLE_H
#define RANGE 4
#define MIN 0
#define MAX 3
#include <iostream>
#include "Object.h"
#include <ctime>

class Obstacle {
    public:
        glm::vec3 movement;
        Object* obj;

        Obstacle(Object* obj){
            this->obj = obj;
            this->movement = glm::vec3(0.0f,0.0f,0.0f);
        }

        void update(float z_axis, double dt){
            glm::vec3 pos = this->obj->pos;
            pos.x += this->movement.x * dt * 25;
            pos.y += this->movement.y * dt * 25;
            pos.z += this->movement.z * dt * 25;

            if(abs(pos.x) > RANGE || abs(pos.y) > RANGE){
                this->movement.x = this->movement.x * -1;
                this->movement.y = this->movement.y * -1;
                this->movement.z = this->movement.z * -1;
            }
            else{
                this->obj->setPos(pos);
            }

            if(pos.z > z_axis){    //behind the camera?
                srand(time(NULL));
                float x = MIN + (rand() % (MAX +1) + MIN);
                float y = MIN + (rand() % (MAX +1) + MIN);
                glm::vec3 pos1 =  glm::vec3(x, y,this->obj->pos.z -15.0f);

//                cout << pos1.x << endl;
//                cout << pos1.y << endl;
//                cout << pos1.z << endl;
                this->obj->setPos( pos1);
            }

        }
};


#endif //FCG_OBSTACLE_H
