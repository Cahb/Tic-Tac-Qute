#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QString>
#include <QHostAddress>
#include <QObject>
#include <QLabel>
#define X_TEXTURE "/X_TEXTURE.png"
#define O_TEXTURE "/O_TEXTURE.png"
class ServerWaiting;
class ClientConnecting;
class PlayingForm;
//class ErrorWind;

class Network : public QObject
{
    Q_OBJECT
    friend class ClientConnecting;
    friend class ServerWaiting;
    friend class PlayingForm;
public:
    explicit Network(QObject *parent = 0);
    virtual ~Network();
    bool SetName(const QString&);
    bool SetupServer();
    bool SetupClient(const QHostAddress*);
    void GetPlayingFormPointer(PlayingForm*);
    bool getMove();
    const QString getName();
    char getModel();
    int getScore();
    int getCount();
    bool isServer();
    bool sendPing();
private slots:
    void disconnected();
    void OnClickedObject(QLabel*,const int&);
    void connected();
    void ProceedRead();
    //void ErrorWindowOkClicked(); simply not used - added chat info about winnig \ draw \ losing;
public slots:
signals:
    void _connected();
    void OpenPlayingForm();
protected:
    bool SendData(const qint64&, const QByteArray&,const qint16&);
    void haveWinningPlayer(const int&);
    QTcpSocket *socket;
    QTcpServer *server;
private:
    bool InitializeParams();
    void RoundDraw();
    enum Message{Ping=0,Msg, RequestRemoteName,ApplyMove,ApplyModel,ApplyRemoteName,ApplyRemoteScore, CloseMessage,ClickedObject,
                IncreaseScoreBy,OpponentWinning,AreYouThere,YesIAm,CheckMove,NoOneMoves};
    QString *name;
    int* count;
    PlayingForm *playing_form;
    const bool *isserver;
    int *score;
    char *model;
    bool *ismoving;
    //ErrorWind *message;
};
#endif // NETWORK_H
