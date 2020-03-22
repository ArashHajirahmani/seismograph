#ifndef DIGITIZER_DEMONS_H
#define DIGITIZER_DEMONS_H

#include <QObject>
#include <QList>
#include <QThread>
#include "digitizer_ab.h"

class Digitizer_Demons : public QObject
{
    Q_OBJECT
public:
    explicit Digitizer_Demons(QObject *parent = nullptr);
    int addDigitizer(const Digitizer_ab *dg); //returns id
    void remDigitizer(Digitizer_ab *dg);
    void remDigitizer(int id);
    bool start(Digitizer_ab * dg);
    bool start(const int num);
    bool startAll(); //starts All
    bool stop(Digitizer_ab * dg);
    bool stop(const int num);
    bool stopAll(); //stop All
    Digitizer_Demons *getInstant() const;

private:
    static Digitizer_Demons *instant;
    QList<Digitizer_ab *> digitizers;
    QList<QThread *> workerThreads ;

signals:
    void dgEesult(const int id, const QString cmd, const QString res);
    void dgErrorStr(const int id, const QString err);
    void dgready(const int id);
    void dgDead(const int id);

private slots:
    void onDgResult(const int id, const QString cmd, const QString res);
    void onDgError(const int id, const QString err);
    void onDgReady(const int id);
    void onDgDead(const int id);


};

#endif // DIGITIZER_DEMONS_H
