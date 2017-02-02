#include "network.h"
#include "playingform.h"
#include "QMessageBox"
#include <QApplication>
#include <QLabel>
#include <QIODevice>
#include <QPixmap>
#include <QIcon>
#include <QDir>
#include <QTime>
#include "ctime"
#include "stdlib.h"
#include <exception>
//#include "error.h"
Network::Network(QObject *parent) :
    QObject(parent)
{
    score =  new int(0);
    ismoving = new bool(false);
    count = new int(0);
    model = new char('N');
}

Network::~Network()
{
    delete  name;
    delete  isserver;
    delete  score;
    delete  ismoving;
    delete model;
    delete count;
}

void Network::GetPlayingFormPointer(PlayingForm *pointer)
{
    playing_form = pointer;
    connect(this, SIGNAL(OpenPlayingForm()), playing_form, SLOT(onCreateWindow()));
}

bool Network::getMove()
{
    return *ismoving;
}

void Network::haveWinningPlayer(const int &what_type)
{
    if(what_type == 10)
    {
        playing_form->appendChatMessage(QString("[%1]:%2").arg(QTime::currentTime().toString(),QString("ROUND ENDED UP BEIGN DRAW")));
    }
    else if(*model == playing_form->state_table.value(playing_form->table[what_type]))
    {
        if(*isserver)
        {
            *score +=1;
            SendData(qint64(QString::number(*score).length()),QByteArray(QString::number(*score).toUtf8()),Message::ApplyRemoteScore);
        }
        playing_form->appendChatMessage(QString("[%1]:%2 WON (you)").arg(QTime::currentTime().toString(),QString(*name)));
        playing_form->setScore(qint64(*score));
    }
    else
    {
        if(*isserver)
        {
            SendData(qint64(1),QByteArray("1"),Message::IncreaseScoreBy);
        }
        playing_form->appendChatMessage(QString("[%1]:%2 WON").arg(QTime::currentTime().toString(),playing_form->getRemoteName()));
    }
    playing_form->initStates();
    *count = int(0);
    if(*isserver){
        *model = (*model=='X'?'O':'X');
        *ismoving = (*model=='X'?true:false);
        SendData(qint64(1),QByteArray(*ismoving?"0":"1"),Message::ApplyMove);
        SendData(qint64(1),QByteArray(*model=='X'?"O":"X"),Message::ApplyModel);
        playing_form->SetRemoteModelIcon(*model=='X'?'O':'X');
        playing_form->setMove(*ismoving);
    }
}

void Network::OnClickedObject(QLabel* clickedObject,const int &id)
{
    if(*ismoving)
    {
        playing_form->changeBoxState(clickedObject,*model);
        *ismoving = false;
        SendData(qint64(1),QByteArray(QString::number(id).toLocal8Bit()),Message::ClickedObject);
        playing_form->setMove(false);
        *count += 1;
    }
    else
    {
        playing_form->appendChatMessage("NOT your turn!");
    }
}

bool Network::SetName(const QString &temp)
{
    name = new QString(temp);
    return(*name == temp);
}

bool Network::SetupServer()
{
    qDebug() << "Setting up the server";
    isserver = new bool(true);
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()),this, SLOT(connected()));
    return server->listen(QHostAddress::Any, 55555);
}

bool Network::InitializeParams()
{
    int random_number = 0;
    srand(time(NULL));
    random_number = rand()%100;
     qDebug() << (random_number<=50?"X":"O") << random_number;
     *model = (random_number<=50?'X':'O');
    *ismoving = (*model=='X'?true:false);
    *score = 0;
    playing_form->SetRemoteModelIcon(*model=='X'?'O':'X');
    playing_form->setMove(*ismoving);
    playing_form->setName(QString(*name));
    playing_form->setIssServer(*isserver);
    playing_form->setScore(int(0));
    playing_form->setRemoteScore(int(0));
    if(!SendData(qint64(1),QByteArray((*model=='X'?"O":"X")),qint64(Message::ApplyModel))
            ||!SendData(qint64(1),QByteArray(*ismoving==true?"0":"1"),qint64(Message::ApplyMove))
            || !SendData(qint64(name->size()),name->toUtf8(),qint64(Message::ApplyRemoteName))
            || !SendData(qint64(2),QByteArray("--"),Message::RequestRemoteName))
    {

        qDebug() << "Fail on INITIALIZATION PARAMS";
        return false;
    }
    return true;
}

bool Network::SendData(const qint64 &size, const QByteArray &message,const qint16 &type) // TO EDIT
{
    QByteArray packet_info;
    QDataStream in (&packet_info, QIODevice::WriteOnly);
    in << size;
    in << type;
    in << message.data();
    in.device()->seek(qint64(0));
    if(socket->state() == QUdpSocket::ConnectedState)
    {
        socket->write(packet_info);
        return (socket->waitForBytesWritten(-1));
    }
    return false;
}

bool Network::isServer()
{
    return *isserver;
}

void Network::disconnected()
{
    playing_form->appendChatMessage(QString("DISCONNECTED!"));
    if(*isserver)
    {
        server->deleteLater();
    }
    socket->deleteLater();
}

bool Network::sendPing()
{
    return(SendData(qint64(4), "PING", Message::Ping));
}

QString const Network::getName()
{
    return *name;
}

