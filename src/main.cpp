#include <QApplication>

#include <csignal>

#include "widgets/MainWindow.hpp"

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
