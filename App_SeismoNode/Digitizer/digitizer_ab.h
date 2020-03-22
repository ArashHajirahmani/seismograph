#ifndef DIGITIZER_AB_H
#define DIGITIZER_AB_H

#include <QObject>

class QSerialPort;
class Digitizer_ab : public QObject
{
    Q_OBJECT
public:
    explicit Digitizer_ab(QObject *parent = nullptr);
    ~Digitizer_ab(void);
    virtual void init(void)=0;
    virtual void doPool(void);
    virtual bool send(const QByteArray *func, const QByteArray *data);
    virtual int recive(QByteArray *func, QByteArray *data);
    virtual bool initSerial(QString portName, uint32_t boudRate);
    int getID() const;
    void setID(int value);

private:
    QSerialPort *serial;
    int ID; // id, invalid is -1
signals:
     void ready();
public slots:

};

#endif // DIGITIZER_AB_H
