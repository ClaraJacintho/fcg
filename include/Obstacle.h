//
// Created by Maria on 6/16/2018.
//

#ifndef FCG_OBSTACLE_H
#define FCG_OBSTACLE_H
#define RANGE 4
#include <iostream>
#include "Object.h"
class Obstacle {
    public:
        glm::vec3 movement;
        Object* obj;

        Obstacle(Object* obj){
            this->obj = obj;
            this->movement = glm::vec3(0.0f,0.0f,0.0f);
        }

        void update_position(){
            glm::vec3 pos = this->obj->pos + this->movement;
            if(abs(pos.x) > RANGE || abs(pos.y) > RANGE||abs(pos.z) > RANGE){
                cout << "aa??";
                this->movement.x = this->movement.x * -1;
                this->movement.y = this->movement.y * -1;
                this->movement.z = this->movement.z * -1;
            }
            else{
                cout << pos.x << endl;
                this->obj->setPos(pos);
            }

        }
};


#endif //FCG_OBSTACLE_H
