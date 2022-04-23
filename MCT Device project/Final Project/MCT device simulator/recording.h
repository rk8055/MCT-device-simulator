#ifndef RECORDING_H
#define RECORDING_H
#include <QString>
#include <iostream>
#include <QTime>
using namespace std;

class Recording{
public:
    Recording(QString s, QString t, QTime d, QDateTime da, int p);
    ~Recording();
    QString formatText();           //This returns a senctence of string to display each treatment in the history tab
    QString state;                  //All the variables are taken from mct_simulator class during a treatment
    QString treatment;
    QTime duration;
    QDateTime date;
    int powerLevel;

};

#endif // RECORDING_H
