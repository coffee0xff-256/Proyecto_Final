#include "game_mode2.h"
#include <QPainter>
#include <QColor>
#include <QPen>
#include <algorithm>
#include <QTime>

#include <QMediaPlayer>
#include <QAudioOutput>

Game_mode2::Game_mode2(QWidget *parent)
    : QWidget{parent}
{
    //musiquita
    audioOutput = new QAudioOutput(this);
    music = new QMediaPlayer(this);

    music->setAudioOutput(audioOutput);

    music->setSource(QUrl("qrc:/musica/sounds_gm2/music.mp3"));

    audioOutput->setVolume(0.3);

    music->play();

    gunAudio = new QAudioOutput(this);
    gunSound = new QMediaPlayer(this);

    gunSound->setAudioOutput(gunAudio);

    gunSound->setSource(QUrl("qrc:/musica/sounds_gm2/shoot.mp3"));

    //fijamos los pixeles de la pantalla
    setFixedSize(1536, 1024);
    setWindowTitle("Game_mode2");

    //deteccion de eventos del teclado
    setFocusPolicy(Qt::StrongFocus);

    srand(time(NULL));

    // carga de los enemigos del vector

    spritesDisponibles.push_back(QPixmap(":/sprites_gm2/xd2.png"));
    spritesDisponibles.push_back(QPixmap(":/sprites_gm2/eskeletin.png"));
    spritesDisponibles.push_back(QPixmap(":/sprites_gm2/spidercita.png"));
    spritesDisponibles.push_back(QPixmap(":/sprites_gm2/brujito.png"));

    //los sprites que utilice


    pistolasprite.load(":/sprites_gm2/pistola2.png");
    texturawall.load(":/textures_gm2/wall1.png");
    heaven.load(":/textures_gm2/heaven2.png");
    //Floor.load("floor.png");
    shoot.load(":/sprites_gm2/shoot3.png");
    center_shot.load(":/sprites_gm2/center_shot.png");

    cargarNivel();

    timerJuego = new QTimer(this);

    // aqui conectamos la señal que nos saca el .h para el tiempo de ejecución del videojuego
    connect(timerJuego, &QTimer::timeout, this, &Game_mode2::bucleJuego);

    // aqui inicializamos en 16 ms approx 60 frames en qt
    timerJuego->start(16);
}

void Game_mode2::cargarNivel() {
    listaEnemigos.clear();
    int cantidadEnemigos = 0;
    double velocidadBase = 0;

    if (nivelActual == 1) {
        cantidadEnemigos = 5;
        velocidadBase = 0.005;
    }
    else if (nivelActual == 2) {
        cantidadEnemigos = 10;
        velocidadBase = 0.008; // Ligeramente más rápidos
    }
    else if (nivelActual >= 3) {
        cantidadEnemigos = 15;
        velocidadBase = 0.012; // Un poco más desafiantes
    }

    while (listaEnemigos.size() < cantidadEnemigos) {

        // como tenemos un mapa de 16x16 tenemos 16 pixeles en ambos ejes
        int posX = rand() % 16;
        int posY = rand() % 16;

        if (map1[posY][posX] == 0 && (posX != (int)playerx || posY != (int)playery)) {

            double velVariada = velocidadBase + ((rand() % 5) / 1000.0);

            QPixmap spriteAleatorio = spritesDisponibles[rand() % spritesDisponibles.size()];

            Enemigo nuevoEnemigo(posX + 0.5, posY + 0.5, velVariada, spriteAleatorio);
            listaEnemigos.push_back(nuevoEnemigo);
        }
    }
}

bool Game_mode2::colisionBalaPared(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double distancia = sqrt(dx * dx + dy * dy);

    int pasos = distancia * 20;
    double pasoX = dx / pasos;
    double pasoY = dy / pasos;

    double rayX = x1;
    double rayY = y1;

    for (int i = 0; i < pasos; i++) {
        rayX += pasoX;
        rayY += pasoY;
        if (map1[(int)rayY][(int)rayX] == 1) {
            return true;
        }
    }
    return false;
}

