/*********************************************************************
 *
 * $Id: yocto_genericsensor.cpp 12324 2013-08-13 15:10:31Z mvuilleu $
 *
 * Implements yFindGenericSensor(), the high-level API for GenericSensor functions
 *
 * - - - - - - - - - License information: - - - - - - - - - 
 *
 *  Copyright (C) 2011 and beyond by Yoctopuce Sarl, Switzerland.
 *
 *  Yoctopuce Sarl (hereafter Licensor) grants to you a perpetual
 *  non-exclusive license to use, modify, copy and integrate this
 *  file into your software for the sole purpose of interfacing 
 *  with Yoctopuce products. 
 *
 *  You may reproduce and distribute copies of this file in 
 *  source or object form, as long as the sole purpose of this
 *  code is to interface with Yoctopuce products. You must retain 
 *  this notice in the distributed source file.
 *
 *  You should refer to Yoctopuce General Terms and Conditions
 *  for additional information regarding your rights and 
 *  obligations.
 *
 *  THE SOFTWARE AND DOCUMENTATION ARE PROVIDED 'AS IS' WITHOUT
 *  WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING 
 *  WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS 
 *  FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO
 *  EVENT SHALL LICENSOR BE LIABLE FOR ANY INCIDENTAL, SPECIAL,
 *  INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, 
 *  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR 
 *  SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT 
 *  LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR
 *  CONTRIBUTION, OR OTHER SIMILAR COSTS, WHETHER ASSERTED ON THE
 *  BASIS OF CONTRACT, TORT (INCLUDING NEGLIGENCE), BREACH OF
 *  WARRANTY, OR OTHERWISE.
 *
 *********************************************************************/


#define _CRT_SECURE_NO_DEPRECATE //do not use windows secure crt
#include "yocto_genericsensor.h"
#include "yapi/yjson.h"
#include "yapi/yapi.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//--- (YGenericSensor constructor)
// Constructor is protected, use yFindGenericSensor factory function to instantiate
YGenericSensor::YGenericSensor(const string& func): YFunction("GenericSensor", func)
//--- (end of YGenericSensor constructor)
//--- (GenericSensor initialization)
            ,_callback(NULL)
            ,_logicalName(Y_LOGICALNAME_INVALID)
            ,_advertisedValue(Y_ADVERTISEDVALUE_INVALID)
            ,_unit(Y_UNIT_INVALID)
            ,_currentValue(Y_CURRENTVALUE_INVALID)
            ,_lowestValue(Y_LOWESTVALUE_INVALID)
            ,_highestValue(Y_HIGHESTVALUE_INVALID)
            ,_currentRawValue(Y_CURRENTRAWVALUE_INVALID)
            ,_calibrationParam(Y_CALIBRATIONPARAM_INVALID)
            ,_signalValue(Y_SIGNALVALUE_INVALID)
            ,_signalUnit(Y_SIGNALUNIT_INVALID)
            ,_signalRange(Y_SIGNALRANGE_INVALID)
            ,_valueRange(Y_VALUERANGE_INVALID)
            ,_resolution(Y_RESOLUTION_INVALID)
            ,_calibrationOffset(0)
//--- (end of GenericSensor initialization)
{}

YGenericSensor::~YGenericSensor() 
{
//--- (YGenericSensor cleanup)
//--- (end of YGenericSensor cleanup)
}
//--- (YGenericSensor implementation)

const string YGenericSensor::LOGICALNAME_INVALID = "!INVALID!";
const string YGenericSensor::ADVERTISEDVALUE_INVALID = "!INVALID!";
const string YGenericSensor::UNIT_INVALID = "!INVALID!";
const double YGenericSensor::CURRENTVALUE_INVALID = -DBL_MAX;
const double YGenericSensor::LOWESTVALUE_INVALID = -DBL_MAX;
const double YGenericSensor::HIGHESTVALUE_INVALID = -DBL_MAX;
const double YGenericSensor::CURRENTRAWVALUE_INVALID = -DBL_MAX;
const string YGenericSensor::CALIBRATIONPARAM_INVALID = "!INVALID!";
const double YGenericSensor::SIGNALVALUE_INVALID = -DBL_MAX;
const string YGenericSensor::SIGNALUNIT_INVALID = "!INVALID!";
const string YGenericSensor::SIGNALRANGE_INVALID = "!INVALID!";
const string YGenericSensor::VALUERANGE_INVALID = "!INVALID!";
const double YGenericSensor::RESOLUTION_INVALID = -DBL_MAX;



