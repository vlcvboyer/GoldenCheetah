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
        QString StrCalib = "calibration type changing from ";
        switch(this->type) {
            case CALIBRATION_TYPE_UNKNOWN:
                StrCalib += "CALIBRATION_TYPE_UNKNOWN";
                break
            case CALIBRATION_TYPE_COMPUTRAINER:
                StrCalib += "CALIBRATION_TYPE_COMPUTRAINER";
                break
            case CALIBRATION_TYPE_ZERO_OFFSET:
                StrCalib += "CALIBRATION_TYPE_ZERO_OFFSET";
                break
            case CALIBRATION_TYPE_SPINDOWN:
                StrCalib += "CALIBRATION_TYPE_SPINDOWN";
                break
        }
        StrCalib += " to ";
        switch(type) {
            case CALIBRATION_TYPE_UNKNOWN:
                StrCalib += "CALIBRATION_TYPE_UNKNOWN";
                break
            case CALIBRATION_TYPE_COMPUTRAINER:
                StrCalib += "CALIBRATION_TYPE_COMPUTRAINER";
                break
            case CALIBRATION_TYPE_ZERO_OFFSET:
                StrCalib += "CALIBRATION_TYPE_ZERO_OFFSET";
                break
            case CALIBRATION_TYPE_SPINDOWN:
                StrCalib += "CALIBRATION_TYPE_SPINDOWN";
                break
        }
        qDebug() << StrCalib;

        this->type = type;

        }
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
        QString StrCalib = "calibration state changing from ";
        switch(this->state) {
            case CALIBRATION_STATE_IDLE:
                StrCalib += "CALIBRATION_STATE_IDLE";
                break
            case CALIBRATION_STATE_REQUIRED:
                StrCalib += "CALIBRATION_STATE_REQUIRED";
                break
            case CALIBRATION_STATE_REQUESTED:
                StrCalib += "CALIBRATION_STATE_REQUESTED";
                break
            case CALIBRATION_STATE_STARTING:
                StrCalib += "CALIBRATION_STATE_STARTING";
                break
            case CALIBRATION_STATE_STARTED:
                StrCalib += "CALIBRATION_STATE_STARTED";
                break
            case CALIBRATION_STATE_SPEEDUP:
                StrCalib += "CALIBRATION_STATE_SPEEDUP";
                break
            case CALIBRATION_STATE_COAST:
                StrCalib += "CALIBRATION_STATE_COAST";
                break
            case CALIBRATION_STATE_SUCCESS:
                StrCalib += "CALIBRATION_STATE_SUCCESS";
                break
            case CALIBRATION_STATE_FAILURE:
                StrCalib += "CALIBRATION_STATE_FAILURE";
                break
        }

        StrCalib += " to ";
        switch(this->state) {
            case CALIBRATION_STATE_IDLE:
                StrCalib += "CALIBRATION_STATE_IDLE";
                break
            case CALIBRATION_STATE_REQUIRED:
                StrCalib += "CALIBRATION_STATE_REQUIRED";
                break
            case CALIBRATION_STATE_REQUESTED:
                StrCalib += "CALIBRATION_STATE_REQUESTED";
                break
            case CALIBRATION_STATE_STARTING:
                StrCalib += "CALIBRATION_STATE_STARTING";
                break
            case CALIBRATION_STATE_STARTED:
                StrCalib += "CALIBRATION_STATE_STARTED";
                break
            case CALIBRATION_STATE_SPEEDUP:
                StrCalib += "CALIBRATION_STATE_SPEEDUP";
                break
            case CALIBRATION_STATE_COAST:
                StrCalib += "CALIBRATION_STATE_COAST";
                break
            case CALIBRATION_STATE_SUCCESS:
                StrCalib += "CALIBRATION_STATE_SUCCESS";
                break
            case CALIBRATION_STATE_FAILURE:
                StrCalib += "CALIBRATION_STATE_FAILURE";
                break
        }
        qDebug() << StrCalib;

        this->state = state;
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


