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

#ifndef SRC_CALIBRATIONDATA_H_
#define SRC_CALIBRATIONDATA_H_

#include <stdint.h> // uint8_t etc

#define CALIBRATION_TYPE_NONE           ((uint8_t) 0x00)
#define CALIBRATION_TYPE_UNKNOWN        ((uint8_t) 0x00)
#define CALIBRATION_TYPE_NOT_SUPPORTED  ((uint8_t) 0x00)
#define CALIBRATION_TYPE_COMPUTRAINER   ((uint8_t) 0x01)
#define CALIBRATION_TYPE_ZERO_OFFSET    ((uint8_t) 0x02)
#define CALIBRATION_TYPE_SPINDOWN       ((uint8_t) 0x04)
#define CALIBRATION_TYPE_CONFIGURATION  ((uint8_t) 0x08) // FIXME: TODO !!! merge previous method with  this one

#define CALIBRATION_STATE_IDLE          0x00
#define CALIBRATION_STATE_REQUIRED      0x01
#define CALIBRATION_STATE_REQUESTED     0x02
#define CALIBRATION_STATE_STARTING      0x03
#define CALIBRATION_STATE_STARTED       0x04
#define CALIBRATION_STATE_SPEEDUP       0x05
#define CALIBRATION_STATE_COAST         0x06
#define CALIBRATION_STATE_SUCCESS       0x07
#define CALIBRATION_STATE_FAILURE       0x08

#define CALIBRATION_DEVICE_NONE         ((uint8_t) 0x00)
#define CALIBRATION_DEVICE_COMPUTRAINER ((uint8_t) 0x01)
#define CALIBRATION_DEVICE_ANT_SPORT    ((uint8_t) 0x02)
#define CALIBRATION_DEVICE_ANT_FEC      ((uint8_t) 0x04)
#define CALIBRATION_DEVICE_ALL          ((uint8_t) 0xFF)

class CalibrationData
{
public:

    CalibrationData();

    static uint8_t getType();
    static void setType(uint8_t type);

    static uint8_t getState();
    static void setState(uint8_t state);

    static uint8_t getCurrentDevice();
    static void setCurrentDevice(uint8_t device);

    static uint8_t getTargetDevice();
    static void setTargetDevice(uint8_t device);

    uint16_t getZeroOffset() const;
    void setZeroOffset(uint16_t offset);

    uint16_t getSpindownTime() const;
    void setSpindownTime(uint16_t time);

    double getTargetSpeed() const;
    void setTargetSpeed(double speed);

    static QString getMessage();
    static void setMessage(QString message);

private:

    static uint8_t  type;
    static uint8_t  state;
    static uint8_t  current_device;
    static uint8_t  target_device;
    static QString  message;
    uint16_t zerooffset;
    uint16_t spindowntime;
    double   targetspeed;

};

#endif /* SRC_CALIBRATIONDATA_H_ */
