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
    checkCollisionAABB(objs);

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


