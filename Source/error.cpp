#include "error.h"
#include "ui_errorwind.h"

ErrorWind::ErrorWind(QString title, QString msg,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorWind)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label->setText(msg);
    this->setModal(true);
    this->setWindowFlags(Qt::WindowTitleHint);
    this->show();
}

ErrorWind::~ErrorWind()
{
    delete ui;
}

void ErrorWind::on_pushButton_clicked()
{
    this->close();
    emit(Clicked());
    this->deleteLater();
}
