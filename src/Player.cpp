#include "Player.h"

bool turned_x;
bool turned_y;

Player::Player():Object("cow", "../../data/cow.obj")
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
    if(turned_y == false){
        printf("DINGUE");
        this->rad.y -= TURN_ANGLE;
        turned_y = true;
    }

}

void Player::move_left(){
    this->speed.x -= SPEED_INCREMENT;
    if(turned_y == false){
        this->rad.y += TURN_ANGLE;
        turned_y = true;
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
    printf("AA");
    this->speed.z -= SPEED_INCREMENT;
}

void Player::update_player(double time){
    double delta_t = time - this->last_update;

    this->pos.x += speed.x;
    this->pos.y += speed.y;
    this->pos.z += speed.z;
}

void Player::unturn_left(){
    this->rad.y -= TURN_ANGLE;
    turned_y = false;
}
void Player::unturn_right(){
    this->rad.y += TURN_ANGLE;
    turned_y = false;
}
void Player::unturn_up(){
    this->rad.x -= TURN_ANGLE;
    turned_x = false;
}
void Player::unturn_down(){
    this->rad.x += TURN_ANGLE;
    turned_x = false;
}



