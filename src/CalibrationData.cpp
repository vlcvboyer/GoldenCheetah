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


#include "CalibrationData.h"
#include <QtDebug>

CalibrationData::CalibrationData()
{
    type = CALIBRATION_TYPE_NOT_SUPPORTED;
    state = CALIBRATION_STATE_IDLE;
    device = CALIBRATION_DEVICE_NONE;
    targetspeed = spindowntime = zerooffset = 0;
}

uint8_t CalibrationData::getType()
{
    return this->type;
}

void CalibrationData::setType(uint8_t type)
{
    if (this->type != type) {

        // debug message:
        QString StrCalib = "Calibration type changing from ";
        switch(this->type) {
            case CALIBRATION_TYPE_UNKNOWN:
                StrCalib += "UNKNOWN";
                break;
            case CALIBRATION_TYPE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_TYPE_ZERO_OFFSET:
                StrCalib += "ZERO_OFFSET";
                break;
            case CALIBRATION_TYPE_SPINDOWN:
                StrCalib += "SPINDOWN";
                break;
            default:
                StrCalib += "UNKNOWN";
        }
        StrCalib += " to ";
        switch(type) {
            case CALIBRATION_TYPE_UNKNOWN:
                StrCalib += "UNKNOWN";
                break;
            case CALIBRATION_TYPE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_TYPE_ZERO_OFFSET:
                StrCalib += "ZERO_OFFSET";
                break;
            case CALIBRATION_TYPE_SPINDOWN:
                StrCalib += "SPINDOWN";
                break;
            default:
                StrCalib += "UNKNOWN";
        }
        qDebug() << qPrintable(StrCalib);

        this->type = type;

    }
}

uint8_t CalibrationData::getState()
{
    return this->state;
}

void CalibrationData::setState(uint8_t state)
{
    if (this->state != state) {

        // debug message:
        QString StrCalib = "Calibration state changing from ";
        switch(this->state) {
            case CALIBRATION_STATE_IDLE:
                StrCalib += "IDLE";
                break;
            case CALIBRATION_STATE_REQUIRED:
                StrCalib += "REQUIRED";
                break;
            case CALIBRATION_STATE_REQUESTED:
                StrCalib += "REQUESTED";
                break;
            case CALIBRATION_STATE_STARTING:
                StrCalib += "STARTING";
                break;
            case CALIBRATION_STATE_STARTED:
                StrCalib += "STARTED";
                break;
            case CALIBRATION_STATE_SPEEDUP:
                StrCalib += "SPEEDUP";
                break;
            case CALIBRATION_STATE_COAST:
                StrCalib += "COAST";
                break;
            case CALIBRATION_STATE_SUCCESS:
                StrCalib += "SUCCESS";
                break;
            case CALIBRATION_STATE_FAILURE:
                StrCalib += "FAILURE";
                break;
            default:
                StrCalib += "UNKNOWN";
        }

        StrCalib += " to ";
        switch(this->state) {
            case CALIBRATION_STATE_IDLE:
                StrCalib += "IDLE";
                break;
            case CALIBRATION_STATE_REQUIRED:
                StrCalib += "REQUIRED";
                break;
            case CALIBRATION_STATE_REQUESTED:
                StrCalib += "REQUESTED";
                break;
            case CALIBRATION_STATE_STARTING:
                StrCalib += "STARTING";
                break;
            case CALIBRATION_STATE_STARTED:
                StrCalib += "STARTED";
                break;
            case CALIBRATION_STATE_SPEEDUP:
                StrCalib += "SPEEDUP";
                break;
            case CALIBRATION_STATE_COAST:
                StrCalib += "COAST";
                break;
            case CALIBRATION_STATE_SUCCESS:
                StrCalib += "SUCCESS";
                break;
            case CALIBRATION_STATE_FAILURE:
                StrCalib += "FAILURE";
                break;
            default:
                StrCalib += "UNKNOWN";
        }
        qDebug() << qPrintable(StrCalib);

        this->state = state;
    }
}

uint8_t CalibrationData::getTargetDevice()
{
    return this->target_device;
}

void CalibrationData::setTargetDevice(uint8_t device)
{
    if (this->target_device != target_device) {

        // debug message:
        QString StrCalib = "Calibration target device changing from ";
        switch(this->device) {
            case CALIBRATION_DEVICE_NONE:
                StrCalib += "NONE";
                break;
            case CALIBRATION_DEVICE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_DEVICE_ANT_SPORT:
                StrCalib += "ANT_SPORT";
                break;
            case CALIBRATION_DEVICE_ANT_FEC:
                StrCalib += "ANT FEC";
                break;
            default:
                StrCalib += "UNKNOWN";
        }

        StrCalib += " to ";
        switch(this->state) {
            case CALIBRATION_DEVICE_NONE:
                StrCalib += "NONE";
                break;
            case CALIBRATION_DEVICE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_DEVICE_ANT_SPORT:
                StrCalib += "ANT_SPORT";
                break;
            case CALIBRATION_DEVICE_ANT_FEC:
                StrCalib += "ANT FEC";
            case CALIBRATION_DEVICE_ALL:
                StrCalib += "ALL";
                break;
            default:
                StrCalib += "UNKNOWN";
        }
        qDebug() << qPrintable(StrCalib);

        this->target_device = target_device;
    }
}

uint8_t CalibrationData::getCurrentDevice()
{
    return this->current_device;
}

void CalibrationData::setCurrentDevice(uint8_t device)
{
    if (this->current_device != current_device) {

        // debug message:
        QString StrCalib = "Calibration device changing from ";
        switch(this->device) {
            case CALIBRATION_DEVICE_NONE:
                StrCalib += "NONE";
                break;
            case CALIBRATION_DEVICE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_DEVICE_ANT_SPORT:
                StrCalib += "ANT_SPORT";
                break;
            case CALIBRATION_DEVICE_ANT_FEC:
                StrCalib += "ANT FEC";
                break;
            default:
                StrCalib += "UNKNOWN";
        }

        StrCalib += " to ";
        switch(this->state) {
            case CALIBRATION_DEVICE_NONE:
                StrCalib += "NONE";
                break;
            case CALIBRATION_DEVICE_COMPUTRAINER:
                StrCalib += "COMPUTRAINER";
                break;
            case CALIBRATION_DEVICE_ANT_SPORT:
                StrCalib += "ANT_SPORT";
                break;
            case CALIBRATION_DEVICE_ANT_FEC:
                StrCalib += "ANT FEC";
                break;
            default:
                StrCalib += "UNKNOWN";
        }
        qDebug() << qPrintable(StrCalib);

        this->current_device = current_device;
    }
}

uint16_t CalibrationData::getSpindownTime()
{
    return this->spindowntime;
}

void CalibrationData::setSpindownTime(uint16_t time)
{
    if (this->spindowntime != time) {
        qDebug() << "calibration spindown time changing to " << time;
        this->spindowntime = time;
    }
}

uint16_t CalibrationData::getZeroOffset()
{
    return this->zerooffset;
}

void CalibrationData::setZeroOffset(uint16_t offset)
{
    if (this->zerooffset != offset) {
        qDebug() << "calibration zero offset changing to " << offset;
        this->zerooffset = offset;
    }
}

double CalibrationData::getTargetSpeed()
{
    return this->targetspeed;
}

void CalibrationData::setTargetSpeed(double speed)
{
    if (this->targetspeed != speed) {
        qDebug() << "calibration target speed changing to " << speed;
        this->targetspeed = speed;
    }
}


