/*
 * Copyright (c) 2015 Jon Escombe (jone@dresco.co.uk)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <QtDebug>
#include "CalibrationData.h"
#include "ANT.h"
#include "ANTChannel.h"
#include "RealtimeController.h"
#include "ANTlocalController.h"

QList<QString> CalibrationData::emptyMessageList;
TrainSidebar* CalibrationData::trainSidebar = NULL;

CalibrationData::CalibrationData(RealtimeController* realtimeController) : realtimeController(realtimeController), parentCalibrationData(NULL)
{
    device=CALIBRATION_TYPE_UNKNOWN;
    supported=CALIBRATION_TYPE_UNKNOWN;
    inProgress=CALIBRATION_TYPE_UNKNOWN;
    completed=CALIBRATION_TYPE_NONE;
    state=CALIBRATION_STATE_IDLE;
    targetSpeed=0.0;
    messageIndex = 0;
    if (realtimeController)
        name = realtimeController->name();
}

CalibrationData::CalibrationData(CalibrationData* parentCalibrationData) : realtimeController(NULL), parentCalibrationData(parentCalibrationData)
{
    device=CALIBRATION_TYPE_UNKNOWN;
    supported=CALIBRATION_TYPE_UNKNOWN;
    inProgress=CALIBRATION_TYPE_UNKNOWN;
    completed=CALIBRATION_TYPE_NONE;
    state=CALIBRATION_STATE_IDLE;
    targetSpeed=0.0;
    messageIndex = 0;
    if (parentCalibrationData) {
        parentCalibrationData->childsCalibrationData.append(this);
        name = parentCalibrationData->getName(); // will be updated later with sub device name
    }
}

CalibrationData::~CalibrationData()
{
    if (parentCalibrationData) {
        parentCalibrationData->childsCalibrationData.removeOne(this);
    }
}
    
uint8_t CalibrationData::getCalibrationDevice()
// the device that is under calibration process (if started) otherwise the first one with calibration support
{
    uint8_t ret_val = CALIBRATION_DEVICE_NONE;

    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList()) {
            if (ret_val)
                break;
            else if (x.controller && x.controller->calibrationData)
                ret_val |= x.controller->calibrationData->getDevice();
        }

    return ret_val;
}

uint8_t CalibrationData::getCalibrationSupported()
// list all calibration that are supported  (by each realtime controllers)
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
                ret_val |= x.controller->calibrationData->getSupported();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationInProgress()
// list all calibration that are in progress
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
                ret_val |= x.controller->calibrationData->getInProgress();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationCompleted()
// list all calibration that are completed
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
                ret_val |= x.controller->calibrationData->getCompleted();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationState()
{
    uint8_t ret_val = CALIBRATION_STATE_IDLE;

    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList()) {
            if (ret_val)
                break;
            else if (x.controller && x.controller->calibrationData)
                ret_val = x.controller->calibrationData->getState();
        }

    return ret_val;
}

void CalibrationData::startCalibration(uint8_t device) {
    qDebug() << "Start calibration...";
    if (!trainSidebar) {
        qDebug() << "ERROR: trainSidebar not defined";
        return;
    }

    if (getCalibrationState()==CALIBRATION_STATE_IDLE)
        foreach(DeviceConfiguration x, trainSidebar->devicesList()) {
            if (x.controller && x.controller->calibrationData) {
                qDebug() << "... device set" << x.controller->name() << "... controller" << x.controller->name() << x.controller->calibrationData->getDevice() 
                         << "supported" << x.controller->calibrationData->getSupported() 
                         << "completed" << x.controller->calibrationData->getCompleted();
                if ((x.controller->calibrationData->getDevice() & device) &&
                    (x.controller->calibrationData->getSupported() & ~x.controller->calibrationData->getCompleted())) {

                    uint8_t calibrationType = 0x01;
                    while (!(calibrationType & x.controller->calibrationData->getSupported() & ~x.controller->calibrationData->getCompleted()))
                        calibrationType = calibrationType<<1;
                    qDebug() << "Start " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData->getDevice()); 
                    x.controller->calibrationData->start(calibrationType);
                    break;
                }
            }
            else
                qDebug() << "... device" << x.name << "does not have active controller";
        }


    if (getCalibrationState()==CALIBRATION_STATE_IDLE)
    {
        qDebug() << "force calibration...";
        if (trainSidebar)
            foreach(DeviceConfiguration x, trainSidebar->devicesList())
                if (x.controller && x.controller->calibrationData)
                {
                    qDebug() << "... device" << x.controller->calibrationData->getDevice() << "supported" << x.controller->calibrationData->getSupported() << "completed" << x.controller->calibrationData->getCompleted();
                    if ((x.controller->calibrationData->getDevice() & device) &&
                        (x.controller->calibrationData->getSupported()))
                    {
                        uint8_t calibrationType = 0x01;
                        while (!(calibrationType & x.controller->calibrationData->getSupported()))
                            calibrationType = calibrationType<<1;
                        qDebug() << "Force " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData->getDevice()); 
                        x.controller->calibrationData->force(calibrationType);
                        break;
                    }
                }
    }
}

void CalibrationData::resetCalibrationProcess()
{
    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if ((x.controller) && (x.controller->calibrationData->getState()!=CALIBRATION_STATE_IDLE))
                x.controller->calibrationData->resetProcess();
}

QString CalibrationData::getCalibrationMessage()
{
    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
            {
                QList<QString> messageList = x.controller->calibrationData->getMessageList();
                int8_t index = x.controller->calibrationData->getMessageIndex();
                if (index<messageList.size())
                    return messageList[index];
            }

    return "";
}

const QList<QString>& CalibrationData::getCalibrationMessageList()
{
    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
            {
                if (x.controller->calibrationData->getMessageList().size())
                    return  x.controller->calibrationData->getMessageList();
            }

    return CalibrationData::emptyMessageList;
}

uint8_t CalibrationData::getCalibrationMessageIndex()
{
    if (trainSidebar)
        foreach(DeviceConfiguration x, trainSidebar->devicesList())
            if (x.controller && x.controller->calibrationData)
            {
                QList<QString> messageList = x.controller->calibrationData->getMessageList();
                int8_t index = x.controller->calibrationData->getMessageIndex();
                if (index)
                    return index;
            }

    return 0;
}

QString CalibrationData::typeDescr(uint8_t param_type)
{
    switch(param_type) {
        case CALIBRATION_TYPE_UNKNOWN:
            return qPrintable("UNKNOWN");
            break;
        case CALIBRATION_TYPE_COMPUTRAINER:
            return qPrintable("COMPUTRAINER");
            break;
        case CALIBRATION_TYPE_ZERO_OFFSET:
            return qPrintable("ZERO_OFFSET");
            break;
        case CALIBRATION_TYPE_SPINDOWN:
            return qPrintable("SPINDOWN");
            break;
        default:
            return qPrintable("UNKNOWN");
    }
}

QString CalibrationData::deviceDescr(uint8_t param_device)
{
    switch(param_device) {
        case CALIBRATION_DEVICE_NONE:
            return qPrintable("NONE");
        case CALIBRATION_DEVICE_COMPUTRAINER:
            return qPrintable("COMPUTRAINER");
        case CALIBRATION_DEVICE_ANT_SPORT:
            return qPrintable("ANT_SPORT");
        case CALIBRATION_DEVICE_ANT_FEC:
            return qPrintable("ANT FEC");
        case CALIBRATION_DEVICE_ALL:
            return qPrintable("ALL");
        default:
            return qPrintable("UNKNOWN");
    }
}

QString CalibrationData::stateDescr(uint8_t param_state)
{
    switch(param_state) {
        case CALIBRATION_STATE_IDLE:
            return qPrintable("IDLE");
        case CALIBRATION_STATE_REQUIRED:
            return qPrintable("REQUIRED");
        case CALIBRATION_STATE_REQUESTED:
            return qPrintable("REQUESTED");
        case CALIBRATION_STATE_STARTING:
            return qPrintable("STARTING");
        case CALIBRATION_STATE_STARTED:
            return qPrintable("STARTED");
        case CALIBRATION_STATE_SPEEDUP:
            return qPrintable("SPEEDUP");
        case CALIBRATION_STATE_COAST:
            return qPrintable("COAST");
        case CALIBRATION_STATE_SUCCESS:
            return qPrintable("SUCCESS");
        case CALIBRATION_STATE_FAILURE:
            return qPrintable("FAILURE");
        case CALIBRATION_STATE_ABORT:
            return qPrintable("ABORT");
        default:
            return qPrintable("UNKNOWN");
    }
}

void     CalibrationData::setDevice(uint8_t device)
{
    this->device=device;
}

uint8_t  CalibrationData::getDevice() const
{
    if (childsCalibrationData.size()) {
        // first find the device which calibration is in progress
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getDevice() && x->getInProgress())
                return x->getDevice();
        // otherwise return the first device found
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getDevice())
                return x->getDevice();
        return CALIBRATION_DEVICE_NONE;
    }
    else
        return device;
}

void     CalibrationData::setSupported(uint8_t type)
{
    supported=type;
}

uint8_t  CalibrationData::getSupported() const
{
    if (childsCalibrationData.size()) {
        uint8_t supported=CALIBRATION_TYPE_NONE;
        foreach (CalibrationData* x, childsCalibrationData) {
            supported |= x->getSupported();
        }

        qDebug() << qPrintable("CalibrationData::getSupported() of device " + name + QString(" output: 0x") + QString("%1").arg(supported, 2, 16, QLatin1Char( '0' )).toUpper());
        return supported;
    }
    else {
        qDebug() << qPrintable("CalibrationData::getSupported() of device " + name + QString(" output: 0x") + QString("%1").arg(supported, 2, 16, QLatin1Char( '0' )).toUpper());
        return supported;
    }
}

void     CalibrationData::setInProgress(uint8_t type)
{
    inProgress=type;
}

uint8_t  CalibrationData::getInProgress() const
{
    if (childsCalibrationData.size()) {
        uint8_t inProgress=CALIBRATION_TYPE_NONE;
        foreach (CalibrationData* x, childsCalibrationData)
            inProgress |= x->getInProgress();

        return inProgress;
    }
    else
    return inProgress;
}

void     CalibrationData::setCompleted(uint8_t type)
{
    completed=type;
}
uint8_t  CalibrationData::getCompleted() const
{
    if (childsCalibrationData.size()) {
        uint8_t completed=CALIBRATION_TYPE_NONE;
        uint8_t notCompleted=CALIBRATION_TYPE_NONE;
        foreach (CalibrationData* x, childsCalibrationData) {
            completed    |= x->getCompleted();
            notCompleted |= x->getSupported() & ~x->getCompleted();
        }

        return completed & ~notCompleted;
    }
    else
        return completed;
}

void     CalibrationData::setState(uint8_t state)
{
    if (this->state!=state)
    {
        qDebug() << "Calibration step changing from " << typeDescr(this->state) << "to" << typeDescr(state);

        stepTimestamp = QTime::currentTime();

        if (state==CALIBRATION_STATE_IDLE)
        {
            messageIndex=0;
            messageList.clear();
            inProgress = CALIBRATION_TYPE_NONE;
        }

        if (state==CALIBRATION_STATE_REQUESTED)
        {
            messageIndex=0;
            messageList.clear();
            switch (type)
            {
              case CALIBRATION_TYPE_COMPUTRAINER:
                messageList.append(QObject::tr("starting"));
                messageList.append(QObject::tr("Calibrating...\nPress F3 on Controller when done."));
                messageList.append(QObject::tr("done"));
                break;
              case CALIBRATION_TYPE_ZERO_OFFSET:
                messageList.append(QObject::tr("starting"));
                messageList.append(QObject::tr("remove foot from pedals"));
                messageList.append(QObject::tr("done"));
                break;
              case CALIBRATION_TYPE_SPINDOWN:
                messageList.append(QObject::tr("starting"));
                messageList.append(QObject::tr("speedup to 30kph"));
                messageList.append(QObject::tr("Stop pedalling until process is completed"));
                messageList.append(QObject::tr("done"));
                break;
              case CALIBRATION_TYPE_CONFIGURATION:
                messageList.append(QObject::tr("starting"));
                messageList.append(QObject::tr("configuring"));
                messageList.append(QObject::tr("done"));
                break;

            }
        }

        if (state==CALIBRATION_STATE_STARTED)
        {
            switch (type)
            {
              case CALIBRATION_TYPE_COMPUTRAINER:
                messageIndex=1;
                break;

            }
        }

        if (state==CALIBRATION_STATE_SPEEDUP)
        {
            switch (type)
            {
              case CALIBRATION_TYPE_SPINDOWN:
                messageIndex=1;
                break;

            }
        }

        if (state==CALIBRATION_STATE_COAST)
        {
            switch (type)
            {
              case CALIBRATION_TYPE_SPINDOWN:
                messageIndex=2;
                break;
              case CALIBRATION_TYPE_ZERO_OFFSET:
                messageIndex=1;
                break;

            }
        }

        if (state==CALIBRATION_STATE_SUCCESS || state==CALIBRATION_STATE_FAILURE)
        {
            switch (type)
            {
              case CALIBRATION_TYPE_COMPUTRAINER:
                messageIndex=2;
                break;
              case CALIBRATION_TYPE_SPINDOWN:
                messageIndex=3;
                break;
              case CALIBRATION_TYPE_ZERO_OFFSET:
              case CALIBRATION_TYPE_CONFIGURATION:
                messageIndex=2;
                break;

            }
        }
    }

    this->state=state;
}

uint8_t  CalibrationData::getState()
{
    if (childsCalibrationData.size()) {
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getState())
                    return x->getState();

        return CALIBRATION_STATE_IDLE;
    }
    else {
        if ((state==CALIBRATION_STATE_SUCCESS || state==CALIBRATION_STATE_FAILURE || state==CALIBRATION_STATE_ABORT)
            &&  (stepTimestamp.secsTo(QTime::currentTime())>2)) 
        {
            // keep calibration completion state during few seconds then go back to IDLE mode (will allow to keep a message shown on screen)
            setState(CALIBRATION_STATE_IDLE);
            setInProgress(CALIBRATION_TYPE_NONE);
        }

        return state;
    }
}

void     CalibrationData::setTargetSpeed(double target_speed)
{
    targetSpeed = target_speed;

    switch (inProgress)
    {
      case CALIBRATION_TYPE_SPINDOWN:
        messageList[1] = QString(QObject::tr("speedup to %1 kph")).arg(targetSpeed, 0, 'f', 1);
        break;
    }
}

void     CalibrationData::start(uint8_t type)
{
    if (childsCalibrationData.size()) {
        if (getState()==CALIBRATION_STATE_IDLE)
            foreach (CalibrationData* x, childsCalibrationData)
                if (x->getSupported() & ~x->getCompleted() & type)
                        x->start(type);
    }
    else {
        setState(CALIBRATION_STATE_REQUESTED);
        attempts = 0;
        this->type=type;
    }
}

void     CalibrationData::force(uint8_t type)
{
    if (childsCalibrationData.size()) {
        if (getState()==CALIBRATION_STATE_IDLE)
            foreach (CalibrationData* x, childsCalibrationData)
                if (x->getSupported() & type)
                    x->force(type);
    }
    else
        start(type);
}

void     CalibrationData::resetProcess()
{
    if (childsCalibrationData.size()) {
        foreach (CalibrationData* x, childsCalibrationData)
            x->resetProcess();
    }
    else {
        setState(CALIBRATION_STATE_ABORT);
        messageIndex=0;
        messageList.clear();
        messageList.append("aborting");
    }
}

void     CalibrationData::setMessageList(QList<QString> messageList)
{
    this->messageList = messageList;
}

const QList<QString>&  CalibrationData::getMessageList() const
{
    if (childsCalibrationData.size()) {
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getState()!=CALIBRATION_STATE_IDLE)
                return x->getMessageList();

        return  CalibrationData::emptyMessageList;
    }
    else
        return messageList;
}

void     CalibrationData::setMessageIndex(uint8_t index)
{
    messageIndex = index;
}

uint8_t  CalibrationData::getMessageIndex() const
{
    if (childsCalibrationData.size()) {
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getMessageIndex())
                return x->getMessageIndex();
        // otherwise
        return 0;

    } else
        return messageIndex;
}

const QString& CalibrationData::getName() const
{
    return this->name;
}

void CalibrationData::setName(const QString& name)
{
    this->name=name;
}

QTime CalibrationData::getStepTimestamp() const
{
    if (childsCalibrationData.size()) {
        foreach (CalibrationData* x, childsCalibrationData)
            if (x->getState()!=CALIBRATION_STATE_IDLE)
                return x->getStepTimestamp();

        // otherwise
        return QTime::currentTime();        
    } else
        return stepTimestamp;
}

void     CalibrationData::setParent(CalibrationData* parentCalibrationData)
{
    this->parentCalibrationData=parentCalibrationData;
}

void     CalibrationData::setParent(RealtimeController* realtimeController)
{
    this->realtimeController=realtimeController;
}
