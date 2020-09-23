#include "ejemplo1.h"

ejemplo1::ejemplo1() : Ui_Counter() {
    setupUi(this);
    qTimer = new QTimer(this);
    this->button->setText("Start");
    show();
    connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(qTimer, SIGNAL(timeout()), this, SLOT(timerScreen()));


}

void ejemplo1::doButton() {
    if (qTimer->isActive()) {
        qTimer->stop();
        this->button->setText("Start");
    } else{
        this->button->setText("Stop");
        qTimer->start(1000);
    }
}

void ejemplo1::timerScreen() {
    static int cont = 0;
    this->lcdNumber->display(cont++);
}



