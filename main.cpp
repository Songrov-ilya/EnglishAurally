#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

// Законченна программа 05.05.2018. Делалась потихоньку месяца 2,3, по пару часов в день.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen splash(QPixmap(":/Images/Final Materials/Logo_temporarily.png"));
    MainWindow win;

    QPropertyAnimation* anim = new QPropertyAnimation(&splash, "pos");
    anim->setDuration(3000);
    anim->setStartValue(QPoint(splash.geometry().x(),0 - splash.size().height() - 5));
    anim->setEndValue(QPoint(splash.geometry().x(),QApplication::desktop()->height() / 2.6));
    anim->setEasingCurve(QEasingCurve::OutBounce);
    anim->start();

    splash.show();

                    QTime time;
                    time.start();
                        for (int i = 0; i < 100; )
                        {
                            QApplication::processEvents();
                            if (time.elapsed() > 40)
                            {
                                time.start();
                                ++i;
                            }
                            if(anim->currentTime() > 2500)
                                i = 101;
                        }
    splash.finish(&win);
    win.show();// Сначала показываем, а ниже устанавливаем геометрию.
    win.setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,win.size(),
                                        qApp->desktop()->availableGeometry()));

    return a.exec();
}
