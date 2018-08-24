#include "mainwindow.h"
#include <QApplication>
#include <thread>

#include "main_window_setup.h"
#include "TCPClient.h"

void client_thread(int argc, char *argv[])
{
    TCPClient *client = new TCPClient();

    // Validate the parameters
    if (argc != 3) {
        printf("usage: %s server-name\n", argv[1]);
        return;
    }

    if (client->setup(argv[2]))
        return;

    if (client->run_client())
        return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *myWidget = new QWidget();

    myWidget->setGeometry(0,0,1370,700);

    accessories(myWidget);
    chessBoard(myWidget,tile);

    myWidget->show();

    std::thread t = std::thread(client_thread, argc, argv);
   t.detach();

    return a.exec();
}
