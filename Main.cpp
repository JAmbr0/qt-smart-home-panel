#include "HomeScreen.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HomeScreen homescreen;
    homescreen.resize(2000, 1200);
    homescreen.setMinimumSize(1000, 600);
    homescreen.move(QGuiApplication::primaryScreen()->geometry().center() - homescreen.rect().center());
    return app.exec();
}
