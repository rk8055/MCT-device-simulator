#include "recording.h"


Recording::Recording(QString s, QString t, QTime d, QDateTime da, int p)
{
    state = s;
    treatment = t;
    duration = d;
    date = da;
    powerLevel = p;
}

QString Recording::formatText(){
    QString s =  treatment + " / " + duration.toString("mm:ss") + " / " + date.toString() + " / " + QString::number(powerLevel);
    return s;
}

