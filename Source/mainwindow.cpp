#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientconnecting.h"
#include "network.h"
#include "serverwaiting.h"

MainWindow::MainWindow(ServerWaiting *srv, ClientConnecting *clnt,Network *ntwrk, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Tic-tac-toe");
    this->setFixedSize(this->window()->size());
    connect(this, SIGNAL(createClientWindow(Network*)), clnt, SLOT(onCreateWindow(Network*)));
    qDebug() << "Connected createClientWindow-onCreateWindow in mainwindow.cpp";
    connect(this, SIGNAL(createServerWindow(Network*)), srv, SLOT(onCreateWindow(Network*)));
    qDebug() << "Conected createServerWindow-onCreateWindow in mainwindow.cpp";
    srv_form = srv;
    clnt_form = clnt;
    network = ntwrk;
    QIcon icon;
    icon.addFile(QDir::currentPath()+"/ICON.png");
    this->setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    if( !arg1.isEmpty() && !(arg1[arg1.size() - 1] >= 65 && arg1[arg1.size() -1] <= 90  )
            && !(arg1[arg1.size() - 1 ] >= 97 && arg1[arg1.size()-1] <= 122  ) && arg1.size() < 15)
    {
        ui->lineEdit->setText(arg1);
    }
    else if(arg1.size() > 15)
    {
        QString temp(arg1);
        temp.remove(15,arg1.size() - 15);
        ui->lineEdit->setText(temp);
    }
}

void MainWindow::on_applyButton_clicked()
{
    if(ui->lineEdit->text().size() > 0)
    {
        ui->lineEdit->setEnabled(0);
        ui->applyButton->setEnabled(false);
    }
}

void MainWindow::on_proceedButton_clicked()
{
    if( (ui->radiobuttonClient->isChecked() || ui->radiobuttonServer->isChecked() )
            && ui->lineEdit->text().size() > 0 &&  !(ui->lineEdit->isEnabled() ))
    {
        if(ui->radiobuttonServer->isChecked())
        {
            emit createServerWindow(this->network);
            if(!network->SetName(ui->lineEdit->text()))
            {
                qDebug() << "Fail on taking name " << ui->lineEdit->text();
            }
            this->close();
            clnt_form->deleteLater();
            this->deleteLater();
        }
        else if (ui->radiobuttonClient->isChecked())
        {
            emit createClientWindow(this->network);
            if(!network->SetName(ui->lineEdit->text()))
            {
                qDebug() << "Fail on taking name " << ui->lineEdit->text();
            }
            this->close();
            srv_form->deleteLater();
            this->deleteLater();
        }
    }
}
