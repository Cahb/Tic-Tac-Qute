#ifndef SERVERWAITING_H
#define SERVERWAITING_H

#include <QWidget>
#include <QtCore>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
class Network;

namespace Ui {
class ServerWaiting;
}

class ServerWaiting : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWaiting(QWidget *parent = 0);
    ~ServerWaiting();
private slots:
    void on_pushButton_clicked();
    void newConnection();
    void onCreateWindow(Network *ntwrk);
private:
    Ui::ServerWaiting *ui;
    Network *network;
};

#endif // SERVERWAITING_H