void Game_mode2::bucleJuego() {
    if (vida <= 0) return;

    bool todosMuertos = true;

    for (int i = 0; i < listaEnemigos.size(); i++) {
        if (!listaEnemigos[i].vivo) continue;

        todosMuertos = false;

        double dx = playerx - listaEnemigos[i].x;
        double dy = playery - listaEnemigos[i].y;
        listaEnemigos[i].distanciaAlJugador = sqrt(dx * dx + dy * dy);

        if (listaEnemigos[i].distanciaAlJugador > 0.3) {
            double moveX = (dx / listaEnemigos[i].distanciaAlJugador) * listaEnemigos[i].velocidad;
            double moveY = (dy / listaEnemigos[i].distanciaAlJugador) * listaEnemigos[i].velocidad;

            double nextX = listaEnemigos[i].x + moveX;
            double nextY = listaEnemigos[i].y + moveY;

            if(map1[(int)nextY][(int)nextX] == 0) {
                listaEnemigos[i].x = nextX;
                listaEnemigos[i].y = nextY;
            } else {
                if(map1[(int)listaEnemigos[i].y][(int)nextX] == 0) listaEnemigos[i].x = nextX;
                if(map1[(int)nextY][(int)listaEnemigos[i].x] == 0) listaEnemigos[i].y = nextY;
            }
        } else {
            if(rand() % 6 == 0) vida -= 1;
        }
    }

    if (todosMuertos) {
        nivelActual++;
        cargarNivel();
    }

    if (tiemposhoot > 0) tiemposhoot--;
    else disparando = false;

    update();
}

void Game_mode2::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (vida <= 0) {
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 60, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
        return;
    }

    painter.setPen(Qt::white);
    painter.drawText(450, 360, "GAME MODE 2");

    // el tamaña de los pixeles nos interesa, para dividir la pantalla
    int tilesize = 25;
    for (int fila = 0; fila < 16; fila++) {
        for (int columna = 0; columna < 16; columna++) {
            if (map1[fila][columna] == 1) {
                painter.fillRect(columna * tilesize, fila * tilesize, tilesize, tilesize, Qt::white);
            }
            painter.setPen(QPen(Qt::green, 1));
            painter.drawRect(columna * tilesize, fila * tilesize, tilesize, tilesize);
        }
    }

    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(playerx * tilesize - 5, playery * tilesize - 5, 10, 10);

    // empezamos hacer los truquitos del falso 3d
    int viewportX = 400;
    int viewportWidth = width() - viewportX;
    double rayWidth = (double)viewportWidth / numrays;

    painter.fillRect(viewportX, 0, viewportWidth, height() / 2, heaven);
    painter.fillRect(viewportX, height() / 2, viewportWidth, height() / 2, Qt::darkGray);

    for (int i = 0; i < numrays; i++) {
        double rayangle = angle - fov / 2 + (double)i / numrays * fov;
        double rayx = playerx;
        double rayy = playery;
        double paso = 0.02;

        while (true) {
            rayx += cos(rayangle) * paso;
            rayy += sin(rayangle) * paso;


            // condicion para que el rayo no se salga

            if (rayx < 0 || rayx >= 16 || rayy < 0 || rayy >= 16) break;
            if (map1[(int)rayy][(int)rayx] == 1) break;
        }

        double distancia = sqrt((rayx - playerx) * (rayx - playerx) + (rayy - playery) * (rayy - playery));
        distancia *= cos(rayangle - angle);

        double hitx = rayx - floor(rayx);
        double hity = rayy - floor(rayy);
        double texcoordenada = (hitx < 0.01 || hitx > 0.99) ? hity : hitx;
        int texturex = texcoordenada * texturawall.width();

        zbuffer[i] = distancia;

        int wallaltura = 1080 / distancia;
        double screenx = viewportX + i * rayWidth;
        int wallTop = height() / 2 - wallaltura / 2;

        painter.drawPixmap(QRect(screenx,wallTop,rayWidth +1,wallaltura),texturawall,QRect(texturex,0,1,texturawall.height()));
    }

    //apartado de los enemigos
    std::vector<Enemigo*> enemigosRender;
    for (int i = 0; i < listaEnemigos.size(); i++) {
        if (listaEnemigos[i].vivo) enemigosRender.push_back(&listaEnemigos[i]);
    }

    std::sort(enemigosRender.begin(), enemigosRender.end(), [](Enemigo* a, Enemigo* b) {
        return a->distanciaAlJugador > b->distanciaAlJugador;
    });

    for (Enemigo* e : enemigosRender) {
        painter.setBrush(Qt::blue);
        painter.drawEllipse(e->x * tilesize - 3, e->y * tilesize - 3, 6, 6);

        double dx2 = e->x - playerx;
        double dy2 = e->y - playery;
        double enemyangle = atan2(dy2, dx2);
        double angleDiferencia = enemyangle - angle;

        while (angleDiferencia > M_PI) angleDiferencia -= 2 * M_PI;
        while (angleDiferencia < -M_PI) angleDiferencia += 2 * M_PI;

        if (fabs(angleDiferencia) < fov / 2) {
            double rayIndexFloat = (angleDiferencia + fov / 2) / fov * numrays;
            int rayindex = (int)rayIndexFloat;

            if (rayindex >= 0 && rayindex < numrays) {
                if (e->distanciaAlJugador < zbuffer[rayindex]) {
                    double screenx = viewportX + rayIndexFloat * rayWidth;
                    int spriteHeight = 400 / e->distanciaAlJugador;
                    int spriteWeight = spriteHeight / 2;

                    painter.drawPixmap(screenx - spriteWeight / 2, height() / 2 - spriteHeight / 2,
                                       spriteWeight, spriteHeight, e->sprite);
                }
            }
        }
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    painter.drawText(500, 30, QString("Vida: %1 | Nivel: %2").arg(vida).arg(nivelActual));

    int centerx = viewportX + viewportWidth / 2;
    int centery = height() / 2;
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(centerx - 10, centery, centerx + 10, centery);
    painter.drawLine(centerx, centery - 10, centerx, centery + 10);

    int gunWidth = 400;
    int gunHeight = 300;
    if(disparando){
        painter.drawPixmap(viewportX+viewportWidth/2 - gunWidth/2, height()-gunHeight, gunWidth, gunHeight, shoot);
        painter.drawPixmap(centerx - 25, centery - 25, 50, 50, center_shot);
    } else {
        painter.drawPixmap(viewportX + viewportWidth/2 - gunWidth/2, height() - gunHeight, gunWidth, gunHeight, pistolasprite);
    }
}

