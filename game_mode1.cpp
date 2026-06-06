#include "game_mode1.h"
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QColor>
#include <QLinearGradient>
#include <QRadialGradient>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Implementacion de Jugador: posicion, velocidad, hitbox, fisicas y estado de golpe.
Jugador::Jugador(float posicionX, float posicionY,int teclaMoverIzquierda, int teclaMoverDerecha,int teclaSaltar,int teclaGolpear)
{
    this->posicionX = posicionX;
    this->posicionY    = posicionY;
    this->velocidadX          = 0;
    this->velocidadY          = 0;
    this->anchoHitbox         = 72;
    this->altoHitbox          = 130;
    this->gravedad            = 1200.0f;
    this->velocidadMovimiento = 320.0f;
    this->fuerzaSalto         = -620.0f;
    this->teclaMoverIzquierda = teclaMoverIzquierda;
    this->teclaMoverDerecha   = teclaMoverDerecha;
    this->teclaSaltar         = teclaSaltar;
    this->teclaGolpear        = teclaGolpear;
    this->presionandoIzquierda = false;
    this->presionandoDerecha   = false;
    this->estaEnSuelo     = false;
    this->golpeandoActivo = false;
    this->timerGolpe      = 0;
}

void Jugador::mover(float deltaTiempo)
{
    velocidadX = 0;
    if (presionandoIzquierda) velocidadX = -velocidadMovimiento;
    if (presionandoDerecha)   velocidadX =  velocidadMovimiento;
    velocidadY += gravedad * deltaTiempo;
    posicionX  += velocidadX * deltaTiempo;
    posicionY  += velocidadY * deltaTiempo;
}

void Jugador::controlarConIA(bool moverIzquierda, bool moverDerecha,
                             bool realizarSalto,  bool realizarGolpe)
{
    presionandoIzquierda = moverIzquierda;
    presionandoDerecha   = moverDerecha;
    if (realizarSalto) saltar();
    if (realizarGolpe) iniciarGolpe();
}

void Jugador::iniciarGolpe()
{
    if (!golpeandoActivo) {
        golpeandoActivo = true;
        timerGolpe      = 18; // frames que dura la animacion
    }
}

void Jugador::actualizarGolpe()
{
    if (golpeandoActivo) {
        timerGolpe--;
        if (timerGolpe <= 0) golpeandoActivo = false;
    }
}

void Jugador::saltar()
{
    if (estaEnSuelo) {
        velocidadY  = fuerzaSalto;
        estaEnSuelo = false;
    }
}

void Jugador::teclaPresionada(int tecla)
{
    if (tecla == teclaMoverIzquierda) presionandoIzquierda = true;
    if (tecla == teclaMoverDerecha)   presionandoDerecha   = true;
    if (tecla == teclaSaltar)         saltar();
    if (tecla == teclaGolpear)        iniciarGolpe();
}

void Jugador::teclaSoltada(int tecla)
{
    if (tecla == teclaMoverIzquierda) presionandoIzquierda = false;
    if (tecla == teclaMoverDerecha)   presionandoDerecha   = false;
}

// Implementacion de Balon: posicion, velocidad, gravedad y rebote.
Balon::Balon(float posicionInicialX, float posicionInicialY)
{
    this->posicionX        = posicionInicialX;
    this->posicionY        = posicionInicialY;
    this->velocidadX       = 0;
    this->velocidadY       = 0;
    this->radio            = 20;
    this->posicionInicialX = posicionInicialX;
    this->posicionInicialY = posicionInicialY;
    this->gravedad         = 1200.0f;
    this->factorRebote     = 0.62f;
    this->factorFriccion   = 0.86f;
}

void Balon::mover(float deltaTiempo)
{
    velocidadY += gravedad * deltaTiempo;
    posicionX  += velocidadX * deltaTiempo;
    posicionY  += velocidadY * deltaTiempo;
}

void Balon::rebotar()
{
    velocidadY = -velocidadY * factorRebote;
    velocidadX =  velocidadX * factorFriccion;
}

void Balon::reiniciar()
{
    posicionX  = posicionInicialX;
    posicionY  = posicionInicialY;
    velocidadX = 0;
    velocidadY = 0;
}

