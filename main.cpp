#include "datastudent.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataStudent w;
    w.show();
    return a.exec();
}
