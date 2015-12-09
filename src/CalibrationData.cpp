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

CalibrationData::CalibrationData()
{
    device=CALIBRATION_TYPE_UNKNOWN;
    supported=CALIBRATION_TYPE_UNKNOWN;
    inProgress=CALIBRATION_TYPE_UNKNOWN;
    completed=CALIBRATION_TYPE_NONE;
    state=CALIBRATION_STATE_IDLE;
    targetSpeed=0.0;
    messageIndex = 0;
}

uint8_t CalibrationData::getCalibrationDevice()
// the device that is under calibration process (if started) otherwise the first one with calibration support
{
    uint8_t ret_val = CALIBRATION_DEVICE_NONE;

    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
    {
        if (ret_val)
            break;
        else if (x.controller)
            ret_val |= x.controller->calibrationData.getDevice();
    }

    return ret_val;
}

uint8_t CalibrationData::getCalibrationSupported()
// list all calibration that are supported  (by each realtime controllers)
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if (x.controller)
            ret_val |= x.controller->calibrationData.getSupported();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationInProgress()
// list all calibration that are in progress
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if (x.controller)
            ret_val |= x.controller->calibrationData.getInProgress();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationCompleted()
// list all calibration that are completed
{
    uint8_t ret_val = CALIBRATION_TYPE_NONE;

    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if (x.controller)
            ret_val |= x.controller->calibrationData.getCompleted();

    return ret_val;
}

uint8_t CalibrationData::getCalibrationState()
{
    uint8_t ret_val = CALIBRATION_STATE_IDLE;

    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
    {
        if (ret_val)
            break;
        else if (x.controller)
            ret_val = x.controller->calibrationData.getState();
    }

    return ret_val;
}

void CalibrationData::startCalibration(uint8_t device)
{
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    if (getCalibrationState()==CALIBRATION_STATE_IDLE)
        foreach(DeviceConfiguration x, Devices)
            if ((x.controller) &&
                (x.controller->calibrationData.getDevice() & device) &&
                (x.controller->calibrationData.getSupported() & ~x.controller->calibrationData.getCompleted()))
            {
                uint8_t calibrationType = 0x01;
                while (!(calibrationType & x.controller->calibrationData.getSupported() & ~x.controller->calibrationData.getCompleted()))
                    calibrationType = calibrationType<<1;
                qDebug() << "Start " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData.getDevice()); 
                x.controller->calibrationData.start(calibrationType);
                break;
            }

    if (getCalibrationState()==CALIBRATION_STATE_IDLE)
        foreach(DeviceConfiguration x, Devices)
            if ((x.controller) &&
                (x.controller->calibrationData.getDevice() & device) &&
                (x.controller->calibrationData.getSupported()))
            {
                uint8_t calibrationType = 0x01;
                while (!(calibrationType & x.controller->calibrationData.getSupported()))
                    calibrationType = calibrationType<<1;
                qDebug() << "Force " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData.getDevice()); 
                x.controller->calibrationData.force(calibrationType);
                break;
            }
}

void CalibrationData::resetCalibrationProcess()
{
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if ((x.controller) && (x.controller->calibrationData.getState()!=CALIBRATION_STATE_IDLE))
            x.controller->calibrationData.resetProcess();
}

QString CalibrationData::getCalibrationMessage()
{
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if (x.controller)
        {
            QList<QString> messageList = x.controller->calibrationData.getMessageList();
            int8_t index = x.controller->calibrationData.getMessageIndex();
            if (index<messageList.size())
                return messageList[index];
        }
    return "";
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
        default:
            return qPrintable("UNKNOWN");
    }
}

QList<QString> ProcessMessageList(uint8_t type)
{
    QList<QString> ret_val;
    switch (type)
    {
        case CALIBRATION_TYPE_COMPUTRAINER:
            ret_val.append("step 1");
            ret_val.append("step 2");
            ret_val.append("done");
            break;
        case CALIBRATION_TYPE_ZERO_OFFSET:
            ret_val.append("remove foot from pedals");
            ret_val.append("done");
            break;
        case CALIBRATION_TYPE_SPINDOWN:
            ret_val.append("speed up to 30kph");
            ret_val.append("coast freewheel");
            ret_val.append("done");
            break;
        case CALIBRATION_TYPE_CONFIGURATION:
            ret_val.append("configuring");
            ret_val.append("done");
            break;
    }
    return ret_val;
}

