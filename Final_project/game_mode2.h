#ifndef GAME_MODE2_H
#define GAME_MODE2_H

#include <QWidget>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QTimer>
#include <cmath>
#include <QPixmap>
#include <vector>

// librerias para la musiquita
#include <QMediaPlayer>
#include <QAudioOutput>


//implementacion de POO, para los enemigos.
class Enemigo {
public:
    double x;
    double y;
    bool vivo;
    double velocidad;
    QPixmap sprite;
    double distanciaAlJugador;


    // utilizo los start para sobrecargas de enemigos mas adelante
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

    //musiquita
    QMediaPlayer *music;
    QAudioOutput *audioOutput;

    QMediaPlayer *gunSound;
    QAudioOutput *gunAudio;


    // mapa con vista cenital
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

    //aqui spawneamos al jugador

    double playerx = 1.5;
    double playery = 1.5;

    //niveles, asi tambien como los diferentes enemigos, mediante un vector
    int nivelActual = 1;
    std::vector<Enemigo> listaEnemigos;
    std::vector<QPixmap> spritesDisponibles;

    // inicio del motor y el movimiento
    double angle = 0;
    QTimer *timerJuego;

    //implementacion e inicio del raycasting
    double fov = M_PI / 3.0;
    int numrays = 120;
    double zbuffer[120];


    //los diferentes sprites que utilizamos
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