int YGenericSensor::_parse(yJsonStateMachine& j)
{
    if(yJsonParse(&j) != YJSON_PARSE_AVAIL || j.st != YJSON_PARSE_STRUCT) {
    failed:
        return -1;
    }
    while(yJsonParse(&j) == YJSON_PARSE_AVAIL && j.st == YJSON_PARSE_MEMBNAME) {
        if(!strcmp(j.token, "logicalName")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _logicalName =  _parseString(j);
        } else if(!strcmp(j.token, "advertisedValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _advertisedValue =  _parseString(j);
        } else if(!strcmp(j.token, "unit")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _unit =  _parseString(j);
        } else if(!strcmp(j.token, "currentValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _currentValue =  floor(atof(j.token)/65.536+.5) / 1000;
        } else if(!strcmp(j.token, "lowestValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _lowestValue =  floor(atof(j.token)/65.536+.5) / 1000;
        } else if(!strcmp(j.token, "highestValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _highestValue =  floor(atof(j.token)/65.536+.5) / 1000;
        } else if(!strcmp(j.token, "currentRawValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _currentRawValue =  atof(j.token)/65536.0;
        } else if(!strcmp(j.token, "calibrationParam")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _calibrationParam =  _parseString(j);
        } else if(!strcmp(j.token, "signalValue")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _signalValue =  floor(atof(j.token)/65.536+.5) / 1000;
        } else if(!strcmp(j.token, "signalUnit")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _signalUnit =  _parseString(j);
        } else if(!strcmp(j.token, "signalRange")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _signalRange =  _parseString(j);
        } else if(!strcmp(j.token, "valueRange")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _valueRange =  _parseString(j);
        } else if(!strcmp(j.token, "resolution")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _resolution =  (atoi(j.token) > 100 ? 1.0 / floor(65536.0/atof(j.token)+.5) : 0.001 / floor(67.0/atof(j.token)+.5));
        } else {
            // ignore unknown field
            yJsonSkip(&j, 1);
        }
    }
    if(j.st != YJSON_PARSE_STRUCT) goto failed;
    return 0;
}

/**
 * Returns the logical name of the generic sensor.
 * 
 * @return a string corresponding to the logical name of the generic sensor
 * 
 * On failure, throws an exception or returns Y_LOGICALNAME_INVALID.
 */
string YGenericSensor::get_logicalName(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_LOGICALNAME_INVALID;
    }
    return _logicalName;
}

/**
 * Changes the logical name of the generic sensor. You can use yCheckLogicalName()
 * prior to this call to make sure that your parameter is valid.
 * Remember to call the saveToFlash() method of the module if the
 * modification must be kept.
 * 
 * @param newval : a string corresponding to the logical name of the generic sensor
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_logicalName(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("logicalName", rest_val);
}

/**
 * Returns the current value of the generic sensor (no more than 6 characters).
 * 
 * @return a string corresponding to the current value of the generic sensor (no more than 6 characters)
 * 
 * On failure, throws an exception or returns Y_ADVERTISEDVALUE_INVALID.
 */
string YGenericSensor::get_advertisedValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_ADVERTISEDVALUE_INVALID;
    }
    return _advertisedValue;
}

/**
 * Returns the measuring unit for the measured value.
 * 
 * @return a string corresponding to the measuring unit for the measured value
 * 
 * On failure, throws an exception or returns Y_UNIT_INVALID.
 */
string YGenericSensor::get_unit(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_UNIT_INVALID;
    }
    return _unit;
}

/**
 * Changes the measuring unit for the measured value.
 * Remember to call the saveToFlash() method of the module if the
 * modification must be kept.
 * 
 * @param newval : a string corresponding to the measuring unit for the measured value
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_unit(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("unit", rest_val);
}

/**
 * Returns the current measured value.
 * 
 * @return a floating point number corresponding to the current measured value
 * 
 * On failure, throws an exception or returns Y_CURRENTVALUE_INVALID.
 */
double YGenericSensor::get_currentValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_CURRENTVALUE_INVALID;
    }
    double res = YAPI::_applyCalibration(_currentRawValue, _calibrationParam, _calibrationOffset, _resolution);
    if(res != Y_CURRENTVALUE_INVALID) return res;
    return _currentValue;
}

