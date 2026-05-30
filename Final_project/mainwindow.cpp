#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <game_mode1.h>
#include <game_mode2.h>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

