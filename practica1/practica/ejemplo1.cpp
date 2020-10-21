#include "ejemplo1.h"

ejemplo1::ejemplo1() : Ui_Counter() {
    setupUi(this);
    qTimer = new QTimer(this);
    this->button->setText("Start");
    show();
    connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(qTimer, SIGNAL(timeout()), this, SLOT(timerScreen()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(doResetButton()));
    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(doSpeedSlider()));
    this->cont = 0;

}

void ejemplo1::doButton() {
    if (qTimer->isActive()) {
        qTimer->stop();
        this->button->setText("Start");
    } else {
        this->button->setText("Stop");
        qTimer->start(1000);
    }
}

void ejemplo1::timerScreen() {
    this->lcdNumber->display(cont++);
}

void ejemplo1::doResetButton() {
    this->cont = 0;
    if (!qTimer->isActive())
        this->lcdNumber->display(cont);
}

void ejemplo1::doSpeedSlider() {
    qTimer->setInterval(speedSlider->sliderPosition());
}