/**
 * Changes the recorded minimal value observed.
 * 
 * @param newval : a floating point number corresponding to the recorded minimal value observed
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_lowestValue(double newval)
{
    string rest_val;
    char buf[32]; sprintf(buf,"%d", (int)floor(newval*65536.0 +0.5)); rest_val = string(buf);
    return _setAttr("lowestValue", rest_val);
}

/**
 * Returns the minimal value observed.
 * 
 * @return a floating point number corresponding to the minimal value observed
 * 
 * On failure, throws an exception or returns Y_LOWESTVALUE_INVALID.
 */
double YGenericSensor::get_lowestValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_LOWESTVALUE_INVALID;
    }
    return _lowestValue;
}

/**
 * Changes the recorded maximal value observed.
 * 
 * @param newval : a floating point number corresponding to the recorded maximal value observed
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_highestValue(double newval)
{
    string rest_val;
    char buf[32]; sprintf(buf,"%d", (int)floor(newval*65536.0 +0.5)); rest_val = string(buf);
    return _setAttr("highestValue", rest_val);
}

/**
 * Returns the maximal value observed.
 * 
 * @return a floating point number corresponding to the maximal value observed
 * 
 * On failure, throws an exception or returns Y_HIGHESTVALUE_INVALID.
 */
double YGenericSensor::get_highestValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_HIGHESTVALUE_INVALID;
    }
    return _highestValue;
}

/**
 * Returns the uncalibrated, unrounded raw value returned by the sensor.
 * 
 * @return a floating point number corresponding to the uncalibrated, unrounded raw value returned by the sensor
 * 
 * On failure, throws an exception or returns Y_CURRENTRAWVALUE_INVALID.
 */
double YGenericSensor::get_currentRawValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_CURRENTRAWVALUE_INVALID;
    }
    return _currentRawValue;
}

string YGenericSensor::get_calibrationParam(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_CALIBRATIONPARAM_INVALID;
    }
    return _calibrationParam;
}

int YGenericSensor::set_calibrationParam(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("calibrationParam", rest_val);
}

/**
 * Configures error correction data points, in particular to compensate for
 * a possible perturbation of the measure caused by an enclosure. It is possible
 * to configure up to five correction points. Correction points must be provided
 * in ascending order, and be in the range of the sensor. The device will automatically
 * perform a linear interpolation of the error correction between specified
 * points. Remember to call the saveToFlash() method of the module if the
 * modification must be kept.
 * 
 * For more information on advanced capabilities to refine the calibration of
 * sensors, please contact support@yoctopuce.com.
 * 
 * @param rawValues : array of floating point numbers, corresponding to the raw
 *         values returned by the sensor for the correction points.
 * @param refValues : array of floating point numbers, corresponding to the corrected
 *         values for the correction points.
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::calibrateFromPoints(vector<double> rawValues,vector<double> refValues)
{
    string rest_val;
    rest_val = this->_encodeCalibrationPoints(rawValues,refValues,_resolution,_calibrationOffset,_calibrationParam);
    return _setAttr("calibrationParam", rest_val);
}

int YGenericSensor::loadCalibrationPoints(vector<double> rawValues,vector<double> refValues)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return _lastErrorType;
    }
    return this->_decodeCalibrationPoints(_calibrationParam,rawValues,refValues,_resolution,_calibrationOffset);
}

/**
 * Returns the measured value of the electrical signal used by the sensor.
 * 
 * @return a floating point number corresponding to the measured value of the electrical signal used by the sensor
 * 
 * On failure, throws an exception or returns Y_SIGNALVALUE_INVALID.
 */
double YGenericSensor::get_signalValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_SIGNALVALUE_INVALID;
    }
    return _signalValue;
}

/**
 * Returns the measuring unit of the electrical signal used by the sensor.
 * 
 * @return a string corresponding to the measuring unit of the electrical signal used by the sensor
 * 
 * On failure, throws an exception or returns Y_SIGNALUNIT_INVALID.
 */
string YGenericSensor::get_signalUnit(void)
{
    if(_signalUnit == Y_SIGNALUNIT_INVALID) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_SIGNALUNIT_INVALID;
    }
    return _signalUnit;
}