void Game_mode2::keyPressEvent(QKeyEvent *event) {
    if (vida <= 0) return;

    // mis variables que puedo mejorar
    double velocidad = 0.15;
    double rotacion = 0.1;

    switch (event->key()) {
    case Qt::Key_W: {
        double newx = playerx + cos(angle) * velocidad;
        double newy = playery + sin(angle) * velocidad;
        if (map1[(int)newy][(int)newx] == 0) { playerx = newx; playery = newy; }
        break;
    }
    case Qt::Key_S: {
        double newx = playerx - cos(angle) * velocidad;
        double newy = playery - sin(angle) * velocidad;
        if (map1[(int)newy][(int)newx] == 0) { playerx = newx; playery = newy; }
        break;
    }
    case Qt::Key_Left:
        angle -= rotacion;
        break;
    case Qt::Key_Right:
        angle += rotacion;
        break;
    case Qt::Key_Space: {

        //sonidito
        gunSound->setPosition(0);
        gunSound->play();
        if (!disparando) {
            disparando = true;
            tiemposhoot = 8;

            for (int i = 0; i < listaEnemigos.size(); i++) {
                if (!listaEnemigos[i].vivo) continue;

                double dx = listaEnemigos[i].x - playerx;
                double dy = listaEnemigos[i].y - playery;
                double dirx = dx / listaEnemigos[i].distanciaAlJugador;
                double diry = dy / listaEnemigos[i].distanciaAlJugador;
                double viewx = cos(angle);
                double viewy = sin(angle);

                double dot = dirx * viewx + diry * viewy;

                if (dot > 0.95 && listaEnemigos[i].distanciaAlJugador < 6.0) {
                    if (!colisionBalaPared(playerx, playery, listaEnemigos[i].x, listaEnemigos[i].y)) {
                        listaEnemigos[i].vivo = false;
                        break;
                    }
                }
            }
        }
        break;
    }
    }
}