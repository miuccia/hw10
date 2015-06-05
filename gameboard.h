/**
 * Yao Xiao
   004171960
   PIC10C Spring 2015
   HW10

    @file gameboard.h
    @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <chrono>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QKeyEvent>
#include "countdowntimer.h"
//#include <QGraphicsScene>
//#include <QGraphicsView>

#include <iostream>

namespace Ui {
class GameBoard;
}

class GameBoard : public QWidget
{
    Q_OBJECT

public slots:
    void TimeOut();

signals:
    void game_over();
    void restart(int levelNum, size_t boardSize, unsigned int live_remain, size_t mins, size_t secs);
    void next_beginner();
    void next_standard();
    void next_premium();

public:
    explicit GameBoard(QWidget *parent = 0, int num_range = 10, int level_number = 1, size_t board_size = 10,
                       unsigned int lives_remain = 4, size_t minute = 0, size_t second = 20);
    ~GameBoard();

    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

    void updateUser(int px, int py, int nx, int ny);

    /** Functions to reveal numbers
     *  @First one actually reveals the number
     *  @Second one sets the revealed bool variable to true
     *  @Third one tells if to hide the two numbers
    */
    void revealNumber(int x, int y);
    void set_revealed(int x, int y);
    bool bool_bingo(int i, int j);

    void setWin();


private:
    Ui::GameBoard *ui;

    //user variables
    const QPixmap* user_image;
    QPoint *user_position;

    // Board Variables
    QWidget *Board;
    QString num_range;
    size_t board_size;
    QLabel** labels;
    int* values;
    //int* values_10;
    //int* values_100;
    //int* values_1000;
    int revealed_times;
    bool* if_revealed;
    bool* if_bingo;
    bool win;


    // Top bar variables
    QLabel* game_type_value;
    QLabel* level_value;
    int level_num;
    QWidget* Top;
    unsigned int lives_remaining;
    QLabel** lives;

    // Timer Variables
    size_t min;
    size_t sec;
    CountDownTimer* clocking;
    QTimer *local_timer;

};

#endif // GAMEBOARD_H
