#include "serialconnection.h"

SerialConnection::SerialConnection(MainWindow *parent)
    : QWidget(parent)
{
    serial = new QSerialPort();
    this->parent = parent;

    lastDataWritten = "";
    isConnected = false;

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    //std::string name = "toto";
    //openSerialPort(name);
}

void SerialConnection::getSerialList(QComboBox* list) {
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.description().contains("mbed"))
        {
            list->insertItem(0, info.portName() + " : " + info.description());
        }
        else
        {
            list->addItem(info.portName() + " : " + info.description());
        }
    }
    list->setCurrentIndex(0);
}

void SerialConnection::openSerialPort(QString name)
{
    serial->setPortName(name);
    if (!isConnected)
    {
        if (serial->open(QIODevice::ReadWrite))
        {
            if(serial->setBaudRate(9600))
            {
                qDebug() << "Connected";
                //serial->setReadBufferSize(13);

                isConnected = true;
            }
            else
            {
                serial->close();
                qDebug() << serial->errorString();
                throw 1;
            }
        }
        else
        {
            qDebug() << serial->errorString();
            throw 1;
        }
    }
}

void SerialConnection::readData()
{
    QByteArray d = serial->readAll();
    QString data(d);
    qDebug() << "Recu " << data;
    *(parent->out) << "Recu " << data << "\n";

    if (parent->getState() == POSITION)
    {
        if (data.contains("done") && data.contains(lastDataWritten))     { parent->positionOK(); }
        else                                                             { writeData(lastDataWritten); }
    }

}

void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(2000);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void SerialConnection::sendBegin()
{
    QString envoi="";
    //writeData(envoi);
}

void SerialConnection::sendEnd()
{
    QString envoi="$00000";
    //writeData(envoi);
}

void SerialConnection::sendAbort()
{
    QString envoi="";
    //writeData(envoi);
}

void SerialConnection::sendPosition(int angleVert, int angleHoriz)
{
    QChar msg[10];
    msg[0] = '$';
    msg[1] = '0';
    msg[2] = (angleVert < 0 ? '-' : '+');
    msg[3] = (char)((abs(angleVert)/100) % 10) + 48;
    msg[4] = (char)((abs(angleVert)/10) % 10) + 48;
    msg[5] = (char)((abs(angleVert)/1) % 10) + 48;
    msg[6] = (angleHoriz < 0 ? '-' : '+');
    msg[7] = (char)((abs(angleHoriz)/100) % 10) + 48;
    msg[8] = (char)((abs(angleHoriz)/10) % 10) + 48;
    msg[9] = (char)((abs(angleHoriz)/1) % 10) + 48;

    QString envoi(msg, 10);
    //qDebug() << "Position : " << envoi;
    writeData(envoi);
}

void SerialConnection::sendLaserOn()
{
    QString envoi = "$101100000";
    writeData(envoi);
}

void SerialConnection::sendLaserOff()
{
    QString envoi = "$100000000";
    writeData(envoi);
}

void SerialConnection::getPosition()
{
    // TODO comment lire la position
    //QByteArray data = serial->readAll();
    //qDebug() << "Recu" << data;
}

void SerialConnection::writeData(const QString &data)
{
    lastDataWritten = QString(data);
    QByteArray byteData = data.toLocal8Bit();
    byteData.truncate(10);
    serial->write(byteData);
    qDebug() << parent->getState() << "Envoyé" << data;
    *(parent->out) << parent->getState() << "Envoyé" << data << "\n";
}


void SerialConnection::closeSerialPort()
{
    serial->close();
    // GUI notes
}

SerialConnection::~SerialConnection()
{
    closeSerialPort();
}
