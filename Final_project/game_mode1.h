#ifndef GAME_MODE1_H
#define GAME_MODE1_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <cmath>

// Define structure to Player. Position save, velocity and keyboard status.

class player{
public:
    player(float x, float y, int keyIzq, int keyDer, int keySalto);

    void move(float dt);
    void jump();
    void keypush(int key);
    void keydrop(int key);

    float x, y;
    float vx, vy;
    float width, height;
    bool down;

private:
    float gravity;
    float velocidadmov;
    float fuerzasalto;

    int keyIzq;
    int keyDer;
    int keysalto;

    bool pushIzq;
    bool pushDer;
};

//Define structure to Ball. Position, physic and rebound.

class ball{

public:
    ball(float x, float y);

    void move(float dt);
    void rebote();
    void reset();

    float x, y;
    float vx, vy;
    float radius;
    float startX, startY;

private:

    float gravity;
    float rebotar;
    float friction;
};

//Main class, unites everythings and manages teh game loop.

class Game_mode1: public QWidget
{
    Q_OBJECT

public:
    explicit Game_mode1(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;

private slots:
    void update();

private:
    QTimer *gameTimer;
    player *player1 ;
    player *player2;
    ball *ball;

    int windowwidth = 1080;
    int windowhwight = 720;
    int levelfloor= 600;


};

#endif // GAME_MODE1_H
