#include "digitizer_ab.h"
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

Digitizer_ab::Digitizer_ab(QObject *parent) : QObject(parent),ID(-1)
{
    serial =nullptr;
}

Digitizer_ab::~Digitizer_ab()
{
    serial->close();
}

void Digitizer_ab::doPool()
{

}

bool Digitizer_ab::send(const QByteArray *func, const QByteArray *data)
{

}

int Digitizer_ab::recive(QByteArray *func, QByteArray *data)
{

}

bool Digitizer_ab::initSerial(QString portName, uint32_t baudRate)
{
    if(nullptr !=serial){
        qDebug()<<"already open ";
        return true;
    }

    serial = new QSerialPort(nullptr);
    serial->setPortName(portName);
    serial->setBaudRate(static_cast<qint32>(baudRate));
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity) ;
    if (false == serial->open(QIODevice::ReadWrite)){
        qDebug()<<"cant open " << portName <<"@" << baudRate;
        return false;
    }
    serial->flush();
    emit ready();
    qDebug()<<"open " << portName <<"@" << baudRate << "ready";
    return true;

}

int Digitizer_ab::getID() const
{
    return ID;
}

void Digitizer_ab::setID(int value)
{
    ID = value;
}
