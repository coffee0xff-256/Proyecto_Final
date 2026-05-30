#ifndef GAME_MODE2_H
#define GAME_MODE2_H

#include <QWidget>

class Game_mode2 : public QWidget
{
    Q_OBJECT
public:
    explicit Game_mode2(QWidget *parent = nullptr);

private:
    //***********************Mapa vista cenital************************

    int map1[8][8]={
                      {1,1,1,1,1,1,1,1},
                      {1,0,0,0,0,0,0,1},
                      {1,0,0,0,0,0,0,1},
                      {1,0,0,1,1,0,0,1},
                      {1,0,0,1,1,0,0,1},
                      {1,0,0,0,0,0,0,1},
                      {1,0,0,0,0,0,0,1},
                      {1,1,1,1,1,1,1,1}};

    double playerx = 2.5;
    double playery = 2.5;

 //***********************Mapa vista cenital************************

signals:

protected:
//************ESQUELETO*****************************
    void paintEvent(QPaintEvent *event);
//************ESQUELETO*****************************



};

#endif // GAME_MODE2_H
