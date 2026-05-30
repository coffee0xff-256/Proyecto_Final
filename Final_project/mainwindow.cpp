#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <game_mode1.h>
#include <game_mode2.h>
#include <QApplication>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //***********************Start background move******


    //todo esto es para el fondo del menu, para que el tengo como el movimiento de fondo//

    setFixedSize(1536,1024);
    //pongo lo del background en esta parte del constructor (esto lo vi en clase del laboratorio)
    position_x = 0;
 /*
    background = new QLabel(this);
    background->setPixmap(QPixmap("xd.png"));
    background->resize(1080,720);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this, &MainWindow::background_move);
    timer->start(30);
    background->lower();*/
    background1 = new QLabel(this);
    background2 = new QLabel(this);
    QPixmap imagen1("background3.png");
    //escalar la imagen para cuando maximicemos no queden esos huecos blancos
    QPixmap imagen2("background3.png");
    background1->setPixmap(imagen1);
    background2->setPixmap(imagen2);
    background1->resize(1536,1024);
    background2->resize(1536,1024);
    background1->move(0,0);
    background2->move(1536,0);
    background1->lower();
    background2->lower();
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this, &MainWindow::background_move);
    timer->start(16);

//  *******************END Background move******************************

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_game_mode1_clicked()
{
    Game_mode1* game1 = new Game_mode1();
    game1->show();
    hide();
}


void MainWindow::on_button_game_mode2_clicked()
{
    Game_mode2* game2 = new Game_mode2();
    game2->show();
    hide();
}


void MainWindow::on_botton_exit_clicked()
{
    QApplication::quit();
}

void MainWindow::background_move()
{
    /*
    position_x--;
    background->move(position_x,0);
    if(position_x <= -800){position_x = 0;}
*/
    background1->move(background1->x()-1,0);
    background2->move(background2->x()-1,0);
    if(background1->x() <= -1536){background1->move(background2->x()+1536,0);}

    if(background2->x() <= -1536){background2->move(background1->x()+1536,0);}

}