void Network::ProceedRead() // size -> type -> msg
{
    QByteArray *buff = new QByteArray();
    *buff = socket->readAll();
    QDataStream *in = new QDataStream(buff, QIODevice::ReadOnly );
    qint64 *size = new qint64(-1);
    qint16 *type = new qint16(-1);
    int* id;
    while(in->device()->bytesAvailable()>0)
    {
        *in >> *size;
        char *data;
        *in >> *type;
        qDebug() << (*isserver?"SERVER:":"CLIENT:") << in->device()->bytesAvailable()  <<"BYTES << TYPE: " << *type << "SIZE:" << *size;
        if(*size==0)
        {
            return;
        }
        switch(*type)
        {
        case Message::Ping:
            qDebug() << "PING";
            break;
        case Message::Msg:
            data = new char[*size];
            *in >> data;
            playing_form->appendChatMessage(QString("[%1]%2:%3").arg(QTime::currentTime().toString(), playing_form->getRemoteName(),QString(data)));
            delete data;
            break;
        case Message::RequestRemoteName:
            SendData(qint64(name->size()),name->toUtf8(),qint64(Message::ApplyRemoteName));
            break;
        case Message::ApplyModel:
            data = new char[*size];
            *in >> data;
            *model = *data;
            playing_form->SetRemoteModelIcon(*model=='X'?'O':'X');
            delete data;
            break;
        case Message::ApplyRemoteScore:
            data = new char[*size];
            *in >> data;
            playing_form->setRemoteScore(qint64(atoi(data)));
            delete data;
            break;
        case Message::ApplyMove:
            data = new char[*size];
            *in >> data;
            playing_form->setMove(data[0]=='1'?true:false);
            *ismoving = (data[0]=='1'?true:false);
            delete data;
            break;
        case Message::ApplyRemoteName:
            data = new char[*size];
            *in >> data;
            playing_form->setRemoteName(QString(data));
            playing_form->setName(*name);
            delete data;
            break;
        case Message::ClickedObject:
            data = new char[*size];
            *in >> data;
            qDebug() << "CLICKED" << data[0]-48;
            id = new int(data[0]-48);
            playing_form->changeBoxState(*id, (*model=='X'?'O':'X'));
            playing_form->setMove(true);
            *ismoving = true;
            *count+= 1;
            delete id;
            break;
        case Message::OpponentWinning:
            data = new char[*size];
            *in >> data;
            haveWinningPlayer(data[0]-48);
            if(*isserver)
            {
                SendData(qint64(1),QByteArray(*model=='X'?"0":"1"),Message::ApplyMove);
            }
            delete data;
            break;
        case Message::IncreaseScoreBy:
            data = new char[*size];
            *in >> data;
            *score += atoi(data);
            qDebug() << "Increased score by " << atoi(data);
            SendData(qint64(QString::number(*score).length()),QByteArray(QString::number(*score).toUtf8()),Message::ApplyRemoteScore);
            playing_form->setScore(*score);
            delete data;
            break;
        case Message::YesIAm:
            playing_form->appendChatMessage(QString("[%1]%2:%3").arg(QTime::currentTime().toString(), playing_form->getRemoteName(),QString(data)));
            break;
        case Message::AreYouThere:
            playing_form->appendChatMessage(QString("[%1]%2:%3").arg(QTime::currentTime().toString(), playing_form->getRemoteName(),QString("ARE YOU THERE?")));
            SendData(qint64(1),QByteArray("1"),Message::YesIAm);
            playing_form->appendChatMessage(QString("[%1]%2:%3").arg(QTime::currentTime().toString(), *name,QString("YES I AM")));
            break;
        case Message::NoOneMoves:
            if(*isserver)
            {
                SendData(qint64(1),QByteArray(*ismoving?"0":"1"),Message::ApplyMove);
                SendData(qint64(1),QByteArray("-"),Message::NoOneMoves);
            }
            playing_form->initStates();
            break;
        case Message::CheckMove:
            data = new char[*size];
            *in >> data;
            if(bool(atoi(data)) == *ismoving)
            {
                qDebug() << "Err:: NO ONE MOVES";
                if(*isserver)
                {
                    playing_form->appendChatMessage(QString("Err::1--TcpPacketDrop(ApplyMove)..now resending"));
                    SendData(qint64(1),QByteArray(*ismoving?"0":"1"),Message::ApplyMove);
                }
                else{
                    SendData(qint64(1),QByteArray(*ismoving?"1":"0"),Message::NoOneMoves);
                }
            }
            delete data;
            break;
        default:
            qDebug() << "NO SUCH TYPE OF MESSAGE " << type;
            break;
        }
    }
    delete buff;
    delete in;
    delete size;
    delete type ;
}

int Network::getCount()
{
    return *count;
}

void Network::connected()
{
    qDebug() << "CONNECTED";
    if(server->hasPendingConnections())
    {
        socket = new QTcpSocket();
        socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(ProceedRead()));
        connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    }
    else
    {
        QApplication::exit();
    }
    disconnect(server, SIGNAL(newConnection()), this, SLOT(connected()));
    emit(OpenPlayingForm());
    InitializeParams();
}

bool Network::SetupClient(const QHostAddress *what_ip)
{
    isserver = new bool(false);
    socket = new QTcpSocket(this);
    qDebug() << "Setting up the client " << what_ip->toString();
    connect (socket, SIGNAL(readyRead()), this, SLOT(ProceedRead()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    socket->connectToHost(*what_ip,quint16(55555), QTcpSocket::ReadWrite);
    if(socket->waitForConnected(-1))
    {
        emit OpenPlayingForm();
        playing_form->setIssServer(false);
        playing_form->setScore(qint64(0));
        playing_form->setRemoteScore(qint64(0));
        return true;
    }
    return false; // ssssss
}
