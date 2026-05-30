#ifndef GAME_MODE2_H
#define GAME_MODE2_H

#include <QWidget>

class Game_mode2 : public QWidget
{
    Q_OBJECT
public:
    explicit Game_mode2(QWidget *parent = nullptr);

signals:

protected:
//************ESQUELETO*****************************
    void paintEvent(QPaintEvent *event);
//************ESQUELETO*****************************

};

#endif // GAME_MODE2_H
