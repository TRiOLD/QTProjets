#include "immintrin.h"
#include "Program.h"

#include "mainwindow.h"
#include <QApplication>
#include <QLabel>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    Program prg( argc, argv );
    MainWindow w;
    prg.initialize();
    w.show();

    S32 res = a.exec();
    prg.shutdown();
    return res;
}
