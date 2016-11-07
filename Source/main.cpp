#include "mainwindow.h"
#include "serverwaiting.h"
#include "clientconnecting.h"
#include "playingform.h"
#include "network.h"
//#include "error.h"
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    Network *network;
    PlayingForm *pl;
    network = new Network();
    pl = new PlayingForm();
    network->GetPlayingFormPointer(pl);
    pl->GetNetworkPointer(network);
    ClientConnecting *clnt = new ClientConnecting();
    ServerWaiting *srv = new ServerWaiting();
    MainWindow *w = new MainWindow(srv, clnt, network);
    w->show();
    return a.exec();
}
