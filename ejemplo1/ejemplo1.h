#ifndef ejemplo1_H
#define ejemplo1_H

#include <QtGui>
#include "ui_counterDlg.h"

class ejemplo1 : public QWidget, public Ui_Counter {
Q_OBJECT
private:
    int cont;
public:
    ejemplo1();
    QTimer *qTimer;


public slots:

    void doButton();
    void timerScreen();
    void doResetButton();
    void doSpeedSlider();
};

#endif // ejemplo1_H
