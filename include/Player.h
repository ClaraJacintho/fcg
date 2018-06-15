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

class Player:public Object
{
    public:
        glm::vec3 speed;
        glm::vec3 direction;
        glm::vec3 acceleration;

        double last_update;
        Player();
        virtual ~Player();
        void move(glm::vec3 direction);
        void move_up();
        void move_down();
        void move_left();
        void move_right();
        void move_foward();
        void move_backwards();
        void update_player(double dt, vector<Object*> objs);
        void unturn_left();
        void unturn_right();
        void unturn_up();
        void unturn_down();



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

        }

        void checkCollisionSphere(Sphere* obj) {
            /*glm::vec3 closest_point(
                max(this->bbox_min.x, min(obj->pos.x, this->bbox_min.x)),
                max(this->bbox_min.y, min(obj->pos.y, this->bbox_min.y)),
                max(this->bbox_min.z, min(obj->pos.z, this->bbox_min.z))
            );

            float distance =
                (closest_point.x - obj->pos.x) * (closest_point.x - obj->pos.x) +
                (closest_point.y - obj->pos.y) * (closest_point.y - obj->pos.y) +
                (closest_point.z - obj->pos.z) * (closest_point.z - obj->pos.z);

            if (distance < radius?)*/
            //https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection

            glm::vec3 difference = obj->pos - this->pos;
            cout << "Diff" <<endl;
            cout << difference.x <<endl;
            cout << difference.y <<endl;
            cout << difference.z <<endl;

            glm::vec3 half_extents;
            half_extents.x = (this->bbox_max.x - this->bbox_min.x) / 2;
            half_extents.y = (this->bbox_max.y - this->bbox_min.y) / 2;
            half_extents.z = (this->bbox_max.z - this->bbox_min.z) / 2;
            cout << "Halves" <<endl;
            cout << half_extents.x <<endl;
            cout << half_extents.y <<endl;
            cout << half_extents.z <<endl;

            glm::vec3 clamped = glm::clamp(difference, -half_extents, half_extents);
            cout << "Clamped" <<endl;
            cout << clamped.x <<endl;
            cout << clamped.y <<endl;
            cout << clamped.z <<endl;

            glm::vec3 closest = this->pos + clamped;
            cout << "Closest" <<endl;
            cout << closest.x <<endl;
            cout << closest.y <<endl;
            cout << closest.z <<endl;

            difference = closest - obj->pos;
            cout << "Differece" <<endl;
            cout << difference.x <<endl;
            cout << difference.y <<endl;
            cout << difference.z <<endl;

            cout << "Object pos" <<endl;
            cout << obj->pos.x <<endl;
            cout << obj->pos.y <<endl;
            cout << obj->pos.z <<endl;

            cout << "Object pos" <<endl;
            cout << this->bbox_max.x <<endl;
            cout << this->bbox_max.y <<endl;
            cout << this->bbox_max.z <<endl;

            cout << "Object pos" <<endl;
            cout << this->bbox_min.x <<endl;
            cout << this->bbox_min.y <<endl;
            cout << this->bbox_min.z <<endl;



            if(glm::length(difference) < obj->radius){
                cout << "Boom";
            }

        }
    protected:

    private:
        bool turned_x;
        bool turned_z;
};

#endif // PLAYER_H
