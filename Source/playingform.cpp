#include "playingform.h"
#include "ui_playingform.h"
#include "network.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPixmap>
#include <QDir>
#include <QLabel>
#include <QDebug>
#include <QTime>
#include <QHash>
PlayingForm::PlayingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayingForm)
{
    empty_box = new QPixmap(QDir::currentPath() +   "/BOX_TEXTURE.png");
    if(empty_box->isNull())
    {
        qDebug() << "Failed loading BOX_TEXTURE.PNG";
        qDebug() << QDir::currentPath() + "/BOX_TEXTURE.png";
    }
    ui->setupUi(this);
    table.insert(1,ui->box_One);
    table.insert(2,ui->box_Two);
    table.insert(3,ui->box_Three);
    table.insert(4,ui->box_Four);
    table.insert(5,ui->box_Five);
    table.insert(6,ui->box_Six);
    table.insert(7,ui->box_Seven);
    table.insert(8,ui->box_Eight);
    table.insert(9,ui->box_Nine);
    for(int i = 1; i <= 9; i++)
    {
        table[i]->setPixmap(*empty_box);
        state_table.insert(table[i],i+48);
    }
    connect(this, SIGNAL(Clicked(QPoint)), this, SLOT(ClickedWhere(QPoint)));
    this->setFixedSize(this->window()->size());
    delete empty_box;
    QPixmap * frame = new QPixmap();
    frame->load(QDir::currentPath() + "/FRAME.png");
    ui->frame->setPixmap(*frame);
    delete frame;
    QIcon icon;
    icon.addFile(QDir::currentPath()+"/ICON.png");
    this->setWindowIcon(icon);
}

void PlayingForm::changeBoxState(int what_box,char what_state)
{
    QPixmap *state = new QPixmap();
    state->load(QDir::currentPath()+(what_state=='X'?X_TEXTURE:O_TEXTURE));
    table[what_box]->setPixmap(*state);
    state_table[table[what_box]] = what_state;
    delete state;
}

void PlayingForm::changeBoxState(QLabel* what_box, const char what_state)
{
    QPixmap* state = new QPixmap();
    if(what_state == 'X')
    {
        state->load(QDir::currentPath() + X_TEXTURE);
        what_box->setPixmap(*state);
        state_table[what_box] = what_state;
    }
    else
    {
        state->load(QDir::currentPath() + O_TEXTURE);
        what_box->setPixmap(*state);
        state_table[what_box] = what_state;
    }
    delete state;
}

void PlayingForm::GetNetworkPointer(Network* pointer)
{
    network = pointer;
    connect(this, SIGNAL(SendClickedObject(QLabel*,int)), network, SLOT(OnClickedObject(QLabel*,int)));
}

void PlayingForm::initStateTable()
{
    for(int i = 1; i <= 9;i++)
    {
        state_table[table[i]] = 'N';
        ui->winStripe->setBaseSize(4,562);
        ui->winStripe->setPixmap(QPixmap(0,0));
        qDebug() << "state_table[" << i << "] == " << state_table.value(table[i]);
    }
}

void PlayingForm::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit Clicked(event->pos());
    }
}

void PlayingForm::displayTable()
{
    for(int i = 1; i <= 9;i++)
    {
        qDebug()  << state_table[table[i]];
    }
}

void PlayingForm::ClickedWhere(const QPoint &where)
{
    for(int i = 1; i <= 9; i++)
    {
        if( (where.x() >= table[i]->pos().x()&& where.x() <= table[i]->pos().x() + table[i]->width())
                &&(where.y() >= table[i]->pos().y() && where.y() <= table[i]->pos().y() + table[i]->height()))
        {
            if(state_table.value(table[i])!='X' && state_table.value(table[i]) !='O')
            {
                network->SendData(qint64(1),QByteArray(network->getMove()?"1":"0"),Network::Message::CheckMove);
                emit SendClickedObject(table[i],i);
                if(checkGrid(0) != 0)
                {
                    network->SendData(qint64(1),QByteArray(checkGrid(4)!=0?checkGrid(4)+"0":"10"),Network::Message::OpponentWinning);
                    network->haveWinningPlayer(checkGrid(0)==10&&checkGrid(4)!=0?checkGrid(4):checkGrid(0));
                }
                break;
            }
        }
    }
}

