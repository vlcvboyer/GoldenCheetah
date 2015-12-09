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

uint8_t CalibrationData::target_device = CALIBRATION_DEVICE_NONE;

CalibrationData::CalibrationData()
{
    targetDevice=CALIBRATION_TYPE_UNKNOWN;
    calibrationSupported=CALIBRATION_TYPE_UNKNOWN;
    calibrationInProgress=CALIBRATION_TYPE_UNKNOWN;
    calibrationCompleted=CALIBRATION_TYPE_NONE;
    calibrationState=CALIBRATION_STATE_IDLE;
    resetCalibration=false;
    calibrationSpeed=0.0;
    zeroOffset=0;
    spindownTime=0;
    message=QString("");
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
            ret_val |= x.controller->calibrationData._getCalibrationDevice();
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
            ret_val |= x.controller->calibrationData._getCalibrationSupported();

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
            ret_val |= x.controller->calibrationData._getCalibrationInProgress();

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
            ret_val |= x.controller->calibrationData._getCalibrationCompleted();

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
            ret_val = x.controller->calibrationData._getCalibrationState();
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
                (x.controller->calibrationData._getCalibrationDevice() & device) &&
                (x.controller->calibrationData._getCalibrationSupported() & ~x.controller->calibrationData._getCalibrationCompleted())
            {
                for (uint8_t calibrationType = 0x01; !(calibrationType & x.controller->calibrationData._getCalibrationSupported() & ~x.controller->calibrationData._getCalibrationCompleted()); calibrationType<<1)
                    ;
                qDebug() << "Start " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData._getCalibrationDevice()); 
                x.controller->calibrationData._startCalibration(calibrationType);
                break;
            }

    if (getCalibrationState()==CALIBRATION_STATE_IDLE)
        foreach(DeviceConfiguration x, Devices)
            if ((x.controller) &&
                (x.controller->calibrationData._getCalibrationDevice() & device) &&
                (x.controller->calibrationData._getCalibrationSupported()))
            {
                for (uint8_t calibrationType = 0x01; !(calibrationType & x.controller->calibrationData._getCalibrationSupported()); calibrationType<<1)
                    ;
                qDebug() << "Force " << typeDescr(calibrationType) << "calibration of " << deviceDescr(x.controller->calibrationData._getCalibrationDevice()); 
                x.controller->calibrationData._forceCalibration(calibrationType);
                break;
            }
}

void CalibrationData::resetCalibrationProcess(uint8_t device)
{
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if ((x.controller) && (x.controller->calibrationData._getCalibrationState()!=CALIBRATION_STATE_IDLE))
            x.controller->calibrationData._resetCalibrationProcess();
}

QString CalibrationData::getMessage()
{
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices;
    Devices = all.getList();

    foreach(DeviceConfiguration x, Devices)
        if ((x.controller) && (x.controller->calibrationData._getMessage()!=""))
            return x.controller->calibrationData._getMessage();
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
            Sreturn qPrintable("UNKNOWN");
    }
}

uint8_t  ANTCalibrationData::_getCalibrationDevice() const
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            if (parent->antChannel[i]->calibrationData._getCalibrationDevice())
                return parent->antChannel[i]->calibrationData._getCalibrationDevice();

    return CALIBRATION_DEVICE_NONE;
}

uint8_t  ANTCalibrationData::_getCalibrationSupported() const
{
    uint8_t supported=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            supported |=parent->antChannel[i]->calibrationData._getCalibrationSupported();

    return supported;
}

uint8_t  ANTCalibrationData::_getCalibrationInProgress() const
{
    uint8_t inProgress=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            inProgress |=parent->antChannel[i]->calibrationData._getCalibrationInProgress();

    return inProgress;
}

uint8_t  ANTCalibrationData::_getCalibrationCompleted() const
{
    uint8_t completed=CALIBRATION_TYPE_NONE;
    uint8_t notCompleted=CALIBRATION_TYPE_NONE;
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
        {
            completed    |= parent->antChannel[i]->calibrationData._getCalibrationCompleted();
            notCompleted |= parent->antChannel[i]->calibrationData._getCalibrationSupported() & ~parent->antChannel[i]->calibrationData._getCalibrationCompleted();
        }

    return completed & ~notCompleted;
}

uint8_t  ANTCalibrationData::_getCalibrationState() const;
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            if (parent->antChannel[i]->calibrationData._getCalibrationState())
                return parent->antChannel[i]->calibrationData._getCalibrationState();

    return CALIBRATION_STATE_IDLE;
}

void     ANTCalibrationData::_startCalibration(uint8_t type)
{
    if (_getCalibrationState()==CALIBRATION_STATE_IDLE)
        for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
            if (parent && parent->antChannel[i] && 
               (parent->antChannel[i]->calibrationData._getCalibrationSupported()
               & ~parent->antChannel[i]->calibrationData._getCalibrationCompleted() 
               & type))
                parent->antChannel[i]->calibrationData._startCalibration;
}

void     ANTCalibrationData::_forceCalibration(uint8_t type)
{
    if (_getCalibrationState()==CALIBRATION_STATE_IDLE)
        for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
            if (parent && parent->antChannel[i] && 
               (parent->antChannel[i]->calibrationData._getCalibrationSupported()
               & type))
                parent->antChannel[i]->calibrationData._forceCalibration;
}

void     ANTCalibrationData::_resetCalibrationProcess();
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            parent->antChannel[i]->calibrationData._resetCalibrationProcess();
}

QString  ANTCalibrationData::_getMessage() const
{
    for (uint8_t i=0; i<ANT_MAX_CHANNELS; i++)
        if (parent && parent->antChannel[i])
            if (parent->antChannel[i]->calibrationData._getMessage()!="")
                return parent->antChannel[i]->calibrationData._getMessage();

    return "";
}