void     CalibrationData::setDevice(uint8_t device)
{
    this->device=device;
}

uint8_t  CalibrationData::getDevice() const
{
    return device;
}

void     CalibrationData::setSupported(uint8_t type)
{
    supported=type;
}

uint8_t  CalibrationData::getSupported() const
{
    return supported;
}

void     CalibrationData::setInProgress(uint8_t type)
{
    inProgress=type;
}

uint8_t  CalibrationData::getInProgress() const
{
    return inProgress;
}

void     CalibrationData::setCompleted(uint8_t type)
{
completed=type;
}
uint8_t  CalibrationData::getCompleted() const
{
    return completed;
}

void     CalibrationData::setState(uint8_t state)
{
    if (this->state!=state)
        qDebug() << "Calibration type changing from " << typeDescr(this->state) << "to" << typeDescr(state);
    this->state=state;
}

uint8_t  CalibrationData::getState() const
{
    return state;
}

void     CalibrationData::setTargetSpeed(double target_speed)
{
    targetSpeed = target_speed;
}

void     CalibrationData::start(uint8_t type)
{
    state = CALIBRATION_STATE_REQUESTED;
    this->type=type;
}

void     CalibrationData::force(uint8_t type)
{
    state = CALIBRATION_STATE_REQUESTED;
    this->type=type;
}

void     CalibrationData::resetProcess()
{
    state = CALIBRATION_STATE_ABORT;
}

void     CalibrationData::setMessageList(QList<QString> messageList)
{
    this->messageList = messageList;
}

QList<QString>  CalibrationData::getMessageList() const
{
    return messageList;
}

void     CalibrationData::setMessageIndex(uint8_t index)
{
    messageIndex = index;
}

uint8_t  CalibrationData::getMessageIndex() const
{
    return messageIndex;
}

uint8_t  ANTCalibrationData::getDevice() const
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            if (parent->antChannel[i]->calibrationData.getDevice())
                return parent->antChannel[i]->calibrationData.getDevice();

    return CALIBRATION_DEVICE_NONE;
}

uint8_t  ANTCalibrationData::getSupported() const
{
    uint8_t supported=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            supported |=parent->antChannel[i]->calibrationData.getSupported();

    return supported;
}

uint8_t  ANTCalibrationData::getInProgress() const
{
    uint8_t inProgress=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            inProgress |=parent->antChannel[i]->calibrationData.getInProgress();

    return inProgress;
}

uint8_t  ANTCalibrationData::getCompleted() const
{
    uint8_t completed=CALIBRATION_TYPE_NONE;
    uint8_t notCompleted=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
        {
            completed    |= parent->antChannel[i]->calibrationData.getCompleted();
            notCompleted |= parent->antChannel[i]->calibrationData.getSupported() & ~parent->antChannel[i]->calibrationData.getCompleted();
        }

    return completed & ~notCompleted;
}

uint8_t  ANTCalibrationData::getState() const
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            if (parent->antChannel[i]->calibrationData.getState())
                return parent->antChannel[i]->calibrationData.getState();

    return CALIBRATION_STATE_IDLE;
}

void     ANTCalibrationData::start(uint8_t type)
{
    if (getState()==CALIBRATION_STATE_IDLE)
        for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
            if (parent && parent->antChannel[i] && 
               (parent->antChannel[i]->calibrationData.getSupported()
               & ~parent->antChannel[i]->calibrationData.getCompleted() 
               & type))
                parent->antChannel[i]->calibrationData.start(type);
}

void     ANTCalibrationData::force(uint8_t type)
{
    if (getState()==CALIBRATION_STATE_IDLE)
        for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
            if (parent && parent->antChannel[i] && 
               (parent->antChannel[i]->calibrationData.getSupported()
               & type))
                parent->antChannel[i]->calibrationData.force(type);
}

void     ANTCalibrationData::resetProcess()
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            parent->antChannel[i]->calibrationData.resetProcess();
}
