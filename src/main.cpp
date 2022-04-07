#ifdef _WIN32
#    pragma warning(disable : 4005)
#endif
#include <QApplication>

#include "widgets/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
