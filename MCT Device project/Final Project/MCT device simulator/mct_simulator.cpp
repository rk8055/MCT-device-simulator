#include "mct_simulator.h"
#include "ui_mct_simulator.h"


MCT_Simulator::MCT_Simulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MCT_Simulator)

{
    ui->setupUi(this);
    initUI();

}

MCT_Simulator::~MCT_Simulator()
{
    delete ui;
}

//This function sets the icons for all the buttons and sets the display into default (Poweroff)
void MCT_Simulator::initUI()
{
    menu = ui->listWidget;
    time.setHMS(0, 4, 0);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateCountDown()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(directionButtonPressed()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(directionButtonPressed()));
    connect(ui->okButton, SIGNAL(released()), this, SLOT(okButtonPressed()));
    connect(ui->returnButton, SIGNAL(released()), this, SLOT(returnButtonPressed()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(powerButtonPressed()));
    connect(ui->menuButton, SIGNAL(released()), this, SLOT(menuButtonPressed()));
    connect(ui->rightButton, SIGNAL(released()), this, SLOT(directionButtonPressed()));
    connect(ui->leftButton, SIGNAL(released()), this, SLOT(directionButtonPressed()));
    connect(ui->batterySlider,SIGNAL(valueChanged(int)),this,SLOT(setBattery()));

    isOn = false;
    batteryLevel = 100;
    changetoPowerOff();

    QPixmap pixmap(path + "left_icon.png");
    ui->leftButton->setIcon(QIcon(pixmap));
    ui->leftButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "right_icon.png");
    ui->rightButton->setIcon(QIcon(pixmap));
    ui->rightButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "down_icon.png");
    ui->downButton->setIcon(QIcon(pixmap));
    ui->downButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "up_icon.png");
    ui->upButton->setIcon(QIcon(pixmap));
    ui->upButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "ok_icon.png");
    ui->okButton->setIcon(QIcon(pixmap));
    ui->okButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "return_icon.png");
    ui->returnButton->setIcon(QIcon(pixmap));
    ui->returnButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "menu_icon.png");
    ui->menuButton->setIcon(QIcon(pixmap));
    ui->menuButton->setIconSize(QSize(30, 30));

    pixmap.load(path + "power_icon.png");
    ui->powerButton->setIcon(QIcon(pixmap));
    ui->powerButton->setIconSize(QSize(30, 30));

}

void MCT_Simulator::menuButtonPressed()
{
    if (isOn == true){
        changetoMainMenu();
        timer.stop();
    }
}

void MCT_Simulator::directionButtonPressed()
{
    if (isOn){
        QPushButton *button = (QPushButton*) sender();
        if(button->objectName() == "downButton" && ui->listWidget->currentRow() != downUpper && ui->listWidget->currentRow() != -1)
        {
            int rowNum = ui->listWidget->currentRow();
            ui->listWidget->setCurrentRow(rowNum+1);
        }

        else if (button->objectName() == "upButton" && ui->listWidget->currentRow() != upUpper && ui->listWidget->currentRow() != 1)
        {
            int rowNum = ui->listWidget->currentRow();
            ui->listWidget->setCurrentRow(rowNum-1);
        }
        else if (button->objectName() == "leftButton" && powerLevel != 0 && (state == "Frequency Treatment" || state == "Programs Treatment"))
        {
            powerLevel--;
            menu->item(5)->setText("Power : " + QString::number(powerLevel));
        }
        else if (button->objectName() == "rightButton" && powerLevel != 100 && (state == "Frequency Treatment" || state == "Programs Treatment"))
        {
            powerLevel++;
            menu->item(5)->setText("Power : " + QString::number(powerLevel));

            if (highestLevel < powerLevel){
                highestLevel = powerLevel;
            }
        }
    }

}

void MCT_Simulator::powerButtonPressed()
{
    if (menu->item(2)->text() == "POWER OFF" && isOn == false && batteryLevel > 0) {
        isOn = true;
        changetoMainMenu();
    }else{
        isOn = false;
        timer.stop();
        changetoPowerOff();
    }
}

void MCT_Simulator::returnButtonPressed()
{
    if (isOn){
        if (menu->item(0)->text() == "Frequency" || menu->item(0)->text() == "Programs" || menu->item(0)->text() == "History"){
            changetoMainMenu();
        }else if (menu->item(0)->text() == "Rcord this treatment?")
        {
             if (state == "Frequency"){changetoFrequency();}
            else if (state == "Programs") {changetoPrograms();}
        }else if (state == "Frequency Treatment")
        {
            if (recordingToggle == true){
            history.push_back(new Recording(state, currentTreatment, time, date, powerLevel));
            }
            changetoFrequency();


        }else if (state == "Programs Treatment")
        {
            if (recordingToggle == true){
            QTime total;
            total.setHMS(0, 5, 0);
            int sec = time.secsTo(total);
            time.setHMS(0, 0, 0);
            time = time.addSecs(sec);
            Recording* r = new Recording(state, currentTreatment, time, date, powerLevel);
            history.push_back(r);
            }

            changetoPrograms();
        }
        else if ( menu->item(0)->text() == "Treatment / Duration / Date / Power"){changetoHistory();}
        timer.stop();
    }

}

void MCT_Simulator::okButtonPressed()
{
    QString temp = menu->currentItem()->text();
    if (isOn == true){
        if (temp == "Frequency"){
            changetoFrequency();
        }else if (temp == "Programs"){
            changetoPrograms();
        }else if ((state == "Frequency" || state =="Programs") && menu->item(0)->text() != "Rcord this treatment?"){
            currentTreatment = menu->currentItem()->text();
            askForRecording();
        }else if (temp == "Yes"){
            recordingToggle = true;
            changetoSelectPower();
        }else if (temp == "No"){
            recordingToggle = false;
            changetoSelectPower();
        }
        else if ((state == "Frequency Treatment" || state == "Programs Treatment") && powerLevel > 0){
            date = date.currentDateTime();
            changetoTreatment();
        }else if (temp == "History"){
            changetoHistory();
        }else if (temp == "View"){
            changetoView();
        }else if (temp == "Clear"){
            history.clear();
        }
    }
}

