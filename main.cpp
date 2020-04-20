#include "findpairs.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FindPairs w;

    w.show();
    return a.exec();
}
