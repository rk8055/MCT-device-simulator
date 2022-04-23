#ifndef MCT_SIMULATOR_H
#define MCT_SIMULATOR_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QCoreApplication>
#include "recording.h"
#include <QListWidget>
namespace Ui {
class MCT_Simulator;
}

class MCT_Simulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit MCT_Simulator(QWidget *parent = nullptr);
    ~MCT_Simulator();
    void initUI();                          //initialize the simulator, create buttons for the UI, as well as connect slot to signals.


private:
    QString path = "/home/student/Downloads/Winter-COMP3004-FinalProject-main/MCT_device_simulator/"; //change this path to load the image properly!

    void changetoMainMenu();                //Redirect the UI to Main Menu, this stops the timer if its on
    void changetoFrequency();               //Redriect the UI to Frequency page, with 4 options.
    void changetoPrograms();                //Redirect the UI to the progammed page, with 4 options as well.
    void changetoPowerOff();                //Redirect the UI to poweroff page, this is triggered by power button / battery reaching 0 %.
    void changetoHistory();                 //Redirect the UI to the History tab, with view and clear options
    void changetoView();                    //Redirect the UI to View tab, showing recorded treatment
    void askForRecording();                 //Redirect the UI to ask the user if they want to record the next recording
    void setUnhidden();                     //This hide the unneeded rows in the UI, making them unselectable for the user
    void changetoTreatment();               //Redirect the UI to a treatment page with time and power level dislayed
    void changetoSelectPower();           //Redirect the UI to selecting power level
    void updateBattery();                   //This update the battery level

    QString state;                          // This state allows the buttons to detect which state the simulator is at in order to change to correct UI layout
    QString currentTreatment;               // This is used to store into a recording in order for History viewr
    QDateTime date;                         //This saves the date + time for recording
    QTime time;                             //This is the duration of the current treatment
    QTimer timer;

    int downUpper;                          //This int limits the user from moving too much with the Up button
    int upUpper;                            //This int limits the user from moving out of bound with the down button
    int powerLevel = 0;                     //Power level selected by the user
    int highestLevel;                       //This saves the highest power level used in a treatment for recording
    double batteryLevel;                    //
    bool isOn;                              //boolean decides if the simulator is on
    bool recordingToggle;                   //boolean decides if the current treatment will be saved in history

    Ui::MCT_Simulator *ui;
    QListWidget *menu;
    std::vector<Recording*> history;        //This vector of recordings save the recorded treatments





private slots:
    void directionButtonPressed();  //Handles direction buttons
    void okButtonPressed();         //Handles the ok button
    void returnButtonPressed();     //
    void powerButtonPressed();      //
    void menuButtonPressed();       //
    void updateCountDown();         //This gets called when timer releases signal, in order to update the treatment's duration
    void setBattery();              //Slide bar to control the battery
};



#endif // MCT_SIMULATOR_H
