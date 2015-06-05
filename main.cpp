/**
  PIC 10C
  Yao Xiao
  004171960
  HW10

  @file main.cpp
  @only play mainwindow here
  @All copyrights of pictures in this game belong to NiuHongHong（牛轰轰）
  */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
