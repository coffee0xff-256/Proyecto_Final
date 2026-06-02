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

