#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Sphere.h"
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#define SPEED_INCREMENT 1.0f
#define MAX_SPEED 12.0f
#define ACCELERATION 2.0f
#define TURN_ANGLE 0.3f
#define MAX_ANGLE 1.2f

class Player : public Object  {
public:
    glm::vec3 speed;
    glm::vec3 direction;
    glm::vec3 acceleration;
    double last_update;

    Player() : Object("jet", "../../data/jet.obj")
    {
        this->speed.x = 0;
        this->speed.y = 0;
        this->speed.z = 0;
        this->turned_x = false;
        this->turned_z = false;
        this->last_update = 0;

        this->acceleration = glm::vec3(0.0f,0.0f,0.0f);
    }

//        virtual ~Player();

    void update_player(double dt, vector<Object*> objs){
        this->checkCollision(objs);
        this->speed.x += this->acceleration.x * dt;
        this->speed.y += this->acceleration.y * dt;
        this->speed.z += this->acceleration.z * dt;

        this->speed.x = (speed.x >= MAX_SPEED ? MAX_SPEED : speed.x);
        this->speed.y = (speed.y >= MAX_SPEED ? MAX_SPEED : speed.y);
        this->speed.z = (speed.z >= MAX_SPEED ? MAX_SPEED : speed.z);

        glm::vec3 newpos(this->pos.x + this->speed.x * dt,
                         this->pos.y + this->speed.y * dt,
                         this->pos.z + this->speed.z * dt);

        this->setPos(newpos);
    }

    void move(glm::vec3 direction) {
        this->acceleration = direction;
    }

    void checkCollision(vector<Object *> vect){
        for(auto const obj : vect){
            if(obj->name != this->name){
                switch (obj->obj_type) {
                    case SPHERE:
                        //checkCollisionSphere((Sphere *) obj);
                        //break;
                    case PLANE:
                    case AABB:
                        checkCollisionAABB(obj);
                    default:
                        break;
                }
            }
        }
    }

    void checkCollisionAABB(Object* obj){
        bool col_x = false;
        bool col_y = false;
        bool col_z = false;

        if(this->bbox_max.x > obj->bbox_min.x
           && obj->bbox_max.x > this->bbox_min.x) {
            col_x = true;
            //            std::cout << "col_x: " << this->name << " " << obj->name << std::endl;
        }

        if(this->bbox_max.y > obj->bbox_min.y
           && obj->bbox_max.y > this->bbox_min.y) {
            col_y = true;
            //            std::cout << "col_y: " << this->name << " " << obj->name << std::endl;
        }

        if(this->bbox_max.z > obj->bbox_min.z
           && obj->bbox_max.z > this->bbox_min.z) {
            col_z = true;
            //            std::cout << "col_z: " << this->name << " " << obj->name << std::endl;
        }

        if(col_x && col_y && col_z){
            //obj->destroyed = true;
            printf("touched %s\n", obj->name.c_str());
            //return true;
        }

    }

    void move_up() {
        this->speed.y += SPEED_INCREMENT;
        if (turned_x == false) {
            this->rad.x += TURN_ANGLE;
            turned_x = true;
        }
    }

    void move_down() {
        this->speed.y -= SPEED_INCREMENT;
        if (turned_x == false) {
            this->rad.x -= TURN_ANGLE;
            turned_x = true;
        }
    }

    void move_left() {
        this->speed.x -= SPEED_INCREMENT;
        if (turned_z == false) {
            this->rad.y += TURN_ANGLE;
            turned_z = true;
        }
    }

    void move_right() {
        this->speed.x += SPEED_INCREMENT;
        if (turned_z == false) {
            this->rad.y -= TURN_ANGLE;
            turned_z = true;
        }
    }

    void move_foward(){
        this->speed.z += SPEED_INCREMENT;
    }

    void move_backwards(){
        this->speed.z -= SPEED_INCREMENT;
    }

