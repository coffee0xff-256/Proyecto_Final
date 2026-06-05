#ifndef GAME_MODE2_H
#define GAME_MODE2_H

#include <QWidget>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QTimer>
#include <cmath>
#include <QPixmap>
#include <vector>

// Seguimos usando nuestra clase Enemigo
class Enemigo {
public:
    double x;
    double y;
    bool vivo;
    double velocidad;
    QPixmap sprite;
    double distanciaAlJugador;

    Enemigo(double startX, double startY, double vel, QPixmap tex) {
        x = startX;
        y = startY;
        velocidad = vel;
        sprite = tex;
        vivo = true;
        distanciaAlJugador = 0.0;
    }
};

class Game_mode2 : public QWidget
{
    Q_OBJECT
public:
    explicit Game_mode2(QWidget *parent = nullptr);

private slots:
    void bucleJuego();

private:
    //*********************** Mapa vista cenital (MAZMORRA 16x16) ************************
    int map1[16][16] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1},
        {1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1},
        {1,0,1,0,0,0,0,0,1,0,1,1,1,0,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
        {1,0,1,1,1,1,1,0,1,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,1},
        {1,1,1,1,1,0,1,0,1,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    int vida = 100;
    // Te aparecemos en una esquina segura de la mazmorra
    double playerx = 1.5;
    double playery = 1.5;

    //*********************** Sistema de Niveles ************************
    int nivelActual = 1;
    std::vector<Enemigo> listaEnemigos;
    std::vector<QPixmap> spritesDisponibles;

    //***********************Motor y Movimiento********************************
    double angle = 0;
    QTimer *timerJuego;

    //**********************Raycasting********************************
    double fov = M_PI / 3.0;
    int numrays = 120;
    double zbuffer[120];

    QPixmap pistolasprite;
    QPixmap texturawall;
    QPixmap heaven;
    QPixmap Floor;
    QPixmap shoot;
    QPixmap center_shot;

    bool disparando = false;
    int tiemposhoot = 0;

    void cargarNivel();
    bool colisionBalaPared(double x1, double y1, double x2, double y2);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAME_MODE2_H