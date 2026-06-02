#include "game_mode2.h"
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QPixmap>
Game_mode2::Game_mode2(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1536, 1024);
    setWindowTitle("Game_mode2");
    setFocusPolicy(Qt::StrongFocus);

    enemysprite.load("xd.png");
}

void Game_mode2::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(450, 360, "GAME MODE 2");

    //***********Mapa vista cenital (2D)**********************
    int tilesize = 50;
    for (int fila = 0; fila < 8; fila++) {
        for (int columna = 0; columna < 8; columna++) {
            if (map1[fila][columna] == 1) {
                painter.fillRect(columna * tilesize, fila * tilesize, tilesize, tilesize, Qt::white);
            }
            painter.setPen(QPen(Qt::gray, 1));
            painter.drawRect(columna * tilesize, fila * tilesize, tilesize, tilesize);
        }
    }

    // Dibujo del personaje en 2D
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(playerx * tilesize - 5, playery * tilesize - 5, 10, 10);

    // Dibujo del enemigo en 2D
    if (enemyalive) {
        painter.setBrush(Qt::blue);
        painter.drawEllipse(enemyx * tilesize - 5, enemyy * tilesize - 5, 10, 10);
    }

    // Línea de visión 2D
    int linelenght = 30;
    int x1 = playerx * tilesize;
    int y1 = playery * tilesize;
    int x2 = x1 + cos(angle) * linelenght;
    int y2 = y1 + sin(angle) * linelenght;
    painter.setPen(QPen(Qt::green, 2));
    painter.drawLine(x1, y1, x2, y2);

    //********************** 3D RAYCASTING ********************************
    int viewportX = 400;
    int viewportWidth = width() - viewportX;
    double rayWidth = (double)viewportWidth / numrays; // Ancho dinámico de cada rayo en pantalla

    // Cielo y Piso
    painter.fillRect(viewportX, 0, viewportWidth, height() / 2, Qt::darkBlue);
    painter.fillRect(viewportX, height() / 2, viewportWidth, height() / 2, Qt::darkGray);

    for (int i = 0; i < numrays; i++) {
        double rayangle = angle - fov / 2 + (double)i / numrays * fov;
        double rayx = playerx;
        double rayy = playery;
        double paso = 0.02;

        while (true) {
            rayx += cos(rayangle) * paso;
            rayy += sin(rayangle) * paso;
            if (map1[(int)rayy][(int)rayx] == 1) break;
        }

        // Rayos en el mapa 2D
        painter.setPen(QPen(Qt::gray, 1));
        painter.drawLine(playerx * tilesize, playery * tilesize, rayx * tilesize, rayy * tilesize);


        double distancia = sqrt((rayx - playerx) * (rayx - playerx) + (rayy - playery) * (rayy - playery));
        distancia *= cos(rayangle - angle); // Corrección de ojo de pez

        zbuffer[i] = distancia; // Guardamos la distancia en el Z-Buffer

        // Sombreado
        int brillo = 255 - distancia * 40;
        if (brillo < 30) brillo = 30;
        if (brillo > 255) brillo = 255;
        QColor color(brillo, brillo, brillo);

        int wallaltura = 1080 / distancia;
        double screenx = viewportX + i * rayWidth;

        int wallTop = height() / 2 - wallaltura / 2;
        int wallBottom = height() / 2 + wallaltura / 2;

        painter.setPen(QPen(color, std::ceil(rayWidth))); // Ancho de pluma adaptado al ancho del rayo
        painter.drawLine(screenx, wallTop, screenx, wallBottom);
    }

    //*********************** ENEMIGO 3D ************************
    double dx = playerx - enemyx;
    double dy = playery - enemyy;
    double distanciaEnemigo = sqrt(dx * dx + dy * dy);

    if (distanciaEnemigo > 0.2) {
        if (enemyalive) {
            if(distanciaEnemigo < 0.7){vida --;}

            double dx2 = enemyx - playerx;
            double dy2 = enemyy - playery;

            // CORRECCIÓN 3: Uso de dy2 en lugar de dy
            double distanciaEnemigo2 = sqrt(dx2 * dx2 + dy2 * dy2);
            double enemyangle = atan2(dy2, dx2);
            double angleDiferencia = enemyangle - angle;

            while (angleDiferencia > M_PI) angleDiferencia -= 2 * M_PI;
            while (angleDiferencia < -M_PI) angleDiferencia += 2 * M_PI;

            if (fabs(angleDiferencia) < fov / 2) {


                double rayIndexFloat = (angleDiferencia + fov / 2) / fov * numrays;
                int rayindex = (int)rayIndexFloat;


                if (rayindex >= 0 && rayindex < numrays) {
                    if (distanciaEnemigo2 < zbuffer[rayindex]) {

                        double screenx = viewportX + rayIndexFloat * rayWidth;
                        int spriteHeight = 400 / distanciaEnemigo2;
                        int spriteWeight = spriteHeight / 2;

                        /*
                        painter.setBrush(Qt::red);
                        painter.setPen(Qt::NoPen);
                        painter.drawRect(screenx - spriteWeight / 2, height() / 2 - spriteHeight / 2, spriteWeight, spriteHeight);
                        */

                        painter.drawPixmap(screenx-spriteWeight/2, height()/2-spriteHeight / 2, spriteWeight,spriteHeight,enemysprite);
                    }

                }
            }
        }

        // Movimiento del enemigo (corregido para que no transpasen las paredes)
        double velocidadEnemigo = 0.01;

        double moveX = (dx / distanciaEnemigo) * velocidadEnemigo;
        double moveY = (dy / distanciaEnemigo) * velocidadEnemigo;

        double newEnemyX = enemyx + moveX;
        double newEnemyY = enemyy + moveY;

        // Movimiento normal
        if(map1[(int)newEnemyY][(int)newEnemyX] == 0)
        {
            enemyx = newEnemyX;
            enemyy = newEnemyY;
        }
        else
        {
            // Intentar mover solo en X
            if(map1[(int)enemyy][(int)newEnemyX] == 0)
            {
                enemyx = newEnemyX;
            }

            // Intentar mover solo en Y
            if(map1[(int)newEnemyY][(int)enemyx] == 0)
            {
                enemyy = newEnemyY;
            }
        }
    }

    // UI y textos
    if (vida < 0) vida = 0;
    if (vida <= 0) {
        painter.setPen(Qt::red);
        painter.drawText(width() / 2 - 50, height() / 2, "GAME OVER");
    }

    painter.setPen(Qt::white);
    painter.drawText(500, 50, QString("Distancia: %1").arg(distanciaEnemigo));
    painter.drawText(500, 30, QString("Vida: %1").arg(vida));

    // Retícula (Crosshair)
    int centerx = viewportX + viewportWidth / 2; // Centrado en el viewport 3D
    int centery = height() / 2;
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(centerx - 10, centery, centerx + 10, centery);
    painter.drawLine(centerx, centery - 10, centerx, centery + 10);
}

void Game_mode2::keyPressEvent(QKeyEvent *event) {
    double velocidad = 0.2;
    double rotacion = 0.1;

    switch (event->key()) {
    case Qt::Key_W: {
        double newx = playerx + cos(angle) * velocidad;
        double newy = playery + sin(angle) * velocidad;
        if (map1[(int)newy][(int)newx] == 0) {
            playerx = newx;
            playery = newy;
        }
        break;
    }
    case Qt::Key_S: {
        double newx = playerx - cos(angle) * velocidad;
        double newy = playery - sin(angle) * velocidad;
        if (map1[(int)newy][(int)newx] == 0) {
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
    case Qt::Key_Space: {
        if (enemyalive) {
            double dx = enemyx - playerx;
            double dy = enemyy - playery;
            double distancia = sqrt(dx * dx + dy * dy);
            double dirx = dx / distancia;
            double diry = dy / distancia;
            double viewx = cos(angle);
            double viewy = sin(angle);
            double dot = dirx * viewx + diry * viewy;

            if (dot > 0.95 && distancia < 4.0) {
                enemyalive = false;
            }
        }
        break;
    }
    }
    update(); // Obliga a repintar la pantalla después de procesar el input
}
