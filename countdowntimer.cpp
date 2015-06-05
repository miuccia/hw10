#include "countdowntimer.h"

CountDownTimer::CountDownTimer(QWidget *parent, size_t minutes, size_t seconds) : minute(minutes), second(seconds)
{
    timer = new QTimer();
    timeValue = new QTime(0,minutes,seconds);
    this->setParent(parent);
    this->display(timeValue->toString());
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(setDisplay()));
}

CountDownTimer::~CountDownTimer(){}

void CountDownTimer::setDisplay(){

    this->timeValue->setHMS(0,this->timeValue->addSecs(-1).minute(),this->timeValue->addSecs(-1).second());

    this->display(this->timeValue->toString());
}
