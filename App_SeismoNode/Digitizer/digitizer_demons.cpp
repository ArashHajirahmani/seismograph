#include "digitizer_demons.h"

Digitizer_Demons *Digitizer_Demons::instant = nullptr;
Digitizer_Demons::Digitizer_Demons(QObject *parent) : QObject(parent)
{
 if (nullptr == instant )
     instant = new  Digitizer_Demons(parent);
}

Digitizer_Demons *Digitizer_Demons::getInstant() const
{
    return instant;
}
