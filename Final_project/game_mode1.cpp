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
    spriteRun =  QPixmap(":/Run_Alex.png");
    spriteSalto =  QPixmap(":/Salto_Alex.png");
    spriteGolpe=QPixmap(":/Golpeo_Alex.png");



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

    //Direccion visual de los players

    if(player1 -> vx > 10) Frente_derecho_player1 = true;
    if(player1 -> vx < -10) Frente_derecho_player1 = false;


    //Logica para el player 2.
    updateIA();

    if(++animTick >= 6){
        animTick = 0;
        animFramePlayer1 = (animFramePlayer1 + 1) % 4;
        animFramePlayer2 = (animFramePlayer2 + 1) % 4;
        animFrameBall = (animFrameBall + 1) % 7;
    }

    update();
}


//Desarrollo del render

void Game_mode1::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.fillRect(0,0,windowwidth,240,QColor(100, 180, 240));
    p.fillRect(0,240,windowwidth, levelfloor - 240,QColor(55,  140, 55));
    p.fillRect(0,levelfloor,windowwidth, windowheight - levelfloor, QColor(101, 67, 33));
    p.fillRect(0,levelfloor - 4, windowwidth, 4,QColor(80,  50, 20));

    p.setPen(QPen(QColor(255, 255, 255, 70), 2, Qt::DashLine));
    p.drawLine(windowwidth / 2, 0, windowwidth / 2, levelfloor);

    dibujo_jugador(p, player1, animFramePlayer1, Frente_derecho_player1);
    dibujo_jugador(p, player2, animFramePlayer2, Frente_derecho_player2);

    if (!spriteBall.isNull()) {
        int fw = spriteBall.width() / 7;
        QPixmap ballFrame = spriteBall.copy(animFrameBall * fw, 0, fw, spriteBall.height());
        p.drawPixmap(int(balon->x - balon->radius),
                     int(balon->y - balon->radius),
                     int(balon->radius * 2),
                     int(balon->radius * 2),
                     ballFrame);
    } else {
        p.setBrush(Qt::white);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(balon->x, balon->y),
                      double(balon->radius), double(balon->radius));
    }
}

void Game_mode1::keyPressEvent(QKeyEvent *event)
{
    player1->keypush(event->key());
}

void Game_mode1::keyReleaseEvent(QKeyEvent *event)
{
    player1->keydrop(event->key());
}

void Game_mode1::updateIA()
{
    float p2CX = player2->x + player2->width / 2.0f;

    bool moveLeft  = (p2CX    - balon->x) > 25.0f;
    bool moveRight = (balon->x - p2CX)    > 25.0f;

    if (player2->x < windowwidth / 2) moveLeft = false;

    player2->controlIA(moveLeft, moveRight);

    if (std::abs(balon->x - p2CX) < 120.0f &&
        balon->y < player2->y + player2->height * 0.5f &&
        player2->down)
    {
        player2->jump();
    }

    // Actualizar dirección visual
    if (moveLeft) Frente_derecho_player2 = false;
    else if (moveRight) Frente_derecho_player2  = true;
    else Frente_derecho_player2 = (balon->x > p2CX);
}

void Game_mode1::dibujo_jugador(QPainter &p, player *pl, int animFrame, bool facingRight)
{
    QPixmap frame;

    if (!pl->down) {
        if (!spriteSalto.isNull()) {
            int fw = spriteSalto.width() / 4;
            frame  = spriteSalto.copy(2 * fw, 0, fw, spriteSalto.height());
        }
    } else if (std::abs(pl->vx) > 10.0f) {
        if (!spriteRun.isNull()) {
            int fw = spriteRun.width() / 4;
            frame  = spriteRun.copy((animFrame % 4) * fw, 0, fw, spriteRun.height());
        }
    } else {
        if (!spriteSalto.isNull()) {
            int fw = spriteSalto.width() / 4;
            frame  = spriteSalto.copy(0, 0, fw, spriteSalto.height());
        }
    }


    if (frame.isNull()) {
        p.fillRect(QRectF(pl->x, pl->y, pl->width, pl->height),
                   QColor(0, 120, 255, 180));
        return;
    }

    p.save();
    if (!facingRight) {
        p.translate(pl->x + pl->width, pl->y);
        p.scale(-1.0, 1.0);
        p.drawPixmap(0, 0, int(pl->width), int(pl->height), frame);
    } else {
        p.drawPixmap(int(pl->x), int(pl->y), int(pl->width), int(pl->height), frame);
    }
    p.restore();
}