void MCT_Simulator::updateCountDown() //This gets called whenever the timer signals once!
{
    QListWidget *menu = ui->listWidget;
    if(ui->contactBox->isChecked() == true){
        if (state == "Frequency Treatment"){
            time = time.addSecs(+1);
        }else {
            time = time.addSecs(-1);
        }

        menu->item(2)->setText(time.toString("m:ss"));
        updateBattery();
    }

}


void MCT_Simulator::updateBattery(){        //This is called in the updateCountDown() to change to UI shown for battery level
        batteryLevel -= powerLevel * 0.1;
        QLabel *batteryText = ui->batteryText;

        if (batteryLevel  <= 0){
            batteryLevel = 0;
            timer.stop();
            changetoPowerOff();
        }
        batteryText->setText(QString::number(batteryLevel) + "%");
        ui->batterySlider->setValue(batteryLevel);
}
void MCT_Simulator::setBattery(){           //This changes the UI element of the battery
    batteryLevel = ui->batterySlider->value();
    QLabel *batteryText = ui->batteryText;
    batteryText->setText(QString::number(batteryLevel) + "%");
    if(batteryLevel == 0.0){
        changetoPowerOff();
    }
}
void MCT_Simulator::changetoView()
{
    upUpper = 1;
    downUpper = 1;
    menu->item(0)->setText("Treatment / Duration / Date / Power");
    menu->item(1)->setText("");
    menu->item(2)->setText("");
    menu->setCurrentRow(1);
    for (int i = 0; i < int(history.size()); i ++){
        downUpper = i+1;

        menu->item(i+1)->setText(history.at(i)->formatText());
    }
}
void MCT_Simulator::changetoMainMenu()
{
    state = "Main";
    setUnhidden();
    downUpper = 3;
    upUpper = 1;
    menu->item(0)->setText("Main Menu");
    menu->item(1)->setText("Programs");
    menu->item(2)->setText("Frequency");
    menu->item(3)->setText("History");
    menu->item(4)->setText("");
    menu->item(5)->setText("");
    menu->setCurrentRow(1);
}

void MCT_Simulator::changetoFrequency()
{
    state = "Frequency";
    setUnhidden();
    menu->item(0)->setText("Frequency");
    menu->item(1)->setText("10Hz");
    menu->item(2)->setText("20Hz");
    menu->item(3)->setText("125Hz");
    menu->item(4)->setText("7720");
    menu->item(5)->setText("");
    menu->setCurrentRow(1);

    menu->item(3)->setHidden(false);
    menu->item(4)->setHidden(false);
}

void MCT_Simulator::changetoPrograms()
{
    state = "Programs";
    setUnhidden();
    menu->item(0)->setText("Programs");
    menu->item(1)->setText("Allergy");
    menu->item(2)->setText("Pain");
    menu->item(3)->setText("Bloating");
    menu->item(4)->setText("Dystonia");
    menu->item(5)->setText("");
    menu->setCurrentRow(1);


}

void MCT_Simulator::changetoHistory()
{
     upUpper = 1;
     downUpper = 2;
     menu->item(0)->setText("History");
     menu->item(1)->setText("View");
     menu->item(2)->setText("Clear");
     menu->item(3)->setText("");
     menu->setCurrentRow(1);
}

void MCT_Simulator::changetoSelectPower()
{
     powerLevel = 0;
     highestLevel = 0;
     upUpper = 5;
     downUpper = 5;
     state = state + " Treatment";
     qInfo() << "Current State " + state;
     menu->item(0)->setText(currentTreatment);
     menu->item(1)->setText("");

     if (state == "Frequency Treatment"){
         time.setHMS(0, 0 ,0);
     }else if (state == "Programs Treatment"){
         time.setHMS(0, 5, 0);
     }
     menu->setCurrentRow(5);
     menu->item(2)->setText(time.toString("m:ss"));
     menu->item(3)->setHidden(false);
     menu->item(3)->setText("");
     menu->item(4)->setHidden(false);
     menu->item(4)->setText("");
     menu->item(5)->setText("Power : " + QString::number(powerLevel));
}

void MCT_Simulator::changetoTreatment()
{
    menu->item(0)->setText(currentTreatment);
    menu->item(1)->setText("");
    timer.start(1000);
}
void MCT_Simulator::changetoPowerOff()
{
    isOn = false;
    upUpper = 2;
    downUpper = 2;
    menu->item(0)->setText("");
    menu->item(1)->setText("");
    menu->item(2)->setText("POWER OFF");
    menu->item(3)->setText("");
    menu->item(4)->setText("");
    menu->item(5)->setText("");
    menu->setCurrentRow(2);

}

void  MCT_Simulator::askForRecording()
{
    menu->setCurrentRow(1);
    qInfo() << "Current Treatment" + currentTreatment;
    downUpper = 2;
    menu->item(0)->setText("Rcord this treatment?");
    menu->item(1)->setText("Yes");
    menu->item(2)->setText("No");
    menu->item(3)->setHidden(true);
    menu->item(4)->setHidden(true);
}

void MCT_Simulator::setUnhidden()
{
    menu->item(1)->setHidden(false);
    menu->item(2)->setHidden(false);
    menu->item(3)->setHidden(false);
    menu->item(4)->setHidden(false);
    downUpper = 4;
}




