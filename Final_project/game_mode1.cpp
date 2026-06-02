#include "game_mode1.h"
#include <QPainter>


//Player
player::player(float x, float y, int keyIzq, int keyDer, int keySalto)
{
    this->x = x;
    this->y = y;
    this->vx = 0;
    this->vy = 0;
    this->width = 60;
    this->height = 80;
    this->down = false;

    this->gravity = 1200.0f;
    this->velocidadmov = 300.0f;
    this->fuerzasalto = -600.0f;

    this->keyIzq = keyIzq;
    this->keyDer =  keyDer;
    this->keysalto =  keySalto;

    this->pushIzq = false;
    this->pushDer= false;

}
//Define gravity with the simple terms


void player::move(float dt)
{
    vx = 0;
    if(pushIzq) vx = -velocidadmov;
    if(pushDer) vx = velocidadmov;

    vy += gravity * dt;
    x += vx * dt;
    y += vy * dt;

}

//Push up only that is in the floor.


void player::jump()
{
    if(down){
        vy = fuerzasalto;
        down = false;
    }
}

void player::keypush(int key){

    if(key == keyIzq) pushIzq = true;
    if(key == keyDer) pushDer = true;
    if(key == keysalto) jump();

}

void player::keydrop(int key){
    if(key == keyIzq) pushIzq = false;
    if(key == keyDer) pushDer = false;

}

//Logica del balon

ball:: ball(float x, float y){
    this->x = x;
    this->y = y;
    this->vx = 0;
    this->vy = 0;
    this->radius = 20;
    this->startX = x;
    this-> startY = y;

    this-> gravity = 1200.0f;
    this-> rebotar = 0.6f;
    this-> friction = 0.85f;

}


//Aplicacion de la gravedad para el balon.

void ball::move(float dt){

    vy += gravity * dt;
    x += vx * dt;
    y += vy * dt;

}

//Invertir la velocidad vertical y frena horizontal.


void ball::reset(){
    x =startX;
    y = startY;
    vx = 0;
    vy = 0;

}

