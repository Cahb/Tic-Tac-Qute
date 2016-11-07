#include "serverwaiting.h"
#include "ui_serverwaiting.h"
#include "network.h"
#include <QDebug>
#include <QApplication>

ServerWaiting::ServerWaiting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWaiting)
{
    ui->setupUi(this);
    this->setWindowTitle("Tic-tac-toe-S");
    this->setFixedSize(this->window()->size());
    QIcon icon;
    icon.addFile(QDir::currentPath()+"/ICON.png");
    this->setWindowIcon(icon);
}

ServerWaiting::~ServerWaiting()
{
    delete ui;
}

void ServerWaiting::newConnection()
{
    disconnect(network->socket, SIGNAL(readyRead()), this, SLOT(newConnection()));
    this->close();
}

void ServerWaiting::onCreateWindow(Network *ntwrk)
{
    network = ntwrk;
    if(!network->SetupServer())
    {
        qDebug() << "Can't setup server";
        QApplication::exit();
    }
    connect(network->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    this->show();
}

void ServerWaiting::on_pushButton_clicked()
{
    this->close();
    this->deleteLater();
}
