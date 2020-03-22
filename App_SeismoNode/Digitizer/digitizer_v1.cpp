#include "digitizer_v1.h"
#include <QDateTime>

const char *Digitizer_V1::funcname[] ={nullptr, "PCPON", "REQ", "POLL", "ACK", "NACK", "ACMSK", "TEPON",
                                       "WRUHR", "AUHR", "QUHR", "NUHR", "INIT", "BKOR", "REQTYPE",
                                       nullptr, "BSZDT", "RUHR", "IUHR", "BMSL", "AWOP", "SASUM", "CMSK", "LMSK", "LFIFO",
                                       nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "IMRQA", "IMTFS", "IMTF",
                                       "WXYZ5DATA", "WXYZ2CDATA", "WXYZ3CDATA", "WXYZ4CDATA", "WXYZ5CDATA", "XAXDATA",
                                       "YAXDATA", "ZAXDATA", "XYZDATA", "XYZ2DATA", "XYZ3DATA", "XYZ4DATA", "XYZ5DATA",
                                       "XYZ2CDATA", "XYZ3CDATA", "XYZ4CDATA", "XYZ5CDATA", "CALDATA", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                       nullptr, nullptr, "GETID = 61", "HARDID", "PRGST", " MAXVALIDFUNC"};

const uchar funclen[] = {
    0,   3,   3,  3,  3,  3,  3, 23,   //  0  7
    9,   3,   3,  3, 26, 27,  4,  0,   //  8 15
    16,   3,  17, 27, 60,  9,  6, 36,   // 16 23
    4,  11,   7, 44, 93, 18, 54, 12,   // 24 31
    7, 134, 134, 44, 16, 20, 24, 28,   // 32 39
    6,   6,   6, 10, 16, 22, 28, 34,   // 40 47
    13,  16,  19, 22,  0,  0,  0,  0,   // 48 55
    0,   0,   0,  0,  0,  3,  7,  35}; // 56 63

Digitizer_V1::Digitizer_V1(QObject *parent):Digitizer_ab(parent)
{

}

bool Digitizer_V1::send(const QByteArray *func, const QByteArray *data)
{

}

int Digitizer_V1::recive(QByteArray *func, QByteArray *data)
{

}

char Digitizer_V1::sendfunc(uchar func, uchar te, Digitizer_V1::HANDLE hCom, Digitizer_V1::COMSTAT *comState)
{
    int i, len, sum;
    struct timedate *td;
    time_t tp;
    uchar buff[256];

    time(&tp);

    len = funclen[func];
    buff[0] = te;
    buff[1] = func;
    switch(func)
    {
    case  WRUHR:
      td = gettime((long) tp, 1);
      memcpy((buff + 2), td, (size_t) 6);
      break;
    case  RUHR:
      td = gettime((long) tp, 1);
      memcpy((buff + 2), td, (size_t) 6);
      break;
    case  BMSL:
      //memcpy((buff + 4), "MESSAGE FOR CARD", (size_t) 16);
      break;
    case REQ:
      break;
    default:
      for(i = 2; i < (len - 1); i++) buff[i] = rand();
      break;
    }
    if(len > 2)
    {
      sum = te + func;
      if(sum >= BYTEMODULO) sum -= BYTEMODULO;
      for(i = 2; i < (len - 1); i++)
      {
        sum += buff[i];
        if(sum >= BYTEMODULO) sum -= BYTEMODULO;
      }
      buff[len - 1] = BYTEMODULO - sum;
      //if(flags & DUMMYCONSOLE)
      //{
      //  printf("Send Function ");
      //  for(i = 0; i < len; i++) printf(" %d", buff[i]);
      //  printf("\n");
      //}
    }
    writeser(len, hCom, comState, buff);
    //serialiostatus &= ~SENDBUSY;
    return len;
}

int Digitizer_V1::recfunc(uchar *func, Digitizer_V1::HANDLE hCom, Digitizer_V1::COMSTAT *comState, uchar *buff)
{
    DWORD dwRead;
    short check;
    int  len, ret;
    if(serialiostatus & RECBUSY) return -4;
    serialiostatus |= RECBUSY;
    memset(buff, 0, 100);
    if(comWaitForData(2, (DEFAULT_TIMEOUT * 2), hCom, comState) >= 2)
    {
      int i;

      if(ReadFile(hCom, buff, 2, &dwRead, &OverlappedIn))
      {
        //if(flags & DUMMYCONSOLE) printf("recfunc ");
   #if(DEBUGMODE > 1)
        if(fpd) fprintf(fpd, "recfunc %x, %x\n", buff[0], buff[1]);
   #endif
        if(buff[1] > 0 && buff[1] < MAXVALIDFUNC)
        {
          *func = buff[1];
          len = funclen[*func];
          if(len > 2)
          {
            Sleep(((len - 2) / 22) + 1);
            if(comWaitForData(len - 2, ((len - 2) * DEFAULT_TIMEOUT), hCom, comState) >= len - 2)
            {
              if(ReadFile(hCom, &buff[2], len - 2, &dwRead, &OverlappedIn))
              {
                check = 0;
                for(i = 0; i < len; i++)
                {
                  check += buff[i];
                  //if(flags & DUMMYCONSOLE) printf("%02x, ", buff[i]);
   #if(DEBUGMODE > 1)
                  if(fpd) fprintf(fpd, "%02x, ", buff[i]);
   #endif
                  if(check >= BYTEMODULO) check -= BYTEMODULO;
                }
                //if(flags & DUMMYCONSOLE) printf("  checksum = %d\n", check);
   #if(DEBUGMODE > 1)
                if(fpd) fprintf(fpd, "  checksum = %d\n", check);
   #endif
                if(check == 0) ret = len;
                else ret = -7;
              }
              else ret = -6;
            }
            else ret = -5;
          }
          else ret = -4;
        }
        else ret = -3;
      }
      else ret = -2;
    }
    else ret = -1;
    CancelIo(hCom);
    serialiostatus &= ~RECBUSY;
    return ret;
}
