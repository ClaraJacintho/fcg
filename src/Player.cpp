#include <iostream>
#include <cmath>
#include "Player.h"

Player::Player():Object("jet", "../../data/jet.obj")
{
    this->speed.x =0;
    this->speed.y =0;
    this->speed.z =0;
}

Player::~Player()
{
    //dtor
}

void Player::move_right(){
    this->speed.x += SPEED_INCREMENT;
    if(turned_z == false){
        this->rad.y -= TURN_ANGLE;
        turned_z = true;
    }

}

void Player::move_left(){
    this->speed.x -= SPEED_INCREMENT;
    if(turned_z == false){
        this->rad.y += TURN_ANGLE;
        turned_z = true;
    }

}

void Player::move_up(){
    this->speed.y += SPEED_INCREMENT;
    if(turned_x == false){
        this->rad.x += TURN_ANGLE;
        turned_x = true;
    }

}

void Player::move_down(){
    this->speed.y -= SPEED_INCREMENT;
    if(turned_x == false){
        this->rad.x -= TURN_ANGLE;
        turned_x = true;
    }

}
void Player::move_foward(){
    this->speed.z += SPEED_INCREMENT;
}

void Player::move_backwards(){
    this->speed.z -= SPEED_INCREMENT;
}

void Player::update_player(double dt, vector<Object*> objs){
    checkCollision(objs);

    this->pos.x += speed.x*dt;
    this->pos.y += speed.y*dt;
    this->pos.z += speed.z*dt;
}

void Player::unturn_left(){
    this->rad.y -= TURN_ANGLE;
    this->speed.x = 0;
    turned_z = false;
}
void Player::unturn_right(){
    this->rad.y += TURN_ANGLE;
    this->speed.x = 0;
    turned_z = false;
}
void Player::unturn_up(){
    this->rad.x -= TURN_ANGLE;
    this->speed.y = 0;
    turned_x = false;
}
void Player::unturn_down(){
    this->rad.x += TURN_ANGLE;
    this->speed.y = 0;
    turned_x = false;
}


void Player::checkCollision(vector<Object*> vect){
    for(auto const a : vect){
       if(a->name != this->name){
            bool col_x =false;
            bool col_y =false;
            bool col_z =false;
            if(this->bbox_max.x * this->scale.x + this->pos.x >= a->bbox_min.x * a->scale.x + a->pos.x
               && a->bbox_max.x * a->scale.x + a->pos.x >= this->bbox_min.x * this->scale.x + this->pos.x) {
                col_x = true;
            }

            if(this->bbox_max.y * this->scale.y + this->pos.y >= a->bbox_min.y * a->scale.y + a->pos.y
               && a->bbox_max.y * a->scale.y + a->pos.y >= this->bbox_min.y * this->scale.y + this->pos.y) {
                col_y = true;
            }

            if(this->bbox_max.z * this->scale.z + this->pos.z >= a->bbox_min.z * a->scale.z + a->pos.z
               && a->bbox_max.z * a->scale.z + a->pos.z >= this->bbox_min.z * this->scale.z + this->pos.z) {
                col_z = true;
            }

            if(col_x && col_y && col_z){
                a->destroyed = true;
                printf("Destroyed %s\n", a->name.c_str());
            }
       }

    }
}

