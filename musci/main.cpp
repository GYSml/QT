#include "widget.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixMap(":/new/prefix1/icon/splash.png");
    QSplashScreen splash(pixMap);
    splash.show();
    a.processEvents();
    Widget w;
    w.setWindowTitle("随心而动");
    w.setAttribute(Qt::WA_TranslucentBackground,true);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    splash.finish(&w);
    return a.exec();
}
