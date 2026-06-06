#ifndef GAME_MODE1_H
#define GAME_MODE1_H
#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <cmath>

//Definicion de los estados principales del juego para controlar el flujo
enum EstadoJuego { SeleccionPersonaje, Jugando, FinDePartida };

//Implementacion de Jugador traer consigo informacion de fisicas, teclas y estado de golpe
class Jugador {
public:
    Jugador(float x, float y, int keyIzq, int keyDer, int keySalto, int keyGolpe);
    void mover(float dt);
    void saltar();
    void keypush(int key);
    void keydrop(int key);
    void iniciar_golpe();
    void actualizar_golpe();
    void controlIA(bool left, bool right, bool jump, bool hit);
    float x, y, vx, vy, width, height;
    bool down;
    bool golpeando_active() const { return estado_golpeando; }
private:
    float gravity, velocidadmov, fuerzasalto;
    int keyIzq, keyDer, keysalto, keygolpe;
    bool pushIzq, pushDer;
    bool estado_golpeando;
    int timer_golpe;
};

//Implementacion de Balon manejar fisicas de rebote y limites
class Balon {
public:
    Balon(float x, float y);
    void mover(float dt);
    void rebote_suelo();
    void rebote_pared();
    void reset();
    float x, y, vx, vy, radius, startX, startY;
private:
    float gravity, rebotar, friction;
};

//Implementacion de Game_mode1 clase principal que une logica y renderizado
class Game_mode1: public QWidget {
    Q_OBJECT
public:
    explicit Game_mode1(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void actualizar();
private:
    void updateIA();
    void dibujo_jugador(QPainter &p, Jugador *jugador, int frames_animados, bool frente_derecho);
    void resolver_colisiones();
    void gol_realizado(int jugador);
    void reset_round();
    EstadoJuego estado_actual;
    QTimer *gameTimer;
    Jugador *player1;
    Jugador *player2;
    Balon *balon;
    int windowwidth = 1080, windowheight = 720, levelfloor = 600;
    int scoreP1 = 0, scoreP2 = 0, tiempo_restante = 90;
    int tick_segundo = 0;
    QPixmap spriteBall, spriteRun, spriteSalto, spriteGolpe;
    int animFramePlayer1 = 0, animFramePlayer2 = 0, animFrameBall = 0, animTick = 0;
    bool Frente_derecho_player1 = true, Frente_derecho_player2 = false;
};
#endif // GAME_MODE1_H