// Implementacion de Game_mode1: constructor, logica, renderizado y eventos.
Game_mode1::Game_mode1(QWidget *parent)
    : QWidget{parent}
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    resize(anchoPantalla, altoPantalla);
    setWindowTitle("Head Football");
    setFocusPolicy(Qt::StrongFocus);

    // Cartas de seleccion — [0] Alex  [1] Lewis  [2] Pierre
    spritesPersonaje[0] = QPixmap(":/sprites_gm1/Alex.png");
    spritesPersonaje[1] = QPixmap(":/sprites_gm1/Lewis.png");
    spritesPersonaje[2] = QPixmap(":/sprites_gm1/Pierre.png");

    // Sprites correr — [0] Alex  [1] Lewis  [2] Pierre
    spriteCorrerPersonaje[0] = QPixmap(":/sprites_gm1/Run_Alex.png");
    spriteCorrerPersonaje[1] = QPixmap(":/sprites_gm1/Lewi_Run.png");
    spriteCorrerPersonaje[2] = QPixmap(":/sprites_gm1/Run_Pierre.png");

    // Sprites salto — [0] Alex  [1] Lewis  [2] Pierre
    spriteSaltoPersonaje[0] = QPixmap(":/sprites_gm1/Salto_Alex.png");
    spriteSaltoPersonaje[1] = QPixmap(":/sprites_gm1/Jump_Lewis.png");
    spriteSaltoPersonaje[2] = QPixmap(":/sprites_gm1/Jump_Pierre.png");

    // Sprites golpe — [0] Alex  [1] Lewis  [2] Pierre
    spriteGolpePersonaje[0] = QPixmap(":/sprites_gm1/");
    spriteGolpePersonaje[1] = QPixmap(":/sprites_gm1/");
    spriteGolpePersonaje[2] = QPixmap(":/sprites_gm1/");

    // Sprites genericos (fallback)
    spriteBalon  = QPixmap(":/sprites_gm1/Balon.png");
    spriteCorrer = QPixmap(":/sprites_gm1/Run_Generic.png");
    spriteSalto  = QPixmap(":/sprites_gm1/Jump_Generic.png");
    spriteGolpe  = QPixmap(":/sprites_gm1/Kick_Generic.png");

    // Escenario
    spriteTribunas       = QPixmap(":/sprites_gm1/Tribuna.png");
    spriteCancha         = QPixmap(":/sprites_gm1/Cancha.png");
    spritePorteria       = QPixmap(":/sprites_gm1/Porteria.png");
    spriteClock          = QPixmap(":/sprites_gm1/Reloj.png");
    spriteFondoSeleccion = QPixmap(":/sprites_gm1/FondoSeleccion.png");

    // Musica
    audioMenu  = new QAudioOutput(this);
    audioJuego = new QAudioOutput(this);
    musicaMenu  = new QMediaPlayer(this);
    musicaJuego = new QMediaPlayer(this);
    musicaMenu->setAudioOutput(audioMenu);
    musicaJuego->setAudioOutput(audioJuego);
    musicaMenu->setSource(QUrl("qrc:/musica/sounds_gm1/Dai.mp3"));
    musicaJuego->setSource(QUrl("qrc:/musica/sounds_gm1/Waka.mp3"));
    audioMenu->setVolume(0.7f);
    audioJuego->setVolume(0.8f);
    reproducirMusicaMenu();

    // Timer principal (16 ms ~ 62 fps)
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &Game_mode1::actualizar);
    timerJuego->start(16);
}

void Game_mode1::reproducirMusicaMenu()
{
    musicaJuego->stop();
    musicaMenu->setLoops(QMediaPlayer::Infinite);
    musicaMenu->play();
}

void Game_mode1::reproducirMusicaJuego()
{
    musicaMenu->stop();
    musicaJuego->setLoops(QMediaPlayer::Infinite);
    musicaJuego->play();
}

void Game_mode1::detenerMusica()
{
    musicaMenu->stop();
    musicaJuego->stop();
}

void Game_mode1::iniciarPartida()
{
    delete jugador1; jugador1 = nullptr;
    delete jugador2; jugador2 = nullptr;
    delete balon;    balon    = nullptr;

    // J1: teclado   J2: IA (teclas en 0)
    jugador1 = new Jugador(150, nivelSuelo - 130, Qt::Key_A, Qt::Key_D, Qt::Key_W, Qt::Key_S);
    jugador2 = new Jugador(anchoPantalla - 220, nivelSuelo - 130, 0, 0, 0, 0);
    balon    = new Balon(anchoPantalla / 2.0f, nivelSuelo / 2.0f);

    if (!spriteCorrerPersonaje[personajeSeleccionado].isNull())
        spriteCorrer = spriteCorrerPersonaje[personajeSeleccionado];
    if (!spriteSaltoPersonaje[personajeSeleccionado].isNull())
        spriteSalto = spriteSaltoPersonaje[personajeSeleccionado];
    if (!spriteGolpePersonaje[personajeSeleccionado].isNull())
        spriteGolpe = spriteGolpePersonaje[personajeSeleccionado];

    puntosJugador1 = puntosJugador2 = 0;
    segundosTotales = 90;
    ticksAcumulados = 0;
    tiempoAgotado   = false;
    timerFlashGol   = 0;
    frameAnimacionJugador1 = frameAnimacionJugador2 = 0;
    frameAnimacionBalon    = frameAnimacionClock    = 0;
    contadorTickAnimacion  = 0;
    estadoActual = EstadoJuego::Jugando;
    reproducirMusicaJuego();
}

void Game_mode1::reiniciarPartidaCompleta()
{
    confirmoSeleccion     = false;
    indiceSeleccion       = 0;
    personajeSeleccionado = 0;
    estadoActual          = EstadoJuego::SeleccionPersonaje;
    reproducirMusicaMenu();
}

