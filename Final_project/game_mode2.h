#ifndef GAME_MODE2_H
#define GAME_MODE2_H

#include <QWidget>
#include <QKeyEvent>
#include <QPaintEvent>
#include <cmath>
#include <QPixmap>

class Game_mode2 : public QWidget
{
    Q_OBJECT
public:
    explicit Game_mode2(QWidget *parent = nullptr);

private:
    //***********************Mapa vista cenital************************
    int map1[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,1,1,0,0,1},
        {1,0,0,1,1,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };

    int vida = 100;
    double playerx = 2.5;
    double playery = 2.5;

    //***********************Enemy************************
    bool enemyalive = true;
    double enemyx = 5.5;
    double enemyy = 3.5;

    //***********************Movimiento********************************
    double angle = 0;

    //**********************Raycasting********************************
    double fov = M_PI / 3.0;
    int numrays = 120; // Movido aquí para poder usarlo como tamaño del Z-buffer dinámicamente si quisieras

    // Z-Buffer para ocultar al enemigo detrás de las paredes
    double zbuffer[120];

    // para el sprite del enemigo

    QPixmap enemysprite;
    QPixmap pistolasprite;
    QPixmap texturawall;
    QPixmap heaven;
    QPixmap Floor;

    // disparito

    bool disparando = false;
    int tiemposhoot = 0;

protected:
    //************ESQUELETO*****************************
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAME_MODE2_H
