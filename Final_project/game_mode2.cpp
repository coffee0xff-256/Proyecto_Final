#include "game_mode2.h"
#include <QPainter>

Game_mode2::Game_mode2(QWidget *parent)
    : QWidget{parent}
{
    resize(1080,720);
    setWindowTitle("Game_mode1");

//***********************Movimiento********************************
    setFocusPolicy(Qt::StrongFocus);
//***********************Movimiento********************************

}
//************ESQUELETO*****************************
void Game_mode2::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.fillRect(rect(),Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(450,360,"GAME MODE 2");
//************ESQUELETO*****************************

//***********Mapa vista cenital**********************
    int tilesize = 50;
    for(int fila = 0;fila < 8;fila++){
        for(int columna = 0;columna < 8; columna++){
            if(map1[fila][columna]== 1){
                painter.fillRect(columna * tilesize , fila * tilesize , tilesize,tilesize,Qt::white);
            }

            painter.drawRect(columna * tilesize , fila * tilesize , tilesize,tilesize);
        }
}

//Aqui dibujo el personaje

painter.setBrush(Qt::red);
painter.drawEllipse(
        playerx * tilesize - 5,
        playery * tilesize -5,
        10,10
        );
    ;

//***********Mapa vista cenital**********************

//***********************Movimiento********************************
    //Aqui hay movimiento y visión

    int linelenght = 30;
    int x1 = playerx * tilesize;
    int y1 = playery * tilesize;
    int x2 = x1 + cos(angle) * linelenght;
    int y2 = y1 + sin(angle) * linelenght;
    painter.setPen(QPen(Qt::green,2));
    painter.drawLine(x1,y1,x2,y2);

//***********************Movimiento********************************
}

//***********************Movimiento********************************

void Game_mode2::keyPressEvent(QKeyEvent *event){
    double velocidad = 0.2;
    double rotacion = 0.1;

    switch(event->key()){
        playerx += cos(angle) * velocidad;
        playery += sin(angle) * velocidad;

        break;

    case Qt::Key_W:

        playerx += cos(angle) * velocidad;
        playery += sin(angle) * velocidad;

        break;

    case Qt::Key_S:

        playerx -= cos(angle) * velocidad;
        playery -= sin(angle) * velocidad;

        break;

    case Qt::Key_A:

        angle -= rotacion;

        break;

    case Qt::Key_D:

        angle += rotacion;

        break;
    }
    update();
}
//***********************Movimiento********************************
