#include "mainwindow.h"
#include <QApplication>


#include "TCPServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TCPServer *server = new TCPServer();

    if (server->setup() == 1) {
        return 1;
    }

    if (server->run_server() == 1)
        return 1;

    QWidget *myWidget = new QWidget();

    myWidget->setGeometry(0,0,1370,700);

   // MainWindow w;
  //  w.show();

    return a.exec();
}