void Game_mode1::actualizar()
{
    if (estadoActual == EstadoJuego::SeleccionPersonaje ||
        estadoActual == EstadoJuego::FinDePartida) {
        update();
        return;
    }

    if (tiempoAgotado) {
        estadoActual = EstadoJuego::FinDePartida;
        reproducirMusicaMenu();
        update();
        return;
    }

    const float deltaTiempo = 16.0f / 1000.0f;

    jugador1->mover(deltaTiempo);
    jugador2->mover(deltaTiempo);
    balon->mover(deltaTiempo);
    jugador1->actualizarGolpe();
    jugador2->actualizarGolpe();

    auto colisionSuelo = [&](Jugador *j) {
        if (j->posicionY + j->altoHitbox >= nivelSuelo) {
            j->posicionY   = nivelSuelo - j->altoHitbox;
            j->velocidadY  = 0;
            j->estaEnSuelo = true;
        } else {
            j->estaEnSuelo = false;
        }
    };
    colisionSuelo(jugador1);
    colisionSuelo(jugador2);

    if (balon->posicionY + balon->radio >= nivelSuelo) {
        balon->posicionY = nivelSuelo - balon->radio;
        balon->rebotar();
        if (std::abs(balon->velocidadY) < 60.0f) balon->velocidadY = 0;
    }
    if (balon->posicionY - balon->radio <= 0) {
        balon->posicionY  = balon->radio;
        balon->velocidadY = std::abs(balon->velocidadY) * 0.5f;
    }

    resolverColisionJugadorBalon(jugador1, 750.0f);
    resolverColisionJugadorBalon(jugador2, 750.0f);
    resolverColisionEntreJugadores();
    verificarDisputaBalon();

    auto limitarJugador = [&](Jugador *j) {
        if (j->posicionX < 0)                               j->posicionX = 0;
        if (j->posicionX + j->anchoHitbox > anchoPantalla) j->posicionX = anchoPantalla - j->anchoHitbox;
    };
    limitarJugador(jugador1);
    limitarJugador(jugador2);

    if (balon->posicionX - balon->radio < 0) {
        balon->posicionX  = balon->radio;
        balon->velocidadX = std::abs(balon->velocidadX) * 0.72f;
    }
    if (balon->posicionX + balon->radio > anchoPantalla) {
        balon->posicionX  = anchoPantalla - balon->radio;
        balon->velocidadX = -std::abs(balon->velocidadX) * 0.72f;
    }

    verificarGol();
    if (timerFlashGol > 0) timerFlashGol--;

    if (jugador1->velocidadX >  10) miraDerecha_Jugador1 = true;
    if (jugador1->velocidadX < -10) miraDerecha_Jugador1 = false;

    actualizarIA();

    if (jugador2->velocidadX >  10) miraDerecha_Jugador2 = true;
    if (jugador2->velocidadX < -10) miraDerecha_Jugador2 = false;
    if (std::abs(jugador2->velocidadX) <= 10)
        miraDerecha_Jugador2 = (balon->posicionX > jugador2->posicionX + jugador2->anchoHitbox / 2.0f);

    contadorTickAnimacion++;
    if (contadorTickAnimacion >= 6) {
        contadorTickAnimacion = 0;
        frameAnimacionJugador1 = (frameAnimacionJugador1 + 1) % 8;
        frameAnimacionJugador2 = (frameAnimacionJugador2 + 1) % 8;
        frameAnimacionBalon    = (frameAnimacionBalon    + 1) % 6;
        frameAnimacionClock    = (frameAnimacionClock    + 1) % 8;
    }

    ticksAcumulados++;
    if (ticksAcumulados >= ticksPorSegundo) {
        ticksAcumulados = 0;
        if (segundosTotales > 0) segundosTotales--;
        else tiempoAgotado = true;
    }

    update();
}

void Game_mode1::resolverColisionJugadorBalon(Jugador *jugador, float fuerzaGolpe)
{
    float centroJugadorX = jugador->posicionX + jugador->anchoHitbox / 2.0f;
    float centroJugadorY = jugador->posicionY + jugador->altoHitbox  / 2.0f;
    float diferenciaX   = balon->posicionX - centroJugadorX;
    float diferenciaY   = balon->posicionY - centroJugadorY;
    float distancia     = std::sqrt(diferenciaX * diferenciaX + diferenciaY * diferenciaY);
    float radioColision = jugador->anchoHitbox / 2.0f + balon->radio;

    if (distancia < radioColision && distancia > 0.0f) {
        float normalX = diferenciaX / distancia;
        float normalY = diferenciaY / distancia;
        balon->posicionX = centroJugadorX + normalX * radioColision;
        balon->posicionY = centroJugadorY + normalY * radioColision;

        if (jugador->golpeandoActivo) {
            float dirHorizontal = (jugador == jugador1)
            ? (miraDerecha_Jugador1 ? 1.0f : -1.0f)
            : (miraDerecha_Jugador2 ? 1.0f : -1.0f);
            float mezclaNormalX = normalX * 0.55f + dirHorizontal * 0.45f;
            float mezclaNormalY = normalY * 0.55f - 0.45f;
            float magnitud = std::sqrt(mezclaNormalX * mezclaNormalX + mezclaNormalY * mezclaNormalY);
            if (magnitud > 0.001f) { mezclaNormalX /= magnitud; mezclaNormalY /= magnitud; }
            balon->velocidadX = mezclaNormalX * fuerzaGolpe;
            balon->velocidadY = mezclaNormalY * fuerzaGolpe;
        } else {
            float velocidadRelativaX = balon->velocidadX - jugador->velocidadX;
            float velocidadRelativaY = balon->velocidadY - jugador->velocidadY;
            float productoPunto = velocidadRelativaX * normalX + velocidadRelativaY * normalY;
            if (productoPunto < 0) {
                const float factorRebote = 1.4f;
                balon->velocidadX -= factorRebote * productoPunto * normalX;
                balon->velocidadY -= factorRebote * productoPunto * normalY;
            }
            if (normalY < -0.3f) balon->velocidadY -= 260.0f;
            balon->velocidadX += jugador->velocidadX * 0.35f;
        }

        float velocidadTotal = std::sqrt(balon->velocidadX * balon->velocidadX +
                                         balon->velocidadY * balon->velocidadY);
        if (velocidadTotal > 950.0f) {
            balon->velocidadX = balon->velocidadX / velocidadTotal * 950.0f;
            balon->velocidadY = balon->velocidadY / velocidadTotal * 950.0f;
        }
    }
}

