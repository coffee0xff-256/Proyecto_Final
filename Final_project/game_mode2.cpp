#include "game_mode2.h"
#include <QPainter>

Game_mode2::Game_mode2(QWidget *parent)
    : QWidget{parent}
{
    resize(1080,720);
    setWindowTitle("Game_mode1");

}
//************ESQUELETO*****************************
void Game_mode2::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.fillRect(rect(),Qt::black);
    painter.setPen(Qt::white);
    painter.drawText(450,360,"GAME MODE 2");
//************ESQUELETO*****************************
}