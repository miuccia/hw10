/**
  Yao Xiao
  004171960
  PIC10C Spring 2015
  HW10

  @file mainwindow.h
  @Contain all declarations that matter to mainwindow.cpp
  @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include <QString>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

/**
  all needed slots
  @Three slots to begin the game for different kinds
  @Two slots to update the
*/
public slots:
    /** game starts*/
    void start_beginner();
    void start_standard();
    void start_premium();

    /** update number ranges*/
    void update_type_10();
    void update_type_100();
    void update_type_1000();

    /** closeEvent function*/
    void closeEvent(QCloseEvent *event);

    /** game over function*/
    void game_over();

    /** Show instruction*/
    void instructionWindow();

    void restart(int levelNum, size_t boardSize, unsigned int live_remain, size_t mins, size_t secs);

    void next_beginner();
    void next_standard();
    void next_premium();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /** variables to identify differences between games */
    GameBoard* board; // to show the gameboard in the mainwindow
    int number_range; // passed in the constructor to help updating number type information in the gameboard

    /** variable for instruction window*/
    QWidget* instruction;
};

#endif // MAINWINDOW_H