void Game_mode1::resolverColisionEntreJugadores()
{
    float izq1 = jugador1->posicionX, der1 = jugador1->posicionX + jugador1->anchoHitbox;
    float izq2 = jugador2->posicionX, der2 = jugador2->posicionX + jugador2->anchoHitbox;
    float arr1 = jugador1->posicionY, aba1 = jugador1->posicionY + jugador1->altoHitbox;
    float arr2 = jugador2->posicionY, aba2 = jugador2->posicionY + jugador2->altoHitbox;

    if ((izq1 < der2) && (der1 > izq2) && (arr1 < aba2) && (aba1 > arr2)) {
        float penIzq = der1 - izq2;
        float penDer = der2 - izq1;
        const float fuerza = 0.5f;
        if (penIzq < penDer) {
            jugador1->posicionX -= penIzq * fuerza;
            jugador2->posicionX += penIzq * fuerza;
        } else {
            jugador1->posicionX += penDer * fuerza;
            jugador2->posicionX -= penDer * fuerza;
        }
        float velRel = jugador1->velocidadX - jugador2->velocidadX;
        if (std::abs(velRel) < 50.0f) {
            float sign = (jugador1->posicionX < jugador2->posicionX) ? -1.0f : 1.0f;
            jugador1->velocidadX -= 20.0f * sign;
            jugador2->velocidadX += 20.0f * sign;
        }
    }
}

void Game_mode1::verificarDisputaBalon()
{
    float cx1 = jugador1->posicionX + jugador1->anchoHitbox / 2.0f;
    float cx2 = jugador2->posicionX + jugador2->anchoHitbox / 2.0f;
    float dist12 = std::abs(cx2 - cx1);
    float distB1 = std::abs(balon->posicionX - cx1);
    float distB2 = std::abs(balon->posicionX - cx2);
    const float umbral = jugador1->anchoHitbox * 0.9f;
    if (dist12 < umbral && distB1 < umbral && distB2 < umbral) {
        balon->velocidadY = -700.0f;
        balon->velocidadX = ((std::rand() % 200) - 100) * 2.0f;
    }
}

void Game_mode1::verificarGol()
{
    int yArr = posYPorteria, yAba = posYPorteria + altoPorteria;

    if (balon->posicionX - balon->radio <= anchoPorteria &&
        balon->posicionY >= yArr && balon->posicionY <= yAba) {
        puntosJugador2++;
        timerFlashGol = 60; flashGolIzquierda = true;
        reiniciarRonda(); return;
    }
    if (balon->posicionX + balon->radio >= anchoPantalla - anchoPorteria &&
        balon->posicionY >= yArr && balon->posicionY <= yAba) {
        puntosJugador1++;
        timerFlashGol = 60; flashGolIzquierda = false;
        reiniciarRonda();
    }
}

void Game_mode1::reiniciarRonda()
{
    balon->reiniciar();
    jugador1->posicionX  = 150;
    jugador1->posicionY  = nivelSuelo - jugador1->altoHitbox;
    jugador1->velocidadX = jugador1->velocidadY = 0;
    jugador2->posicionX  = anchoPantalla - 220;
    jugador2->posicionY  = nivelSuelo - jugador2->altoHitbox;
    jugador2->velocidadX = jugador2->velocidadY = 0;
}

void Game_mode1::actualizarIA()
{
    float cx = jugador2->posicionX + jugador2->anchoHitbox / 2.0f;
    float cy = jugador2->posicionY + jugador2->altoHitbox  / 2.0f;
    float bx = balon->posicionX, by = balon->posicionY;
    float dist = std::sqrt((bx - cx)*(bx - cx) + (by - cy)*(by - cy));

    float objetivoX = (bx < anchoPantalla * 0.25f) ? bx + 40.0f : bx - jugador2->anchoHitbox / 2.0f;
    objetivoX = std::max(0.0f, std::min((float)(anchoPantalla - jugador2->anchoHitbox), objetivoX));

    float diff = objetivoX - jugador2->posicionX;
    bool izq = (diff < -25.0f), der = (diff > 25.0f);

    bool salto = false;
    if (dist < 160.0f) {
        if (by < cy - 40.0f && jugador2->estaEnSuelo) salto = true;
        if (balon->velocidadY > 0 && by < jugador2->posicionY + jugador2->altoHitbox &&
            by > jugador2->posicionY && jugador2->estaEnSuelo && dist < 170.0f) salto = true;
    }
    if (std::abs(bx - cx) < 80.0f && by < jugador2->posicionY + 30.0f && jugador2->estaEnSuelo)
        salto = true;

    bool golpe = false;
    float rGolpe = jugador2->anchoHitbox / 2.0f + balon->radio + 30.0f;
    if (dist < rGolpe)                                golpe = true;
    if (!jugador2->estaEnSuelo && dist < rGolpe + 20.0f) golpe = true;

    jugador2->controlarConIA(izq, der, salto, golpe);
}

