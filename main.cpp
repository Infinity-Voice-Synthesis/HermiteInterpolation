#include "HermiteInterpolation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HermiteInterpolation w;
    w.showMaximized();
    return a.exec();
}
