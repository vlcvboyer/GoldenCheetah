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
#include <QString>

class ANT;

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
#define CALIBRATION_STATE_SUCCESS       0x80
#define CALIBRATION_STATE_FAILURE       0x81
#define CALIBRATION_STATE_ABORT         0x82

#define CALIBRATION_DEVICE_NONE         ((uint8_t) 0x00)
#define CALIBRATION_DEVICE_COMPUTRAINER ((uint8_t) 0x01)
#define CALIBRATION_DEVICE_ANT_SPORT    ((uint8_t) 0x02)
#define CALIBRATION_DEVICE_ANT_FEC      ((uint8_t) 0x04)
#define CALIBRATION_DEVICE_ALL          ((uint8_t) 0xFF)

class CalibrationData
{
// This class is used in each RealTime controller in order to manage calibration process
public:
    CalibrationData();

    // here are functions that manage overall calibration process
    // this is the single point of contact to manage calibration process
    // it will go through all devices and check/actuate calibration

    static uint8_t getCalibrationDevice();     // the device that is under calibration process (if started) otherwise the first one with calibration support

    static uint8_t getCalibrationSupported();  // list all calibration that are supported  (by each realtime controllers)
    static uint8_t getCalibrationInProgress(); // list all calibration that are in progress
    static uint8_t getCalibrationCompleted();  // list all calibration that are completed

    static uint8_t getCalibrationState();      // give calibration process state (current step)

    // the TargetDevice defined below will allow each class to give information
    // related to devices to be calibrated (based on user choice), or all (CALIBRATION_DEVICE_ALL)
    static  void    startCalibration(uint8_t device);

    static  void    resetCalibrationProcess();                // this reset calibration process (for each realtime controllers)

    static  QString getCalibrationMessage();
    static  QList<QString>  getCalibrationMessageList();
    static  uint8_t  getCalibrationMessageIndex();

    static QString typeDescr(uint8_t param_type);
    static QString deviceDescr(uint8_t param_device);
    static QString stateDescr(uint8_t param_state);

    static QList<QString> ProcessMessageList(uint8_t type);

    // here are functions to address the device linked to this class instance only 
    // (and sub-classes when sub-nodes are present such as ANT devices connected to ANT controller)
    virtual void     setDevice(uint8_t device);
    virtual uint8_t  getDevice() const;

    virtual void     setSupported(uint8_t type);
    virtual uint8_t  getSupported() const;
    virtual void     setInProgress(uint8_t type);
    virtual uint8_t  getInProgress() const;
    virtual void     setCompleted(uint8_t type);
    virtual uint8_t  getCompleted() const;

    virtual void     setState(uint8_t state);
    virtual uint8_t  getState() const;

    virtual void     setTargetSpeed(double target_speed);
    virtual void     start(uint8_t type);
    virtual void     force(uint8_t type);
    virtual void     resetProcess();     // this reset calibration process (for this calibration class)

    virtual void     setMessageList(QList<QString> messageList);
    virtual QList<QString>  getMessageList() const;
    virtual void     setMessageIndex(uint8_t index);
    virtual uint8_t  getMessageIndex() const;

private:
    uint8_t supported;
    uint8_t inProgress;
    uint8_t completed;
    uint8_t state;
    uint8_t type;
    uint8_t device;
    QList<QString> messageList;
    uint8_t messageIndex;
    double targetSpeed;
};

class ANTCalibrationData : CalibrationData
{
// This class is used in ANT RealTime controller in order to manage calibration process
// it will read data in each ANTChannel CalibrationData in order to give the overall information
public:

    ANTCalibrationData(ANT* parent) : parent(parent) {};

    virtual uint8_t  getDevice() const;

    virtual uint8_t  getSupported() const;
    virtual uint8_t  getInProgress() const;
    virtual uint8_t  getCompleted() const;

    virtual uint8_t  getState() const;

    virtual void     start(uint8_t type);
    virtual void     force(uint8_t type);
    virtual void     resetProcess();

    virtual void     setMessageList(QList<QString> messageList);
    virtual QList<QString>  getMessageList() const;
    virtual void     setMessageIndex(uint8_t index);
    virtual uint8_t  getMessageIndex() const;

private:
    ANT* parent;
};

#endif /* SRC_CALIBRATIONDATA_H_ */