void Game_mode1::paintEvent(QPaintEvent *)
{
    QPainter pintor(this);
    pintor.setRenderHint(QPainter::SmoothPixmapTransform);
    pintor.setRenderHint(QPainter::Antialiasing);

    if (estadoActual == EstadoJuego::SeleccionPersonaje) { dibujarSeleccionPersonaje(pintor); return; }
    if (estadoActual == EstadoJuego::FinDePartida) {
        dibujarFondo(pintor); dibujarTribunas(pintor); dibujarCancha(pintor);
        dibujarFinDePartida(pintor); return;
    }

    dibujarFondo(pintor);
    dibujarTribunas(pintor);
    dibujarCancha(pintor);
    dibujarPorteria(pintor, 0, true);
    dibujarPorteria(pintor, anchoPantalla - anchoPorteria, false);

    if (timerFlashGol > 0 && (timerFlashGol / 6) % 2 == 0) {
        QColor colorFlash(255, 215, 0, 90);
        if (flashGolIzquierda) pintor.fillRect(0, 0, anchoPantalla / 2, nivelSuelo, colorFlash);
        else                   pintor.fillRect(anchoPantalla / 2, 0, anchoPantalla / 2, nivelSuelo, colorFlash);
    }

    dibujarJugador(pintor, jugador1, frameAnimacionJugador1, miraDerecha_Jugador1, jugador1->golpeandoActivo);
    dibujarJugador(pintor, jugador2, frameAnimacionJugador2, miraDerecha_Jugador2, jugador2->golpeandoActivo);
    dibujarBalon(pintor);
    dibujarMarcador(pintor);
    dibujarTimer(pintor);
}

