/**
    Yao Xiao
    004171960
    PIC10C Spring 2015
    HW10

    @file mainwindow.cpp
    @Starting page of the game
    @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include "gameboard.h"
#include <QMessageBox>
#include <QCloseEvent>
//#include <QPalette>
//#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    QPalette* palette = new QPalette();
    QPixmap* pixmap = new QPixmap(":/image/background_image.jpg");
    pixmap->scaledToHeight(500);
    pixmap->scaledToWidth(500);
    palette->setBrush(QPalette::Background,*(new QBrush(*(pixmap))));
    //palette->setBrush(QPalette::Background,*(new QBrush(*(new QPixmap(":/image/background_image.jpg")))));
    setPalette(*palette);
*/
}

/** Define the function to start a game for beginner
    @start the game once the QPushbottum is clicked
    @smallest board size
    @shortest time
    @Start with level 1
*/
void MainWindow::start_beginner(){
    /** create the gameboard to start
     *  @this should start with level 1..to be continued
    */
    board = new GameBoard(this, number_range, 1, 5, 4, 0, 20);
    this->setCentralWidget(board);
}

/**
    Restart the game
    @Same difficulty, range, level, etc.
*/
void MainWindow::restart(int levelNum, size_t boardSize, unsigned int live_remain, size_t mins, size_t secs){
    delete board;
    board = new GameBoard(this, number_range, levelNum, boardSize, live_remain, mins, secs);
    this->setCentralWidget(board);
}

/**
    Go to next level of difficulty beginner
    @Delete original gameboard
    @Create a more complicated gameboard
*/
void MainWindow::next_beginner(){
    delete board;
    board = new GameBoard(this, number_range, 2, 6, 4, 0, 30);
    this->setCentralWidget(board);
}

/** Define the function to start a game for standard
    @start the game once the QPushbottum is clicked
    @median board size
    @longer time between beginner and premium
    @Start with level 1
*/
void MainWindow::start_standard(){
    /** create the gameboard to start
     *  @this should start with level 1..to be continued
    */
    board = new GameBoard(this, number_range, 1, 7, 4, 0, 40);
    this->setCentralWidget(board);
}

/**
    Go to next level of difficulty standard
    @Delete original gameboard
    @Create a more complicated gameboard
*/
void MainWindow::next_standard(){
    delete board;
    board = new GameBoard(this, number_range, 2, 8, 4, 0, 50);
    this->setCentralWidget(board);
}

/** Define the function to start a game for premium
    @start the game once the QPushbottum is clicked
    @largest board size
    @longest time considered the size
    @Start with level 1
*/
void MainWindow::start_premium(){
    /** create the gamebaord to start
     *  @this should start with level 1..to be continued
    */
    board = new GameBoard(this, number_range, 1, 10, 4, 1, 0);
    this->setCentralWidget(board);
}

/**
    Go to next level of difficulty premium
    @Delete original gameboard
    @Create a more complicated gameboard
*/
void MainWindow::next_premium(){
    delete board;
    board = new GameBoard(this, number_range, 2, 11, 4, 1, 10);
    this->setCentralWidget(board);
}

/** Define the function to update the number range
    @update the number range once the radiobuttom has been clicked
*/
void MainWindow::update_type_10(){
    /** set the variable type_number and pass it to create the gamebaord*/
    number_range = 10;
}

/** Define the function to update the number range
    @update the number range once the radiobuttom has been clicked
*/
void MainWindow::update_type_100(){
    /** set the variable type_number and pass it to create the gamebaord*/
    number_range = 100;
}

/** Define the function to update the number range
    @update the number range once the radiobuttom has been clicked
*/
void MainWindow::update_type_1000(){
    /** Sets the variable type_number and pass it to create the gameboard*/
    number_range = 1000;
}

/** Any time user wants to quit the game, the function gets called
    @little quit dialog
    @yes -> exit
    @no -> ignore
*/
void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton quitBtn = QMessageBox::question(this, "Exit", tr("Are you sure to exit?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::Yes);

    if (quitBtn != QMessageBox::Yes) {
        event->ignore();
    }
    else{
        event->accept();
    }
}

/** When the game ends, the function gets called */
void MainWindow::game_over(){

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr); // delete the widget gracefully without memory leak

    /** Display somthing which says "Game Over!"
     *  @using message box like quit dialog
    */
    QMessageBox::StandardButton lostBtn = QMessageBox::information(this, "Game Over", tr("You lost the game!"));

    delete board;

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
}

/**
    Instruction buttom
    @Pop up another window to show the instruction of the game
*/
void MainWindow::instructionWindow(){

    instruction = new QWidget;
    instruction->setWindowTitle("Instruction");
    QVBoxLayout* instructionLayout = new QVBoxLayout;

    QLabel* instr = new QLabel("You are trying to match two same numbers on the gameboard. \n"
                               "You can move the picture to the location via pressing arrow buttoms on the keyboard.\n"
                               "You can press the space key to see the hidden number. There are at most two numbers will be \n"
                               "visible at the same time, except that the two match and they will stay visible all the time.\n"
                               "When you match all the numbers, you will be proccessed to next level.\n"
                               "There is a countdown timer. Every time you do not finish before it goes to 0, you lose a live\n"
                               "and the game will restart with the same level.\n");
    QPushButton* closeInstr = new QPushButton("Done");
    instructionLayout->addWidget(instr);
    instructionLayout->addWidget(closeInstr);
    instruction->setLayout(instructionLayout);
    instruction->show();
    QObject::connect(closeInstr, SIGNAL(clicked()), instruction, SLOT(hide()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**void MainWindow::nextLevelInvite(){
    QMessageBox::StandardButton quitBtn = QMessageBox::question(this, "Well done!", tr("The level is completed, do you want to process to next level?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes);

    if (quitBtn != QMessageBox::Yes) {
        this->start_next level;
    }
    else{
        event->accept();
    }
}*/
