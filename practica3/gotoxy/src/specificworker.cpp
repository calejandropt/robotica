/*
 *    Copyright (C) 2020 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"
#include <Eigen/Dense>

template <typename T>
struct Target
{
    T content;
    std::mutex my_mutex;
    bool activate = false;

    void put(const T &data)
    {
        std::lock_guard<std::mutex> guard(my_mutex);
        content = data;   // generic type must be copy-constructable
        activate = true;
    }
    std::optional<T> get()
    {
        std::lock_guard<std::mutex> guard(my_mutex);
        if(activate)
            return content;
        else
            return {};
    }
    void set_task_finished()
    {
        std::lock_guard<std::mutex> guard(my_mutex);
        activate = false;
    }
};
Target<Eigen::Vector2f> target;


/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
	this->startup_check_flag = startup_check;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
//	THE FOLLOWING IS JUST AN EXAMPLE
//	To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
//	try
//	{
//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//		std::string innermodel_path = par.value;
//		innerModel = std::make_shared(innermodel_path);
//	}
//	catch(const std::exception &e) { qFatal("Error reading config params"); }






	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

void SpecificWorker::compute()
{
    const float threshold = 150;

    // Obtener posicion del robot
    RoboCompGenericBase::TBaseState bState;
    //Obtener estado del mapa base
    differentialrobot_proxy->getBaseState(bState);

    //Actualizar datos del mapa base
    innerModel->updateTransformValues("base", bState.x, 0, bState.z, 0, bState.alpha, 0);

    //Posicion del robot target
    QVec p = innerModel->transform("base", QVec::vec3(target.x(), 0, target.z()), "world");

    // Obtener angulo y distancia
    float rotAngle = atan2(p.x(), p.z());
    float dir = p.norm2();
    if (target.getStatus())
    {
        //Si está en el sitio se para y cambia el estado
        if (dir < threshold)
        {
            differentialrobot_proxy->setSpeedBase(0, 0);
            target.toogleStatus();
        }
            //Si está en un ángulo distinto, se gira
        else if (std::abs(rotAngle) > .1)
        {
            differentialrobot_proxy->setSpeedBase(0, rotAngle);
        }
            //Si está en la dirección del sitio, aumenta la velocidad
        else
        {
            differentialrobot_proxy->setSpeedBase(300, 0);
        }
    }
}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
}


//SUBSCRIPTION to setPick method from RCISMousePicker interface
void SpecificWorker::RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick)
{
//subscribesToCODE

}



/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData

/**************************************/
// From the RoboCompRCISMousePicker you can use this types:
// RoboCompRCISMousePicker::Pick