void Game_mode1::dibujarSeleccionPersonaje(QPainter &p)
{
    if (!spriteFondoSeleccion.isNull())
        p.drawPixmap(0, 0, anchoPantalla, altoPantalla, spriteFondoSeleccion);
    else
        p.fillRect(0, 0, anchoPantalla, altoPantalla, QColor(10, 14, 40));

    QFont fTitulo("Bebas Neue", 42, QFont::Bold);
    p.setFont(fTitulo);
    p.setPen(QColor(255, 215, 0));
    p.drawText(QRect(0, 30, anchoPantalla, 70), Qt::AlignCenter, "¡ELIGE TU PERSONAJE!");

    p.setPen(QColor(200, 200, 200));
    QFont fSub("Bebas Neue", 16);
    p.setFont(fSub);
    p.drawText(QRect(0, 100, anchoPantalla, 30), Qt::AlignCenter,
               "← → para navegar   |   ENTER para confirmar");

    const int cardW = 220, cardH = 300;
    const int startX = (anchoPantalla - 3 * cardW - 2 * 40) / 2;
    const int startY = 150;

    for (int i = 0; i < 3; i++) {
        int cx = startX + i * (cardW + 40);
        bool seleccionado = (i == indiceSeleccion);

        p.setBrush(seleccionado ? QColor(255, 215, 0, 50) : QColor(255, 255, 255, 15));
        p.setPen(seleccionado ? QPen(QColor(255, 215, 0), 3) : QPen(QColor(120, 120, 120), 1));
        p.drawRoundedRect(cx, startY, cardW, cardH, 16, 16);

        QRect fotoRect(cx + 10, startY + 10, cardW - 20, cardH - 70);
        if (!spritesPersonaje[i].isNull()) {
            p.drawPixmap(fotoRect, spritesPersonaje[i]);
        } else {
            QColor colores[3] = { QColor(0,120,255,180), QColor(220,60,60,180), QColor(60,200,80,180) };
            p.setBrush(colores[i]); p.setPen(Qt::NoPen);
            p.drawRoundedRect(fotoRect, 8, 8);
            p.setPen(Qt::white);
            QFont fSil("Bebas Neue", 48, QFont::Bold); p.setFont(fSil);
            p.drawText(fotoRect, Qt::AlignCenter, nombresPersonaje[i].left(1));
        }

        p.setPen(seleccionado ? QColor(255, 215, 0) : Qt::white);
        QFont fNombre("Bebas Neue", 20); p.setFont(fNombre);
        p.drawText(QRect(cx, startY + cardH - 55, cardW, 40), Qt::AlignCenter, nombresPersonaje[i]);

        if (seleccionado) {
            p.setBrush(QColor(255, 215, 0)); p.setPen(Qt::NoPen);
            p.drawRoundedRect(cx + cardW / 2 - 25, startY - 18, 50, 24, 8, 8);
            p.setPen(QColor(20, 20, 20));
            QFont fTag("Bebas Neue", 11, QFont::Bold); p.setFont(fTag);
            p.drawText(QRect(cx + cardW / 2 - 25, startY - 18, 50, 24), Qt::AlignCenter, "TÚ");
        }
    }

    p.setPen(QColor(160, 160, 160));
    QFont fHint("Bebas Neue", 13); p.setFont(fHint);
    p.drawText(QRect(0, startY + cardH + 30, anchoPantalla, 30), Qt::AlignCenter,
               "Tu rival sera uno aleatorio");

    QRect btnRect(anchoPantalla / 2 - 100, startY + cardH + 70, 200, 50);
    p.setBrush(QColor(40, 160, 40)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(btnRect, 12, 12);
    p.setPen(Qt::white);
    QFont fBtn("Bebas Neue", 22); p.setFont(fBtn);
    p.drawText(btnRect, Qt::AlignCenter, "¡ JUGAR !");
}

void Game_mode1::dibujarFinDePartida(QPainter &p)
{
    p.fillRect(0, 0, anchoPantalla, altoPantalla, QColor(0, 0, 0, 175));

    QRect panel(anchoPantalla / 2 - 280, altoPantalla / 2 - 200, 560, 400);
    p.setBrush(QColor(15, 20, 50, 230));
    p.setPen(QPen(QColor(255, 215, 0), 3));
    p.drawRoundedRect(panel, 20, 20);

    QString resultado;
    QColor  colorResult;
    if      (puntosJugador1 > puntosJugador2) { resultado = "¡JUGADOR 1 GANA!"; colorResult = QColor(0, 220, 100); }
    else if (puntosJugador2 > puntosJugador1) { resultado = "¡CPU GANA!";        colorResult = QColor(255, 80, 80); }
    else                                       { resultado = "¡EMPATE!";          colorResult = QColor(255, 215, 0); }

    p.setPen(colorResult);
    QFont fRes("Bebas Neue", 48, QFont::Bold); p.setFont(fRes);
    p.drawText(QRect(panel.x(), panel.y() + 30, panel.width(), 70), Qt::AlignCenter, resultado);

    p.setPen(Qt::white);
    QFont fScore("Bebas Neue", 64, QFont::Bold); p.setFont(fScore);
    p.drawText(QRect(panel.x(), panel.y() + 110, panel.width(), 90), Qt::AlignCenter,
               QString("%1   :   %2").arg(puntosJugador1).arg(puntosJugador2));

    p.setPen(QColor(180, 180, 180));
    QFont fNoms("Bebas Neue", 14); p.setFont(fNoms);
    QString nomP1 = (personajeSeleccionado >= 0 && personajeSeleccionado < 3) ? nombresPersonaje[personajeSeleccionado] : "Jugador 1";
    QString nomIA = (personajeIA >= 0 && personajeIA < 3) ? nombresPersonaje[personajeIA] + " (IA)" : "CPU";
    p.drawText(QRect(panel.x() + 20,                panel.y() + 200, panel.width() / 2 - 20, 30), Qt::AlignCenter, nomP1);
    p.drawText(QRect(panel.x() + panel.width() / 2, panel.y() + 200, panel.width() / 2 - 20, 30), Qt::AlignCenter, nomIA);

    int btnY = panel.y() + 255, btnW = 220, btnH = 55;
    QFont fBtn("Bebas Neue", 20); p.setFont(fBtn);

    QRect btnRevancha(panel.x() + 40, btnY, btnW, btnH);
    p.setBrush(QColor(30, 140, 30)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(btnRevancha, 12, 12);
    p.setPen(Qt::white); p.drawText(btnRevancha, Qt::AlignCenter, "[R]  REVANCHA");

    QRect btnCambiar(panel.x() + panel.width() - 40 - btnW, btnY, btnW, btnH);
    p.setBrush(QColor(60, 80, 180)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(btnCambiar, 12, 12);
    p.setPen(Qt::white); p.drawText(btnCambiar, Qt::AlignCenter, "[Q]  CAMBIAR");

    p.setPen(QColor(140, 140, 140));
    QFont fHint("Bebas Neue", 12); p.setFont(fHint);
    p.drawText(QRect(panel.x(), panel.y() + 325, panel.width(), 30), Qt::AlignCenter,
               "R = misma seleccion   |   Q = volver a elegir");
}

void Game_mode1::dibujarFondo(QPainter &p)
{
    QLinearGradient cielo(0, 0, 0, nivelSuelo - 80);
    cielo.setColorAt(0.0, QColor(20, 60, 160));
    cielo.setColorAt(1.0, QColor(110, 175, 240));
    p.fillRect(0, 0, anchoPantalla, nivelSuelo, cielo);
    p.fillRect(0, nivelSuelo, anchoPantalla, altoPantalla - nivelSuelo, QColor(80, 50, 20));
    p.fillRect(0, nivelSuelo - 3, anchoPantalla, 3, QColor(255, 255, 255, 100));
}

void Game_mode1::dibujarTribunas(QPainter &p)
{
    if (!spriteTribunas.isNull()) {
        p.drawPixmap(0, 0, anchoPantalla, nivelSuelo - 80, spriteTribunas);
    } else {
        int zonaGradas = nivelSuelo - 90, numFilas = 8, altoFila = zonaGradas / numFilas;
        for (int fila = 0; fila < numFilas; fila++) {
            p.setBrush((fila % 2 == 0) ? QColor(160,30,30) : QColor(200,70,30));
            p.setPen(Qt::NoPen);
            p.drawRect(0, fila * altoFila, anchoPantalla, altoFila);
            p.setBrush(QColor(255, 220, 170, 140)); p.setPen(Qt::NoPen);
            int offsetX = (fila % 2) * 10;
            for (int x = offsetX; x < anchoPantalla; x += 20)
                p.drawEllipse(x, fila * altoFila + 3, 8, 8);
        }
        QLinearGradient sombra(0, zonaGradas, 0, zonaGradas + 30);
        sombra.setColorAt(0.0, QColor(0,0,0,100)); sombra.setColorAt(1.0, QColor(0,0,0,0));
        p.fillRect(0, zonaGradas, anchoPantalla, 30, sombra);
    }
}

void Game_mode1::dibujarCancha(QPainter &p)
{
    int yInicio = nivelSuelo - 90;
    if (!spriteCancha.isNull())
        p.drawPixmap(0, yInicio, anchoPantalla, 90 + (altoPantalla - nivelSuelo), spriteCancha);
    else
        p.fillRect(0, yInicio, anchoPantalla, 90 + (altoPantalla - nivelSuelo), QColor(35, 145, 35));
}

void Game_mode1::dibujarPorteria(QPainter &p, int posicionX, bool esIzquierda)
{
    const int prof = anchoPorteria + 40;

    if (!spritePorteria.isNull()) {
        int aw = prof;
        int ah = (spritePorteria.width() > 0) ? aw * spritePorteria.height() / spritePorteria.width() : altoPorteria;
        int xSp = esIzquierda ? 0 : (posicionX - aw + anchoPorteria);
        int ySp = posYPorteria + altoPorteria - ah;
        p.save();
        if (!esIzquierda) { p.translate(xSp + aw, ySp); p.scale(-1.0, 1.0); p.drawPixmap(0, 0, aw, ah, spritePorteria); }
        else               { p.drawPixmap(xSp, ySp, aw, ah, spritePorteria); }
        p.restore();
    } else {
        QColor colorPoste(230, 230, 230);
        p.setBrush(QColor(0,0,0,60)); p.setPen(Qt::NoPen);
        if (esIzquierda) p.drawRect(0, posYPorteria, prof, altoPorteria);
        else             p.drawRect(posicionX - prof + anchoPorteria, posYPorteria, prof, altoPorteria);

        p.setPen(QPen(QColor(255,255,255,50), 1));
        int xBase = esIzquierda ? 0 : (posicionX - prof + anchoPorteria);
        for (int i = 0; i <= 8; i++) p.drawLine(xBase, posYPorteria + i * altoPorteria / 8, xBase + prof, posYPorteria + i * altoPorteria / 8);
        for (int i = 0; i <= 5; i++) p.drawLine(xBase + i * prof / 5, posYPorteria, xBase + i * prof / 5, posYPorteria + altoPorteria);

        p.setPen(QPen(colorPoste, 5)); p.setBrush(colorPoste);
        if (esIzquierda) {
            p.drawLine(anchoPorteria, posYPorteria, anchoPorteria, posYPorteria + altoPorteria);
            p.drawLine(0, posYPorteria, anchoPorteria, posYPorteria);
            p.drawLine(0, posYPorteria + altoPorteria, anchoPorteria, posYPorteria + altoPorteria);
        } else {
            p.drawLine(posicionX, posYPorteria, posicionX, posYPorteria + altoPorteria);
            p.drawLine(posicionX, posYPorteria, posicionX + anchoPorteria, posYPorteria);
            p.drawLine(posicionX, posYPorteria + altoPorteria, posicionX + anchoPorteria, posYPorteria + altoPorteria);
        }
    }
}

void Game_mode1::dibujarJugador(QPainter &p, Jugador *jugador, int frameAnim, bool miraDerecha, bool estaGolpeando)
{
    bool esJ1 = (jugador == jugador1);
    int  idx  = qBound(0, esJ1 ? personajeSeleccionado : personajeIA, 2);

    QPixmap *sprRun  = (!spriteCorrerPersonaje[idx].isNull()) ? &spriteCorrerPersonaje[idx] : &spriteCorrer;
    QPixmap *sprJump = (!spriteSaltoPersonaje[idx].isNull())  ? &spriteSaltoPersonaje[idx]  : &spriteSalto;
    QPixmap *sprKick = (!spriteGolpePersonaje[idx].isNull())  ? &spriteGolpePersonaje[idx]  : &spriteGolpe;

    QPixmap frameSprite;

    if (estaGolpeando && !sprKick->isNull()) {
        int fk = qBound(0, (18 - jugador->timerGolpe) * 8 / 18, 7);
        int aw = sprKick->width() / 8;
        frameSprite = sprKick->copy(fk * aw, 0, aw, sprKick->height());
    } else if (!jugador->estaEnSuelo && !sprJump->isNull()) {
        int aw = sprJump->width() / 8;
        frameSprite = sprJump->copy(3 * aw, 0, aw, sprJump->height());
    } else if (std::abs(jugador->velocidadX) > 10.0f && !sprRun->isNull()) {
        int aw = sprRun->width() / 8;
        frameSprite = sprRun->copy((frameAnim % 8) * aw, 0, aw, sprRun->height());
    } else if (!sprJump->isNull()) {
        int aw = sprJump->width() / 8;
        frameSprite = sprJump->copy(0, 0, aw, sprJump->height());
    }

    if (frameSprite.isNull()) {
        p.fillRect(QRectF(jugador->posicionX, jugador->posicionY, jugador->anchoHitbox, jugador->altoHitbox),
                   esJ1 ? QColor(0, 120, 255, 200) : QColor(255, 60, 60, 200));
        return;
    }

    int aw = int(jugador->anchoHitbox);
    int ah = (frameSprite.width() > 0) ? (aw * frameSprite.height() / frameSprite.width()) : int(jugador->altoHitbox);
    int xD = int(jugador->posicionX);
    int yD = int(jugador->posicionY) + int(jugador->altoHitbox) - ah;

    p.save();
    if (!miraDerecha) { p.translate(xD + aw, yD); p.scale(-1.0, 1.0); p.drawPixmap(0, 0, aw, ah, frameSprite); }
    else              { p.drawPixmap(xD, yD, aw, ah, frameSprite); }
    p.restore();
}

void Game_mode1::dibujarBalon(QPainter &p)
{
    float rv = radioVisual;
    if (!spriteBalon.isNull()) {
        int anchoF = spriteBalon.width() / 6;
        QPixmap fb = spriteBalon.copy(frameAnimacionBalon * anchoF, 0, anchoF, spriteBalon.height());
        int aDest = int(rv * 2.0f);
        int hDest = (fb.width() > 0) ? aDest * fb.height() / fb.width() : aDest;
        p.drawPixmap(int(balon->posicionX - rv), int(balon->posicionY - hDest / 2.0f), aDest, hDest, fb);
    } else {
        p.setBrush(Qt::white); p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(balon->posicionX, balon->posicionY), double(rv), double(rv));
    }
}

void Game_mode1::dibujarMarcador(QPainter &p)
{
    p.setBrush(QColor(0,0,0,160)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(anchoPantalla / 2 - 100, 8, 200, 55, 10, 10);

    QFont fM("Bebas Neue", 28, QFont::Bold); p.setFont(fM); p.setPen(Qt::white);
    p.drawText(anchoPantalla / 2 - 85, 52, QString::number(puntosJugador1));
    p.drawText(anchoPantalla / 2 + 50, 52, QString::number(puntosJugador2));

    QFont fSep("Bebas Neue", 22, QFont::Bold); p.setFont(fSep); p.setPen(QColor(255,255,100));
    p.drawText(anchoPantalla / 2 - 10, 50, "-");

    QFont fNom("Bebas Neue", 10); p.setFont(fNom); p.setPen(QColor(200,200,200));
    QString n1 = (personajeSeleccionado >= 0 && personajeSeleccionado < 3) ? nombresPersonaje[personajeSeleccionado] : "J1";
    QString n2 = (personajeIA >= 0 && personajeIA < 3) ? nombresPersonaje[personajeIA] : "IA";
    p.drawText(anchoPantalla / 2 - 85, 22, n1);
    p.drawText(anchoPantalla / 2 + 50, 22, n2);

    QFont fCtrl("Bebas Neue", 11); p.setFont(fCtrl); p.setPen(QColor(255,255,255,150));
    p.drawText(10, altoPantalla - 10, "A/D: mover   W: saltar   S: golpear");
}

void Game_mode1::dibujarTimer(QPainter &p)
{
    QString txt = QString("%1:%2")
    .arg(segundosTotales / 60, 2, 10, QChar('0'))
        .arg(segundosTotales % 60, 2, 10, QChar('0'));

    p.setBrush(QColor(0,0,0,150)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(anchoPantalla - 175, 8, 160, 55, 10, 10);

    if (!spriteClock.isNull()) {
        int aw = spriteClock.width() / 8;
        QPixmap fc = spriteClock.copy(frameAnimacionClock * aw, 0, aw, spriteClock.height());
        int ah = 42, awD = (fc.width() > 0) ? (ah * fc.width() / fc.height()) : 42;
        p.drawPixmap(anchoPantalla - 170, 13, awD, ah, fc);
    } else {
        p.setPen(QPen(QColor(255,200,0), 2)); p.setBrush(Qt::NoBrush);
        p.drawEllipse(anchoPantalla - 168, 13, 36, 36);
        int cx = anchoPantalla - 150, cy = 31;
        p.drawLine(cx, cy, cx, cy - 10); p.drawLine(cx, cy, cx + 7, cy);
    }

    QFont fT("Bebas Neue", 20, QFont::Bold); p.setFont(fT);
    p.setPen((segundosTotales <= 10 && segundosTotales % 2 == 0) ? QColor(255,60,60) : Qt::white);
    p.drawText(anchoPantalla - 120, 50, txt);
}

void Game_mode1::keyPressEvent(QKeyEvent *ev)
{
    int k = ev->key();

    if (estadoActual == EstadoJuego::SeleccionPersonaje) {
        if (k == Qt::Key_Left)  indiceSeleccion = (indiceSeleccion + 2) % 3;
        if (k == Qt::Key_Right) indiceSeleccion = (indiceSeleccion + 1) % 3;
        if (k == Qt::Key_Return || k == Qt::Key_Enter) {
            personajeSeleccionado = indiceSeleccion;
            do { personajeIA = std::rand() % 3; } while (personajeIA == personajeSeleccionado);
            iniciarPartida();
        }
        return;
    }

    if (estadoActual == EstadoJuego::FinDePartida) {
        if (k == Qt::Key_R) iniciarPartida();
        if (k == Qt::Key_Q) reiniciarPartidaCompleta();
        return;
    }

    jugador1->teclaPresionada(k);
}

void Game_mode1::keyReleaseEvent(QKeyEvent *ev)
{
    if (estadoActual == EstadoJuego::Jugando)
        jugador1->teclaSoltada(ev->key());
}