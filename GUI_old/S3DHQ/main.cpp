#include "mainwindow.h"
#include "camera.h"
#include "serialconnection.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    // Debug of Camera
    //applicationRun();

    return a.exec();
}
