/**
    Yao Xiao
    004171960
    PIC10C Spring 2015
    HW10

    @file countdowntimer.h
    @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
*/

#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>
class CountDownTimer: public QLCDNumber
{
  Q_OBJECT

  public:
  QTimer* timer;
  QTime*  timeValue;

  public:
    CountDownTimer(QWidget * parentWidget,int minutes,int seconds)
    {
        timer = new QTimer();
        timeValue = new QTime(0,minutes,seconds);
        this->setParent(parentWidget);
        this->display(timeValue->toString());
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(setDisplay()));
    }

    ~ CountDownTimer(){};

   public slots:
    void setDisplay()
    {
      this->timeValue->setHMS(0,this->timeValue->addSecs(-1).minute(),this->timeValue->addSecs(-1).second());
      this->display(this->timeValue->toString());
    };
};

#endif // COUNTDOWNTIMER_H
