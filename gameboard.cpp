/**
    Yao Xiao
    004171960
    PIC10C Spring 2015
    HW10

    @file gameboard.cpp
    @Include everything necessary to construct a gameboard
    @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
*/

#include "gameboard.h"
#include "ui_gameboard.h"
#include "mainwindow.h"
#include "countdowntimer.h"
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QTime>
#include <QLCDNumber>
#include <QtAlgorithms>
#include <QBrush>
#include <QImage>
#include <QPalette>
#include <QGraphicsView>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

GameBoard::GameBoard(QWidget *parent, int number_range, int level_number, size_t board_size, unsigned int lives_remain, size_t minute, size_t second) :
    QWidget(parent),
    ui(new Ui::GameBoard), num_range(QString::number(number_range)), board_size(board_size),
    lives_remaining(lives_remain), min(minute), sec(second)
{
    ui->setupUi(this);

    win = false;

    Top = new QWidget;
    //Top->setStyleSheet("QLabel { background-color : pink; color : white}");
    /** sets up the Top bar
        @Image     Number range: ##    Level:#
        @horizontal boxlayout spaces out the elements above
    */
    QVBoxLayout *entire_bar = new QVBoxLayout(Top);
    QHBoxLayout *top_bar = new QHBoxLayout();

    /** load up the image of the user
        @copy file path from resource file
    */
    QString fileName(":/image/user_image.jpeg");
    user_image = new QPixmap(fileName);

    /** fix a total of 4 lives
        @one life is currently playing, the others are listed above
        @lives is an array of QLabel POINTERS
        @one horizontal layout box to contain user_image
    */
    //lives_remaining = 4;
    lives = new QLabel*[lives_remaining-1];
    QHBoxLayout *living = new QHBoxLayout;
    living->setSpacing(3);

    /** set the labels to have the user's image, add to the top bar layout*/
    for(size_t i=0; i<lives_remaining-1; i++){
        lives[i] = new QLabel;
        lives[i]->setPixmap(*user_image);
        lives[i]->setMinimumSize(40,40);
        lives[i]->setMaximumSize(40,40);
        lives[i]->setScaledContents(true);
        living->addWidget(lives[i]);
    }

    top_bar->addLayout(living);

    /** add in some horizontal space before next labels*/
    QSpacerItem* horizontal_space = new QSpacerItem(10,10);
    top_bar->addSpacerItem(horizontal_space);

    /** Image     Number range:##  */
    QLabel* game_type_text = new QLabel("Number range: ");
    if(num_range == "10") game_type_value = new QLabel("1 to 10 ");
    if(num_range == "100") game_type_value = new QLabel("1 to 100 ");
    if(num_range == "1000") game_type_value = new QLabel("1 to 1000 ");

    game_type_text->setStyleSheet("QLabel { background-color : pink; color : white}");
    game_type_value->setStyleSheet("QLabel { background-color : pink; color : white}");

    top_bar->setSpacing(0);
    top_bar->addWidget(game_type_text);
    top_bar->addWidget(game_type_value);

    /** Image      Number range:##   Level: #*/
    QLabel* level_text = new QLabel("Level: ");
    level_num = level_number;
    level_value = new QLabel(QString::number(level_num));

    level_text->setStyleSheet("QLabel { background-color : pink; color : white}");
    level_value->setStyleSheet("QLabel { background-color : pink; color : white}");

    QSpacerItem* horizontal_space2 = new QSpacerItem(*horizontal_space);

    /** add in some horizontal space before next labels*/
    top_bar->addSpacerItem(horizontal_space2);
    top_bar->addWidget(level_text);
    top_bar->addWidget(level_value);


    /** Add in a instruction line to the window*/
    QHBoxLayout* second_bar = new QHBoxLayout;
    QLabel* instrWord = new QLabel("Go to see instruction: ");
    QPushButton* instrBtn = new QPushButton("Instruction");
    QObject::connect(instrBtn, SIGNAL(clicked()), parent, SLOT(instructionWindow()));

    instrWord->setStyleSheet("QLabel { background-color : pink; color : white}");
    instrBtn->setStyleSheet("QLabel { background-color : pink; color : white}");

    second_bar->addWidget(instrWord);
    second_bar->addWidget(instrBtn);

    /** Add in a countdown timer here*/
    QHBoxLayout* timer_bar = new QHBoxLayout;
    QLabel* timer_word = new QLabel("Time Remaining: ");
    timer_word->setMinimumSize(110,30);
    timer_word->setMaximumSize(110,30);
    QWidget* clock = new QWidget;
    clock->resize(110,30);

    clock->setStyleSheet("QLabel { background-color : pink; color : white}");

    clocking = new CountDownTimer(clock, min, sec);
    clocking->setFixedSize(100, 29);

    timer_word->setStyleSheet("QLabel { background-color : pink; color : white}");

    timer_bar->addWidget(timer_word);
    timer_bar->addWidget(clock);


    local_timer = new QTimer(this);
    connect(local_timer, SIGNAL(timeout()), this, SLOT(TimeOut()));


    clocking->timer->start(1000);
    local_timer->start((min*60+sec)*1000);


    entire_bar->addLayout(top_bar);
    entire_bar->addLayout(second_bar);
    entire_bar->addLayout(timer_bar);


    // creates the Board
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size];
    //values_10 = new int[board_size*board_size];
    //values_100 = new int[board_size*board_size];
    //values_1000 = new int[board_size*board_size];
    values = new int[board_size*board_size];
    if_revealed = new bool[board_size*board_size];
    if_bingo = new bool[board_size*board_size];
    QGridLayout *SquareGrid = new QGridLayout(Board);

    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);


    for(size_t i=0; i<board_size; i++){
        for(size_t j=0; j<board_size; j++){

            /** Generate random values
                @depends on what kind of number range user chooses, use different value arrays
                @below is for range "Integer"
            */
            if(num_range == "10"){
                // Random values
                std::uniform_int_distribution<int> distribution(1,10);
                //values_10[i*board_size+j] = distribution(generator);
                values[i*board_size+j] = distribution(generator);


                // Create label and set properties.
                //labels[i*board_size+j] = new QLabel;
                //labels[i*board_size+j]->setText(QString::number( values_10[i*board_size+j]));
            }

            /** Below is for range "100"*/
            if(num_range == "100"){
                // Random values
                std::uniform_int_distribution<int> distribution(1,100);
                //values_100[i*board_size+j] = distribution(generator);
                values[i*board_size+j] = distribution(generator);

                // Create label and set properties.
                //labels[i*board_size+j] = new QLabel;
                //labels[i*board_size+j]->setText(QString::number(values_100[i*board_size+j]));
            }

            /** Below is for version "100"*/
            if(num_range == "1000"){
                // Random values
                std::uniform_int_distribution<int> distribution(1,1000);
                //values_1000[i*board_size+j] = distribution(generator);
                values[i*board_size + j] = distribution(generator);

                // Creator label and set properties
                //labels[i*board_size+j] = new QLabel;
                //labels[i*board_size+j]->setText(QString::number(values_1000[i*board_size+j]));
            }

            labels[i*board_size+j] = new QLabel;
            labels[i*board_size+j]->setText(QString::number(values[i*board_size+j]));

            /** This two lines fix the shape of each QLabel*/
            labels[i*board_size+j]->setMinimumSize(30,30);
            labels[i*board_size+j]->setMaximumSize(30,30);

            /** The line below set the background color and text color of each QLabel
             *  @The original status background and text should share the same color
             *  @Later when one is chosen and space pressed, text color changes
            */
            labels[i*board_size+j]->setStyleSheet("QLabel { background-color : pink; color : pink; }");

            /** Other two properties*/
            labels[i*board_size+j]->setFrameStyle(1);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);

            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);

        }
    }

    // Initialize the user at the top left corner, coordinate (0,0)
    user_position = new QPoint(0,0);
    labels[0]->setPixmap(*user_image);
    labels[0]->setScaledContents(true);

    Board->setFixedSize(400,400);

    /** Now piece everthing together
     *  @QWidget* CoreWidget = this;
     *  @Central Widget composed of Top and Board
     *  @   Top
     *   --------
     *     Board
     *  @Do not allow board to be resized, otherwise spacings get messed up
    */
    setFixedSize(800,500);


    QPushButton* quit = new QPushButton("Quit");
    QObject::connect(quit, SIGNAL(clicked()), parent, SLOT(game_over()));

    entire_bar->addWidget(quit);

    // Create a vertical box layout for the two spieces
    QHBoxLayout *piece_together = new QHBoxLayout;

    piece_together->addWidget(Top,0,Qt::AlignCenter);
    piece_together->addWidget(Board,0,Qt::AlignCenter);

    //piece_together->addWidget(quit);

    this->setLayout(piece_together);

    connect(this, SIGNAL(game_over()), parent, SLOT(game_over()));
    connect(this, SIGNAL(restart(int,size_t,uint,size_t,size_t)), parent, SLOT(restart(int, size_t, uint, size_t, size_t)));
    connect(this, SIGNAL(next_beginner()), parent, SLOT(next_beginner()));
    connect(this, SIGNAL(next_standard()), parent, SLOT(next_standard()));
    connect(this, SIGNAL(next_premium()), parent, SLOT(next_premium()));
}


