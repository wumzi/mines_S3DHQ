#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QMainWindow>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>
#include <QString>
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>
#include "mainwindow.h"

class MainWindow;

class SerialConnection : public QWidget
{
    Q_OBJECT

private:
    MainWindow* parent;
    QSerialPort* serial;

    QString lastDataWritten;
    bool isConnected;

    void writeData(const QString &data);

public:
    SerialConnection(MainWindow *parent);
    ~SerialConnection();

    void openSerialPort(QString name);
    void closeSerialPort();
    void getSerialList(QComboBox *list);
    void sendBegin();
    void sendEnd();
    void sendAbort();
    void sendPosition(int angleVert, int angleHoriz);
    void sendLaserOn();
    void sendLaserOff();
    void getPosition();

public slots:
    void readData();
};

void delay();

#endif // SERIALCONNECTION_H
