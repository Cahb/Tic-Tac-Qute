#include "clientconnecting.h"
#include "ui_clientconnecting.h"
#include "network.h"
#include <QDebug>
#include <QIODevice>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QWidget>
#include <QIcon>
#define NOT_CONNECTED_TEXT "<font color = 'red'>Not connected</font>"
#define CONNECTED_TEXT "<font color = 'green'>Connected</font>"
ClientConnecting::ClientConnecting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientConnecting)
{
    ui->setupUi(this);
    ui->status_bar->setText(NOT_CONNECTED_TEXT);
    this->setWindowTitle("Tic-tac-toe-C");
    this->setFixedSize(this->window()->size());
    QIcon icon;
    icon.addFile(QDir::currentPath()+"/ICON.png");
    this->setWindowIcon(icon);
}
void ClientConnecting::onCreateWindow(Network *ntwrk){
    network = ntwrk;
    qDebug() << "Conected connected-connected in clientconnecting.cpp";
    this->show();
}

ClientConnecting::~ClientConnecting()
{
    qDebug() << "Disconected connected-connected in clientconnecting.cpp";
    delete ui;
}

void ClientConnecting::OnClose()
{
    this->close();
    this->deleteLater();
}

void ClientConnecting::on_pushButton_clicked()
{
    QHostAddress *ip = new QHostAddress();
    if(ip->setAddress(ui->lineEdit->text())){
        qDebug() << "Succeed on taking ip " << ip->toString();
        if(!network->SetupClient(ip))
        {
            qDebug() << "Couldn't setup the client";
            QApplication::exit();
        }
        if(network->socket->state() == QUdpSocket::ConnectedState)
        {
            this->close();
            this->deleteLater();
        }
    }
    delete ip;
}