GameBoard::~GameBoard()
{
    //delete [] values_10;
    //delete [] values_100;
    //delete [] values_1000;
    delete [] values;
    delete [] user_position;
    delete [] if_revealed;
    delete [] if_bingo;
    delete clocking;
    delete local_timer;

    delete ui;
}

void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = user_position->rx();
    int y = user_position->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        if(user_position->rx() != 0)
            updateUser(x,y,x-1,y);
        break;

    case Qt::Key_Right:
        if(user_position->rx() != board_size-1)
            updateUser(x,y,x+1,y);
        break;

    case Qt::Key_Up:
        if(user_position->ry() != 0)
            updateUser(x,y,x,y-1);
        break;

    case Qt::Key_Down:
        if(user_position->ry() != board_size-1)
            updateUser(x,y,x,y+1);
        break;

        /** Once the space key pressed, the text color changes and reveals the number
            @revealNumber(x,y) will change the text color of the QLabel at (x,y)
        */
    case Qt::Key_Space:

        /** If the position's if_bingo is true, the action does not count*/
        if(!if_bingo[y*board_size+x]){
            revealNumber(x,y);
            set_revealed(x,y);
            revealed_times+=1;
        }

        break;
    default:
        QWidget::keyPressEvent(event);
    }

    /** Need both lines to force a repaint
     *  @First line forces processer to process all previously promised events
     *  Second line queues up a repaint
    */
    QCoreApplication::processEvents();
    repaint();

    return;
}

