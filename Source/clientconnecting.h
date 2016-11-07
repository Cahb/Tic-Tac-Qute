#ifndef CLIENTCONNECTING_H
#define CLIENTCONNECTING_H

#include <QWidget>
#include <QtCore>
#include <QTcpSocket>
class Network;
namespace Ui {
class ClientConnecting;
}

class ClientConnecting : public QWidget
{
    Q_OBJECT
public:
    explicit ClientConnecting(QWidget *parent = 0);
    ~ClientConnecting();
private slots:
    void OnClose();
    void on_pushButton_clicked();
    void onCreateWindow(Network*);
private:
    Ui::ClientConnecting *ui;
    Network *network;
};

#endif // CLIENTCONNECTING_H