int PlayingForm::checkGrid(int a)
{
    static int pos = 0;
    if 		(a == 1){pos = 0; return 0;}
    if		(a == 3){
        switch(pos){
        case 1:
            return 3;
            break;
        case 2:
            return 6;
            break;
        case 3:
            return 9;
            break;
        case 4:
            return 1;
            break;
        case 5:
            return 2;
            break;
        case 6:
            return 3;
            break;
        case 7:
            return 9;
            break;
        case 8:
            return 3;
            break;
        default:break;}}
    if (network->getCount() == 9 && a != 4) {qDebug() << "Round draw?" << network->getCount();return 10;}
    else if	(state_table[table[1]] == state_table[table[2]] && state_table[table[1]]  == state_table[table[3]]){pos = 1;    return checkGrid(3);}
    else if	(state_table[table[4]] == state_table[table[5]] && state_table[table[4]] == state_table[table[6]]){pos = 2;     return checkGrid(3);}
    else if (state_table[table[7]] == state_table[table[8]] && state_table[table[7]] == state_table[table[9]]){pos = 3;     return checkGrid(3);}
    else if (state_table[table[1]] == state_table[table[4]] && state_table[table[1]] == state_table[table[7]]){pos = 4;     return checkGrid(3);}
    else if (state_table[table[2]] == state_table[table[5]] && state_table[table[2]] == state_table[table[8]]){pos = 5;     return checkGrid(3);}
    else if (state_table[table[3]] == state_table[table[6]] && state_table[table[3]] == state_table[table[9]]){pos = 6;     return checkGrid(3);}
    else if (state_table[table[1]] == state_table[table[5]] && state_table[table[1]] == state_table[table[9]]){pos = 7;     return checkGrid(3);}
    else if (state_table[table[3]] == state_table[table[5]] && state_table[table[3]] == state_table[table[7]]){pos = 8;     return checkGrid(3);}
    return 0;
}

void PlayingForm::setScore(qint64 score)
{
    ui->statusScore->setText(QString::number(score));
}

void PlayingForm::setRemoteScore(qint64 score)
{
    ui->statusScoreOpponent->setText(QString::number(score));
}

void PlayingForm::initStates()
{
    empty_box = new QPixmap();
    empty_box->load(QDir::currentPath() + "/BOX_TEXTURE.png");
    for(int i = 1; i <= 9; i++)
    {
        state_table[table[i]] = i+48;
        table[i]->setPixmap(*empty_box);
    }
    delete empty_box;
}


void PlayingForm::setIssServer(bool isserver)
{
    ui->statusServer->setText(QString(isserver==true?"TRUE":"FALSE"));
    ui->statusServerOpponent->setText(QString(isserver==true?"FALSE":"TRUE"));
}

void PlayingForm::setMove(bool isMoving)
{
    ui->statusMove->setText(QString(isMoving==true?"TRUE":"FALSE"));
    ui->statusMoveOpponent->setText(QString(isMoving==true?"FALSE":"TRUE"));
}

void PlayingForm::setRemoteName(QString remote_name)
{
    ui->statusNameOpponent->setText(remote_name);
}

bool PlayingForm::SetRemoteModelIcon(char is_X)//
{
    QPixmap *X;
    QPixmap *O;
    qDebug() << "SET REMOTE MODEL ICON CHAR IS X:" << is_X;
    if(is_X == 'X')
    {
        X = new QPixmap(QDir::currentPath() +X_TEXTURE);
        ui->Opponents_Model->setPixmap(*X);
        qDebug() << "Applied Opponents_Model";
        O = new QPixmap();
        O->load(QDir::currentPath() +O_TEXTURE);
        ui->Your_Model->setPixmap(*O);
        qDebug() << "Applied Your Model";
    }
    else
    {
        O = new QPixmap(QDir::currentPath() + O_TEXTURE);
        ui->Opponents_Model->setPixmap(*O);
        qDebug() << "Applied Opponents_Model";
        X = new QPixmap();
        X->load(QDir::currentPath() + X_TEXTURE);
        ui->Your_Model->setPixmap(*X);
        qDebug() << "Applied Your Model";
    }
    delete X;
    delete O;
    return (!ui->Opponents_Model->pixmap()->isNull() &&  !ui->Your_Model->pixmap()->isNull());
}

void PlayingForm::setName(QString what_name)
{
    ui->statusName->setText(what_name);
}

void PlayingForm::onCreateWindow()
{
    InitializeForm();
    this->show();
    qDebug() << "Signal emitted";
}

void PlayingForm::InitializeForm()
{
    this->setWindowTitle("Tic-Tac-Toe" + QString(network->isServer()?"-S":"-C"));
}

PlayingForm::~PlayingForm()
{
    delete ui;
}

void PlayingForm::appendChatMessage(QString message)
{
    ui->Chat->append(message);
}

QString PlayingForm::getRemoteName()
{
    return ui->statusNameOpponent->text();
}

void PlayingForm::on_sendMessageButton_clicked()
{
    if(ui->sendMessageValue->text().size()> 0)
    {
        network->SendData(qint64(ui->sendMessageValue->text().size()),ui->sendMessageValue->text().toLatin1(), Network::Message::Msg);
        appendChatMessage(QString("[%1]%2:%3").arg(QTime::currentTime().toString(), network->getName(),ui->sendMessageValue->text()));
        ui->sendMessageValue->setText(QString(""));
    }
}

void PlayingForm::on_sendMessageValue_textEdited(const QString &arg1)
{
    if(arg1.size() > 30)
    {
        QString temp(arg1);
        temp.remove(30,arg1.size() - 30);
        ui->sendMessageValue->setText(temp);
    }

}
