#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"
#define SPEED_INCREMENT 1.0f
#define MAX_SPEED 1.0f
#define ACCELERATION 0.2f
#define TURN_ANGLE 0.3f
#define MAX_ANGLE 1.2f

class Player:public Object
{
    public:
        glm::vec3 speed;
        double last_update;
        Player();
        virtual ~Player();
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

    protected:

    private:
        bool turned_x;
        bool turned_z;
};

#endif // PLAYER_H
