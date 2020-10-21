#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
	connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(doSpinBox()));
	mytimer.connect(std::bind(&ejemplo1::cuenta, this));
    mytimer.start(1000);
}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
	if(stopped)
		mytimer.stop();
	else
		mytimer.start(mytimer.getPeriod());
	qDebug() << "click on button";
}

void ejemplo1::doSpinBox()
{
    qDebug() << this->mytimer.getPeriod();
    mytimer.setPeriod(1000/doubleSpinBox->value());
    qDebug() << this->mytimer.getPeriod();
}

void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
	trick++;
}

