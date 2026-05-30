#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_button_game_mode1_clicked();

    void on_button_game_mode2_clicked();

    void on_botton_exit_clicked();

    //aqui viene el slot para lo del movimiento del background emma
    void background_move();


    void on_label_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    //atributos para lo del background emma del menu
    QLabel *background;
    QTimer *timer;
    int position_x;
    // end menu //
    //mejoramiento del menu//
    QLabel* background1;
    QLabel* background2;


};
#endif // MAINWINDOW_H