/** Reprint user's image every time after moving a new position*/
void GameBoard::paintEvent(QPaintEvent *e){

    int x = user_position->rx();
    int y = user_position->ry();

    /** When the user moves, paint the user_image at the new place*/
    labels[y*board_size+x]->setPixmap(*user_image);
    labels[y*board_size+x]->setScaledContents(true);

}

void GameBoard::showEvent(QShowEvent *e){
    /** */
    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

void GameBoard::updateUser(int px, int py, int nx, int ny){
    labels[py*board_size+px]->clear();
    /*if(num_range == "10"){
        if(values_10[py*board_size+px])
            labels[py*board_size+px]->setText(QString::number( values_10[py*board_size+px]));
    }
    if(num_range == "100"){
        if(values_100[py*board_size+px])
            labels[py*board_size+px]->setText(QString::number( values_100[py*board_size+px]));
    }
    if(num_range == "1000"){
        if(values_1000[py*board_size+px])
            labels[py*board_size+px]->setText(QString::number( values_1000[py*board_size+px]));
    }*/

    if(values[py*board_size+px])
        labels[py*board_size+px]->setText(QString::number( values[py*board_size+px]));

    user_position->setX(nx);
    user_position->setY(ny);
}

/** It should reveal the number when space key pressed
 *  @It should test how many numbers have been revealed already
 *  @If the number of revealed ones has been 2, there come two different situations
*/
void GameBoard::revealNumber(int x, int y){

    /** Below is the occasion that there are already 2 numbers revealed
     *  @Set the revealed_times to 0
     *  @Reverse if_revealed status back to false
     *  @Depending on bool_bingo whether to hide the 2 numbers
    */
    if(revealed_times == 2){
        revealed_times = 0;

        /** Use for loop to run over bool array
         *  @Locate the two elements of if_revealed array that are true
         *  @Reset the if_revealed element to be false
        */
        for(size_t i=0; i<board_size*board_size; i++){
            for(size_t j=i+1; j<board_size*board_size; j++){ // j starts from the element after i prevents to bingo single entry
                if(if_revealed[i] == true && if_revealed[j] == true){

                    /** When one of the 2 chosen numbers has already in the if_bingo true*/
                    if(if_bingo[i] == true){
                        if(!if_bingo[j] == false)
                            labels[j]->setStyleSheet("QLabel { background-color : pink; color : pink}");
                    }
                    else if(if_bingo[j] == true){
                        if(!if_bingo[i] == false)
                            labels[i]->setStyleSheet("QLabel { background-color : pink; color : pink}");
                    }

                    /** When they DO NOT share the same number*/
                    else if(!bool_bingo(i,j)){
                        labels[i]->setStyleSheet("QLabel { background-color : pink; color : pink}");

                        labels[j]->setStyleSheet("QLabel { background-color : pink; color : pink}");
                    }

                    /** Reverse the if_revealed status to false*/
                    if_revealed[i] = false;
                    if_revealed[j] = false;
                }
            }
        }
    }

    /** After deal with already revealed numbers
     *  Reveal the current chosen QLabel*/
    labels[y*board_size+x]->setStyleSheet("QLabel { background-color : pink; color : white}");

}

/** It sets the bool to be true*/
void GameBoard::set_revealed(int x, int y){
    if_revealed[y*board_size+x] = true;
}

/** This should compare the two values for different num_range*/
bool GameBoard::bool_bingo(int i, int j){
    /** This is for range under 10
     *  @Set the corresponding if_bingo element to be true
     *  @funcion returns true
    */
    /*if(num_range == "10"){
        if(values_10[i] == values_10[j]){
            if_bingo[i] = true;
            if_bingo[j] = true;
            return true;
        }
    }*/

    /** This is for range under 100*/
    /*if(num_range == "100"){
        if(values_100[i] == values_100[j]){
            if_bingo[i] = true;
            if_bingo[j] = true;
            return true;
        }
    }*/

    /** This is for range under 1000*/
    /*if(num_range == "1000"){
        if(values_1000[i] == values_1000[j]){
            if_bingo[i] = true;
            if_bingo[j] = true;
            return true;
        }
    }*/

    if(values[i] == values[j]){
        if_bingo[i] = true;
        if_bingo[j] = true;
        return true;
    }

    /** Function returns false if none of the conditions above fit
     *  @It means the two elements DO NOT share same number
    */
    return false;
}

/** When running out of time*/
void GameBoard::TimeOut(){

    /** See if still lives remain
     *  @yes->lessen one live, restart the game with same level
     *  @When level is larger than 1, shuffle the numbers on the GameBoard every time losing a life
    */
    if(lives_remaining > 1){
        lives_remaining--;
        //lives[lives_remaining-1]->clear();

        // If user wins the game
        // Go to next level
        if(win){
            if(board_size == 5)
                this->next_beginner();
            if(board_size == 7)
                this->next_standard();
            if(board_size == 10)
                this->next_premium();
        }

        else{
            // Restart the game
            this->restart(level_num, board_size, lives_remaining, min, sec);
        }


       /*for(size_t i=0; i<board_size*board_size; i++){
            if(if_revealed[i]){
                labels[i]->setStyleSheet("QLabel { background-color : pink; color : pink}");
                if_revealed[i] = false;
            }
            if(if_bingo[i]){
                labels[i]->setStyleSheet("QLabel { background-color : pink; color : pink}");
                if_bingo[i] = false;
            }
        }*/

    }

    else{
        this->game_over();
        // Game Over
    }


}

/** The level completes if it bools true
 *  @It bools true if there are no two same numbers to match any more
*/
void GameBoard::setWin(){
    /** Check if there are still more than two same numbers
     *  @if can still find two same numbers, returns false
     *  @otherwise, return true, the level completed
    */
    for(size_t i=0; i<board_size*board_size; i++){
        for(size_t j = i+1; j<board_size*board_size; j++){
            if(!if_bingo[i] && !if_bingo[j]){
                if(labels[i] == labels[j])
                    return;
            }
        }
    }

    win = true;
}