    void unturn_left(){
        this->rad.y -= TURN_ANGLE;
        this->speed.x = 0;
        turned_z = false;
    }

    void unturn_right(){
        this->rad.y += TURN_ANGLE;
        this->speed.x = 0;
        turned_z = false;
    }

    void unturn_up(){
        this->rad.x -= TURN_ANGLE;
        this->speed.y = 0;
        turned_x = false;
    }

    void unturn_down(){
        this->rad.x += TURN_ANGLE;
        this->speed.y = 0;
        turned_x = false;
    }



    /*void checkCollisionAABB(Object* obj){
        bool col_x = false;
        bool col_y = false;
        bool col_z = false;

        if(this->bbox_max.x * this->scale.x + this->pos.x >= obj->bbox_min.x * obj->scale.x + obj->pos.x
           && obj->bbox_max.x * obj->scale.x + obj->pos.x >= this->bbox_min.x * this->scale.x + this->pos.x) {
            col_x = true;
        }

        if(this->bbox_max.y * this->scale.y + this->pos.y >= obj->bbox_min.y * obj->scale.y + obj->pos.y
           && obj->bbox_max.y * obj->scale.y + obj->pos.y >= this->bbox_min.y * this->scale.y + this->pos.y) {
            col_y = true;
        }

        if(this->bbox_max.z * this->scale.z + this->pos.z >= obj->bbox_min.z * obj->scale.z + obj->pos.z
           && obj->bbox_max.z * obj->scale.z + obj->pos.z >= this->bbox_min.z * this->scale.z + this->pos.z) {
            col_z = true;
        }

        if(col_x && col_y && col_z){
            //obj->destroyed = true;
            printf("touched %s\n", obj->name.c_str());
            //return true;
        }

    }*/

    void checkCollisionSphere(Sphere* obj) {
            //https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection

            glm::vec3 difference = obj->pos - this->pos;

            glm::vec3 half_extents = (this->bbox_max - this->pos) * this->scale;


            glm::vec3 clamped = glm::clamp(difference, -half_extents, half_extents);
            glm::vec3 closest = this->pos + clamped;
            difference = closest - obj->pos;

            if(glm::length(difference) < obj->radius){
                cout << "sphere boom";
            }

        }

        void checkCollisionPlane(Object* obj){
            //cout << "in plane";
            glm::vec3 half_extents = (this->bbox_max - this->pos) * this->scale;

            glm::vec3 half_extents_plane = obj->bbox_max  * obj->scale;
            glm::vec3 p1 = (obj->bbox_max * obj->scale) + obj->pos;
            glm::vec3 p2 = p1 - half_extents_plane; //this does not make sense, do not worry

            glm::vec3 plane_normal = cross((obj->pos - p1),(obj->pos - p2));

            float distance = dot((this->pos - obj->pos),(plane_normal));
            distance -= half_extents.x;

            cout << "distance" << endl;
            cout << distance << endl;
            cout << "normal" << endl;
            cout << plane_normal.x << endl;
            cout << plane_normal.y << endl;
            cout << plane_normal.z << endl;
            cout << "Object pos" <<endl;
            cout << obj->pos.x <<endl;
            cout << obj->pos.y <<endl;
            cout << obj->pos.z <<endl;
            cout << "p1" <<endl;
            cout<< p1.x <<endl;
            cout<< p1.y <<endl;
            cout<< p1.z <<endl;
            cout << "p2" <<endl;
            cout<< p2.x <<endl;
            cout<< p2.y <<endl;
            cout<< p2.z <<endl;
            cout << "Halves" <<endl;
            cout << half_extents_plane.x <<endl;
            cout << half_extents_plane.y <<endl;
            cout << half_extents_plane.z <<endl;

            if(distance < 0){
             //   cout << "plane boom";
            }

        }
protected:

private:
    bool turned_x;
    bool turned_z;
};

#endif // PLAYER_H