/**
 * Returns the electric signal range used by the sensor.
 * 
 * @return a string corresponding to the electric signal range used by the sensor
 * 
 * On failure, throws an exception or returns Y_SIGNALRANGE_INVALID.
 */
string YGenericSensor::get_signalRange(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_SIGNALRANGE_INVALID;
    }
    return _signalRange;
}

/**
 * Changes the electric signal range used by the sensor.
 * 
 * @param newval : a string corresponding to the electric signal range used by the sensor
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_signalRange(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("signalRange", rest_val);
}

/**
 * Returns the physical value range measured by the sensor.
 * 
 * @return a string corresponding to the physical value range measured by the sensor
 * 
 * On failure, throws an exception or returns Y_VALUERANGE_INVALID.
 */
string YGenericSensor::get_valueRange(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_VALUERANGE_INVALID;
    }
    return _valueRange;
}

/**
 * Changes the physical value range measured by the sensor. The range change may have a side effect
 * on the display resolution, as it may be adapted automatically.
 * 
 * @param newval : a string corresponding to the physical value range measured by the sensor
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_valueRange(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("valueRange", rest_val);
}

/**
 * Changes the resolution of the measured physical values. The resolution corresponds to the numerical precision
 * when displaying value. It does not change the precision of the measure itself.
 * 
 * @param newval : a floating point number corresponding to the resolution of the measured physical values
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YGenericSensor::set_resolution(double newval)
{
    string rest_val;
    char buf[32]; sprintf(buf,"%d", (int)floor(newval*65536.0 +0.5)); rest_val = string(buf);
    return _setAttr("resolution", rest_val);
}

/**
 * Returns the resolution of the measured values. The resolution corresponds to the numerical precision
 * of the values, which is not always the same as the actual precision of the sensor.
 * 
 * @return a floating point number corresponding to the resolution of the measured values
 * 
 * On failure, throws an exception or returns Y_RESOLUTION_INVALID.
 */
double YGenericSensor::get_resolution(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_RESOLUTION_INVALID;
    }
    return _resolution;
}

YGenericSensor *YGenericSensor::nextGenericSensor(void)
{
    string  hwid;
    
    if(YISERR(_nextFunction(hwid)) || hwid=="") {
        return NULL;
    }
    return yFindGenericSensor(hwid);
}

void YGenericSensor::registerValueCallback(YGenericSensorUpdateCallback callback)
{
    if (callback != NULL) {
        _registerFuncCallback(this);
        yapiLockFunctionCallBack(NULL);
        YAPI::_yapiFunctionUpdateCallbackFwd(this->functionDescriptor(), this->get_advertisedValue().c_str());
        yapiUnlockFunctionCallBack(NULL);
    } else {
        _unregisterFuncCallback(this);
    }
    _callback = callback;
}

void YGenericSensor::advertiseValue(const string& value)
{
    if (_callback != NULL) {
        _callback(this, value);
    }
}


YGenericSensor* YGenericSensor::FindGenericSensor(const string& func)
{
    if(YAPI::_YFunctionsCaches["YGenericSensor"].find(func) != YAPI::_YFunctionsCaches["YGenericSensor"].end())
        return (YGenericSensor*) YAPI::_YFunctionsCaches["YGenericSensor"][func];
    
    YGenericSensor *newGenericSensor = new YGenericSensor(func);
    YAPI::_YFunctionsCaches["YGenericSensor"][func] = newGenericSensor ;
    return newGenericSensor;
}

YGenericSensor* YGenericSensor::FirstGenericSensor(void)
{
    vector<YFUN_DESCR>   v_fundescr;
    YDEV_DESCR             ydevice;
    string              serial, funcId, funcName, funcVal, errmsg;
    
    if(YISERR(YapiWrapper::getFunctionsByClass("GenericSensor", 0, v_fundescr, sizeof(YFUN_DESCR), errmsg)) ||
       v_fundescr.size() == 0 ||
       YISERR(YapiWrapper::getFunctionInfo(v_fundescr[0], ydevice, serial, funcId, funcName, funcVal, errmsg))) {
        return NULL;
    }
    return YGenericSensor::FindGenericSensor(serial+"."+funcId);
}

//--- (end of YGenericSensor implementation)

//--- (GenericSensor functions)
//--- (end of GenericSensor functions)