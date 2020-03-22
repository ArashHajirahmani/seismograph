#ifndef DIGITIZER_V1_H
#define DIGITIZER_V1_H

#include "digitizer_ab.h"

class Digitizer_V1 : public Digitizer_ab
{
public:
    Digitizer_V1(QObject *parent);
    bool send(const QByteArray *func, const QByteArray *data) override;
    int recive(QByteArray *func, QByteArray *data) override;

private:
    enum {
     PCPON = 1, REQ, POLL, ACK, NACK, ACMSK, TEPON,
     WRUHR, AUHR, QUHR, NUHR, INIT, BKOR, REQTYPE,
     BSZDT = 16, RUHR, IUHR, BMSL, AWOP, SASUM, CMSK,
     LMSK, LFIFO, IMRQA = 32, IMTFS, IMTF,
     WXYZ5DATA, WXYZ2CDATA, WXYZ3CDATA,
     WXYZ4CDATA, WXYZ5CDATA, XAXDATA, YAXDATA, ZAXDATA,
     XYZDATA, XYZ2DATA, XYZ3DATA, XYZ4DATA, XYZ5DATA,
     XYZ2CDATA, XYZ3CDATA, XYZ4CDATA, XYZ5CDATA,
     CALDATA, GETID = 61, HARDID, PRGST,  MAXVALIDFUNC};
    static const char *funcname[];
    static const uchar funclen[];

    typedef void  COMSTAT;
    typedef void* HANDLE;

    char sendfunc(uchar func, uchar te, HANDLE hCom, COMSTAT *comState);
    int recfunc(uchar *func, HANDLE hCom, COMSTAT *comState, uchar *buff);

};

#endif // DIGITIZER_V1_H
