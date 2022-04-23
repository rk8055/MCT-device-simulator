#include "mct_simulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MCT_Simulator w;
    w.show();


    return a.exec();
}
