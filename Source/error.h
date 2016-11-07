#ifndef ERROR_H
#define ERROR_H

#include <QDialog>

namespace Ui {
class ErrorWind;
}

class ErrorWind : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWind(QString , QString ,QWidget *parent = 0);
    ~ErrorWind();

private slots:
    void on_pushButton_clicked();
signals:
    void Clicked();
private:
    Ui::ErrorWind *ui;
};

#endif // ERROR_H
