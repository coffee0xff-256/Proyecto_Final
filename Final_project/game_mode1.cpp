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

//Permite hacer que el player 2 manipule el moviemiento de manera directamente.
void player::controlIA(bool left, bool right){
    pushIzq = left;
    pushDer = right;
}


void player::golpear(){}
bool player::golpeando(){return false;}

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

void ball::rebote(){
    vy = -vy * rebotar;
    vx =  vx * friction;
}



void ball::reset(){
    x =startX;
    y = startY;
    vx = 0;
    vy = 0;

}

Game_mode1::Game_mode1(QWidget *parent)
    : QWidget{parent}
{
    resize(windowwidth, windowheight);
    setWindowTitle("Head Football");
    setFocusPolicy(Qt::StrongFocus);

    // Player 1 teclado para el desarrollo de juego.
    player1 = new player(100, levelfloor - 80, Qt::Key_A, Qt::Key_D, Qt::Key_W);

    //Asigancion de player 2 va sin teclas ya que es una implementacion de una IA

    player2 = new player(windowwidth - 160, levelfloor - 80,0, 0, 0);

    // Balon empieza desde el centro.
    balon = new ball(windowwidth / 2.0f, levelfloor / 2.0f);


    //Primera implementacion de sprites personaje "Alex" prueba.

    spriteBall = QPixmap(":/Balon.png");
    spriteRun =  QPixmap("/Run_Alex.png");
    spriteSalto =  QPixmap("/Run_Alex.png");
    spriteGolpe=QPixmap("/Run_Alex.png");



    // Timer para que vaya a 60fps
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game_mode1::actualizar);
    gameTimer->start(16);
}

// Game loop para mover todo y de manera organizada
void Game_mode1::actualizar(){
    float dt = 16.0f / 1000.0f;

    player1->move(dt);
    player2->move(dt);
    balon->move(dt);

    //Player 1 colision con el suelo.
    if(player1->y + player1->height >= levelfloor)
    {
        player1->y       = levelfloor - player1->height;
        player1->vy      = 0;
        player1->down = true;
    }
    else player1->down  = false;

    //Balon con colision en la en suelo.
    if(balon->y + balon->radius >= levelfloor)
    {
        balon->y = levelfloor - balon->radius;
        balon->rebote();
    }


    //Player 2 colision con el suelo.
    if(player2->y + player2->height >= levelfloor) {
        player2->y    = levelfloor - player2->height;
        player2->vy   = 0;
        player2->down = true;
    } else {
        player2->down = false;
    }

    //balon con coolision con el player 1
    float centrox_player1   = player1->x + player1->width  / 2.0f;
    float centroy_player1   = player1->y + player1->height / 2.0f;
    float distanciax_player1   = balon->x - centrox_player1;
    float distanciay_player1   = balon->y - centroy_player1;
    float distancia_player1 = sqrt(distanciax_player1*distanciax_player1 + distanciay_player1*distanciay_player1);
    float radius_colision = player1->width / 2.0f + balon->radius;

    if(distancia_player1 < radius_colision && distancia_player1 > 0){
        float direccionx = distanciax_player1 / distancia_player1;
        float direcciony = distanciay_player1 / distancia_player1;
        balon->vx  = direccionx * 500.0f;
        balon->vy  = direcciony * 500.0f - 100.0f;
        balon->x   = centrox_player1 + direccionx * radius_colision;
        balon->y   = centroy_player1 + direcciony * radius_colision;
    }

    //Balon con colision con el player 2, misma idea del player 1 para el desarrollo.
    float centrox_player2 = player2->x + player2->width  / 2.0f;
    float centroy_player2 = player2->y + player2->height / 2.0f;
    float distanciax_player2 = balon->x - centrox_player2;
    float distanciay_player2 = balon->y - centroy_player2;
    float distancia_player2 = sqrt(distanciax_player2*distanciax_player2 + distanciay_player2*distanciay_player2);
    float radius_colision2 = player2->width / 2.0f + balon->radius;

    if(distancia_player2 < radius_colision2 && distancia_player2 > 0){
        float direccionx = distanciax_player2 / distancia_player2;
        float direcciony = distanciay_player2 / distancia_player2;
        balon->vx  = direccionx * 500.0f;
        balon->vy  = direcciony * 500.0f - 100.0f;
        balon->x   = centrox_player2 + direccionx * radius_colision2;
        balon->y   = centroy_player2 + direcciony * radius_colision2;
    }

    //Limite de pantalla para los jugadores.
    if(player1->x < 0){ player1->x = 0; player1->vx = 0;}
    if(player1->x + player1->width > windowwidth){ player1->x = windowwidth - player1->width; player1->vx = 0;}
    if(player2->x < 0){ player2->x = 0;player2->vx = 0;}
    if(player2->x + player2->width > windowwidth){ player2->x = windowwidth - player2->width; player2->vx = 0;}

    //limite de pantalla para el balon.
    if(balon->x - balon->radius < 0){
        balon->x  = balon->radius;
        balon->vx = -balon->vx * 0.7f;
    }
    if(balon->x + balon->radius > windowwidth){
        balon->x  = windowwidth - balon->radius;
        balon->vx = -balon->vx * 0.7f;
    }

    update();
}
