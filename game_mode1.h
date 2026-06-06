#ifndef GAME_MODE1_H
#define GAME_MODE1_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QPainter>
#include <QVector>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <cmath>

enum class EstadoJuego {
    SeleccionPersonaje,
    Jugando,
    FinDePartida
};


class Jugador {
public:
    Jugador(float posicionX,
            float posicionY,
            int   teclaMoverIzquierda,
            int   teclaMoverDerecha,
            int   teclaSaltar,
            int   teclaGolpear);

    void mover(float deltaTiempo);
    void saltar();
    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    void iniciarGolpe();
    void actualizarGolpe();
    void controlarConIA(bool moverIzquierda,bool moverDerecha,bool realizarSalto,bool realizarGolpe);
    float posicionX;
    float posicionY;
    float velocidadX;
    float velocidadY;
    float anchoHitbox;
    float altoHitbox;

    bool  estaEnSuelo;
    bool  golpeandoActivo;
    int   timerGolpe;


    float gravedad;
    float velocidadMovimiento;
    float fuerzaSalto;

private:

    int teclaMoverIzquierda;
    int teclaMoverDerecha;
    int teclaSaltar;
    int teclaGolpear;


    bool presionandoIzquierda;
    bool presionandoDerecha;
};

class Balon {
public:
    Balon(float posicionInicialX, float posicionInicialY);

    void mover(float deltaTiempo);
    void rebotar();
    void reiniciar();

    float posicionX;
    float posicionY;
    float velocidadX;
    float velocidadY;
    float radio;

private:
    float posicionInicialX;
    float posicionInicialY;
    float gravedad;
    float factorRebote;
    float factorFriccion;
};

class Game_mode1 : public QWidget
{
    Q_OBJECT

public:
    explicit Game_mode1(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *evento)  override;
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;

private slots:
    void actualizar();

private:
    EstadoJuego estadoActual = EstadoJuego::SeleccionPersonaje;
    int  personajeSeleccionado  = 0;
    int  personajeIA            = 0;
    int  indiceSeleccion        = 0;
    bool confirmoSeleccion      = false;

    QString nombresPersonaje[3] = { "Alex", "Lewis", "Pierre" };
    QPixmap spritesPersonaje[3];
    QPixmap spriteCorrerPersonaje[3];
    QPixmap spriteSaltoPersonaje[3];
    QPixmap spriteGolpePersonaje[3];


    QPixmap spriteBalon;
    QPixmap spriteCorrer;
    QPixmap spriteSalto;
    QPixmap spriteGolpe;

    QPixmap spriteTribunas;
    QPixmap spriteCancha;
    QPixmap spritePorteria;
    QPixmap spriteClock;
    QPixmap spriteFondoSeleccion;

    QMediaPlayer *musicaMenu   = nullptr;
    QMediaPlayer *musicaJuego  = nullptr;
    QAudioOutput *audioMenu    = nullptr;
    QAudioOutput *audioJuego   = nullptr;

    void iniciarPartida();
    void reiniciarPartidaCompleta();
    void actualizarIA();
    void verificarGol();
    void reiniciarRonda();
    void resolverColisionJugadorBalon(Jugador *jugador, float fuerzaGolpe);
    void resolverColisionEntreJugadores();
    void verificarDisputaBalon();

    void reproducirMusicaMenu();
    void reproducirMusicaJuego();
    void detenerMusica();

    void dibujarSeleccionPersonaje(QPainter &pintor);
    void dibujarFinDePartida(QPainter &pintor);
    void dibujarFondo(QPainter &pintor);
    void dibujarTribunas(QPainter &pintor);
    void dibujarCancha(QPainter &pintor);
    void dibujarPorteria(QPainter &pintor, int posicionX, bool esPorteriaIzquierda);
    void dibujarJugador(QPainter &pintor,
                        Jugador  *jugador,
                        int       frameAnimacion,
                        bool      miraDerecha,
                        bool      estaGolpeando);
    void dibujarBalon(QPainter &pintor);
    void dibujarMarcador(QPainter &pintor);
    void dibujarTimer(QPainter &pintor);

    QTimer  *timerJuego;
    Jugador *jugador1 = nullptr;
    Jugador *jugador2 = nullptr;
    Balon   *balon    = nullptr;

    int anchoPantalla  = 1080;
    int altoPantalla   = 720;
    int nivelSuelo     = 600;

    const int anchoPorteria  = 150;
    const int altoPorteria   = 400;
    const int posYPorteria   = 200;

    int  puntosJugador1    = 0;
    int  puntosJugador2    = 0;
    int  segundosTotales   = 90;
    int  ticksAcumulados   = 0;
    int  ticksPorSegundo   = 62;
    bool tiempoAgotado     = false;


    float radioVisual = 28.0f;

    int frameAnimacionJugador1 = 0;
    int frameAnimacionJugador2 = 0;
    int frameAnimacionBalon    = 0;
    int frameAnimacionClock    = 0;
    int contadorTickAnimacion  = 0;


    bool miraDerecha_Jugador1 = true;
    bool miraDerecha_Jugador2 = false;

    int  timerFlashGol     = 0;
    bool flashGolIzquierda = false;
};

#endif // GAME_MODE1_H