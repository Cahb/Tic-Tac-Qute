#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class ServerWaiting;
class ClientConnecting;
class Network;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(ServerWaiting *srv, ClientConnecting *clnt, Network *ntwrk, QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_lineEdit_textEdited(const QString &arg1);
    void on_applyButton_clicked();
    void on_proceedButton_clicked();
signals:
    void createServerWindow(Network*);
    void createClientWindow(Network*);
private:
    Ui::MainWindow *ui;
    ServerWaiting *srv_form;
    ClientConnecting *clnt_form;
    Network *network;
};

#endif // MAINWINDOW_H
