#ifndef PLAYINGFORM_H
#define PLAYINGFORM_H

#include <QWidget>
#include <QHash>
#include <QLabel>
#include <QGraphicsView>
#include <QPixmap>
class Network;
namespace Ui {
class PlayingForm;
}

class PlayingForm : public QWidget
{
    Q_OBJECT
    friend class Network;
public:
    explicit PlayingForm(QWidget *parent = 0);
    ~PlayingForm();
    void GetNetworkPointer(Network*);
    QString getRemoteName();
    int checkGrid(int);
    void displayTable();
signals:
    void Clicked(QPoint);
    void SendClickedObject(QLabel*,int);
protected:
    void initStates();
    void appendChatMessage(QString);
    void mouseReleaseEvent(QMouseEvent *);
    void SetModelIcon(QPixmap*);
    bool SetRemoteModelIcon(char);
    void setRemoteName(QString);
    void setMove(bool);
    void setScore(qint64);
    void setName(QString);
    void setIssServer(bool);
    void setRemoteScore(qint64);
    void changeBoxState(QLabel*,const char);
    void changeBoxState(int,char);
    QHash <QLabel*, char> state_table;
    QHash <int, QLabel*> table;
    void initStateTable();
private slots:
    void InitializeForm();\
    void ClickedWhere(const QPoint&);
    void onCreateWindow();
    void on_sendMessageButton_clicked();
    void on_sendMessageValue_textEdited(const QString &arg1);
private:
    //void ClickOnObject(QLabel*);
    Ui::PlayingForm *ui;
    QPixmap *empty_box;
    QPixmap *X_box;
    QPixmap *O_box;
    Network *network;
};

#endif // PLAYINGFORM_H
