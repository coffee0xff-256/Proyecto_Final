#include "game_mode2.h"
#include <QPainter>

Game_mode2::Game_mode2(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1536,1024);
    setWindowTitle("Game_mode2");

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
    double dx = playerx - enemyx;
    double dy = playery - enemyy;


//Aqui dibujo el personaje

painter.setBrush(Qt::red);
painter.drawEllipse(
        playerx * tilesize - 5,
        playery * tilesize -5,
        10,10
        );


//Aqui dibujo el enemigo

if(enemyalive)
{painter.setBrush(Qt::blue);
painter.drawEllipse(enemyx * tilesize - 5,enemyy * tilesize - 5,10,10);}

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

//**********************Raycasting********************************
    int numrays = 120;
 //**********************3D***************************************
    //el cielo y el piso

    painter.fillRect(400,0,width()-400,height()/2,Qt::darkBlue);
    painter.fillRect(400,height()/2,width()-400,height()/2,Qt::darkGray);


 //**********************3D***************************************

    for(int i = 0; i < numrays; i++)
    {
        double rayangle =
            angle - fov/2 +
            (double)i / numrays * fov;

        double rayx = playerx;
        double rayy = playery;

        double paso = 0.02;

        while(true)
        {
            rayx += cos(rayangle) * paso;
            rayy += sin(rayangle) * paso;

            if(map1[(int)rayy][(int)rayx] == 1)
                break;
        }

        painter.setPen(QPen(Qt::gray,1));

        painter.drawLine(
            playerx * tilesize,
            playery * tilesize,
            rayx * tilesize,
            rayy * tilesize
            );
 //**********************Raycasting********************************

 //**********************3D***************************************

        double distancia = sqrt((rayx-playerx)*(rayx *playerx)+(rayy-playery)*(rayy-playery));
        distancia *= cos(rayangle -angle);


        //Aqui van las sombras del 3d
        int brillo = 255 - distancia * 40;
        if(brillo < 30){brillo = 30;}
        if(brillo > 255){brillo = 255;}

        QColor color(brillo,brillo,brillo);

        int wallaltura = 1080/ distancia;
        int screenx = 700+i * 5;


        int wallTop = height()/2 - wallaltura/2;
        int wallBottom = height()/2 + wallaltura/2;

        painter.setPen(QPen(color,4));
        painter.drawLine(screenx,wallTop,screenx,wallBottom);
    }

 //**********************3D***************************************

//***********************distancia enemigo************************
    double distanciaEnemigo = sqrt(dx *dx+dy*dy);
    if(distanciaEnemigo > 0.2){
    if( enemyalive && distanciaEnemigo > 0.2)
        {vida--;}}
    if(vida < 0){vida = 0;}
    if(vida <= 0)
    {
        painter.setPen(Qt::red);
        painter.drawText(width()/2 - 50,height()/2,"GAME OVER");
    }

    painter.setPen(Qt::white);
    painter.drawText(500,50,QString("Distancia: %1").arg(distanciaEnemigo));
    if(distanciaEnemigo > 0.2)
    {
        double velocidadEnemigo = 0.01;
        enemyx += (dx / distanciaEnemigo) * velocidadEnemigo;
        enemyy += (dy / distanciaEnemigo) * velocidadEnemigo;
    }

    painter.setPen(Qt::white);
    painter.drawText(500,30,QString("Vida: %1").arg(vida));

    int centerx = width() / 2;
    int centery = height() / 2;
//lineas verticales
    painter.setPen(QPen(Qt::white,2));
    painter.drawLine(
        centerx -10, centery, centerx+10,centery);

//lineas horizontales
    painter.setPen(QPen(Qt::white,2));
    painter.drawLine(centerx , centery-10, centerx,centery+10);


}

//***********************Movimiento********************************

void Game_mode2::keyPressEvent(QKeyEvent *event)
{
    double velocidad = 0.2;
    double rotacion = 0.1;

    switch(event->key())
    {
    case Qt::Key_W:
    {
        double newx = playerx + cos(angle) * velocidad;
        double newy = playery + sin(angle) * velocidad;

        if(map1[(int)newy][(int)newx] == 0)
        {
            playerx = newx;
            playery = newy;
        }

        break;
    }

    case Qt::Key_S:
    {
        double newx = playerx - cos(angle) * velocidad;
        double newy = playery - sin(angle) * velocidad;

        if(map1[(int)newy][(int)newx] == 0)
        {
            playerx = newx;
            playery = newy;
        }

        break;
    }

    case Qt::Key_Left:
        angle -= rotacion;
        break;

    case Qt::Key_Right:
        angle += rotacion;
        break;

    case Qt::Key_Space:
    {
        if(enemyalive)
        {
            double dx = enemyx - playerx;
            double dy = enemyy - playery;

            double distancia =
                sqrt(dx*dx + dy*dy);

            double dirx = dx / distancia;
            double diry = dy / distancia;

            double viewx = cos(angle);
            double viewy = sin(angle);

            double dot =dirx * viewx +diry * viewy;
            if(dot > 0.95 && distancia < 4.0)
            {
                enemyalive = false;
            }
        }

        break;
    }

    }

    update();
}
//***********************Movimiento********************************
