/*********************************************************************
 *
 * $Id: yocto_dualpower.cpp 12324 2013-08-13 15:10:31Z mvuilleu $
 *
 * Implements yFindDualPower(), the high-level API for DualPower functions
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
#include "yocto_dualpower.h"
#include "yapi/yjson.h"
#include "yapi/yapi.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//--- (YDualPower constructor)
// Constructor is protected, use yFindDualPower factory function to instantiate
YDualPower::YDualPower(const string& func): YFunction("DualPower", func)
//--- (end of YDualPower constructor)
//--- (DualPower initialization)
            ,_callback(NULL)
            ,_logicalName(Y_LOGICALNAME_INVALID)
            ,_advertisedValue(Y_ADVERTISEDVALUE_INVALID)
            ,_powerState(Y_POWERSTATE_INVALID)
            ,_powerControl(Y_POWERCONTROL_INVALID)
            ,_extVoltage(Y_EXTVOLTAGE_INVALID)
//--- (end of DualPower initialization)
{}

YDualPower::~YDualPower() 
{
//--- (YDualPower cleanup)
//--- (end of YDualPower cleanup)
}
//--- (YDualPower implementation)

const string YDualPower::LOGICALNAME_INVALID = "!INVALID!";
const string YDualPower::ADVERTISEDVALUE_INVALID = "!INVALID!";



int YDualPower::_parse(yJsonStateMachine& j)
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
        } else if(!strcmp(j.token, "powerState")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _powerState =  (Y_POWERSTATE_enum)atoi(j.token);
        } else if(!strcmp(j.token, "powerControl")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _powerControl =  (Y_POWERCONTROL_enum)atoi(j.token);
        } else if(!strcmp(j.token, "extVoltage")) {
            if(yJsonParse(&j) != YJSON_PARSE_AVAIL) return -1;
            _extVoltage =  atoi(j.token);
        } else {
            // ignore unknown field
            yJsonSkip(&j, 1);
        }
    }
    if(j.st != YJSON_PARSE_STRUCT) goto failed;
    return 0;
}

/**
 * Returns the logical name of the power control.
 * 
 * @return a string corresponding to the logical name of the power control
 * 
 * On failure, throws an exception or returns Y_LOGICALNAME_INVALID.
 */
string YDualPower::get_logicalName(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_LOGICALNAME_INVALID;
    }
    return _logicalName;
}

/**
 * Changes the logical name of the power control. You can use yCheckLogicalName()
 * prior to this call to make sure that your parameter is valid.
 * Remember to call the saveToFlash() method of the module if the
 * modification must be kept.
 * 
 * @param newval : a string corresponding to the logical name of the power control
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YDualPower::set_logicalName(const string& newval)
{
    string rest_val;
    rest_val = newval;
    return _setAttr("logicalName", rest_val);
}

/**
 * Returns the current value of the power control (no more than 6 characters).
 * 
 * @return a string corresponding to the current value of the power control (no more than 6 characters)
 * 
 * On failure, throws an exception or returns Y_ADVERTISEDVALUE_INVALID.
 */
string YDualPower::get_advertisedValue(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_ADVERTISEDVALUE_INVALID;
    }
    return _advertisedValue;
}

/**
 * Returns the current power source for module functions that require lots of current.
 * 
 * @return a value among Y_POWERSTATE_OFF, Y_POWERSTATE_FROM_USB and Y_POWERSTATE_FROM_EXT
 * corresponding to the current power source for module functions that require lots of current
 * 
 * On failure, throws an exception or returns Y_POWERSTATE_INVALID.
 */
Y_POWERSTATE_enum YDualPower::get_powerState(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_POWERSTATE_INVALID;
    }
    return _powerState;
}

/**
 * Returns the selected power source for module functions that require lots of current.
 * 
 * @return a value among Y_POWERCONTROL_AUTO, Y_POWERCONTROL_FROM_USB, Y_POWERCONTROL_FROM_EXT and
 * Y_POWERCONTROL_OFF corresponding to the selected power source for module functions that require lots of current
 * 
 * On failure, throws an exception or returns Y_POWERCONTROL_INVALID.
 */
Y_POWERCONTROL_enum YDualPower::get_powerControl(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_POWERCONTROL_INVALID;
    }
    return _powerControl;
}

/**
 * Changes the selected power source for module functions that require lots of current.
 * 
 * @param newval : a value among Y_POWERCONTROL_AUTO, Y_POWERCONTROL_FROM_USB, Y_POWERCONTROL_FROM_EXT
 * and Y_POWERCONTROL_OFF corresponding to the selected power source for module functions that require
 * lots of current
 * 
 * @return YAPI_SUCCESS if the call succeeds.
 * 
 * On failure, throws an exception or returns a negative error code.
 */
int YDualPower::set_powerControl(Y_POWERCONTROL_enum newval)
{
    string rest_val;
    char buf[32]; sprintf(buf, "%u", newval); rest_val = string(buf);
    return _setAttr("powerControl", rest_val);
}

/**
 * Returns the measured voltage on the external power source, in millivolts.
 * 
 * @return an integer corresponding to the measured voltage on the external power source, in millivolts
 * 
 * On failure, throws an exception or returns Y_EXTVOLTAGE_INVALID.
 */
unsigned YDualPower::get_extVoltage(void)
{
    if(_cacheExpiration <= YAPI::GetTickCount()) {
        if(YISERR(load(YAPI::DefaultCacheValidity))) return Y_EXTVOLTAGE_INVALID;
    }
    return _extVoltage;
}

YDualPower *YDualPower::nextDualPower(void)
{
    string  hwid;
    
    if(YISERR(_nextFunction(hwid)) || hwid=="") {
        return NULL;
    }
    return yFindDualPower(hwid);
}

void YDualPower::registerValueCallback(YDualPowerUpdateCallback callback)
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

void YDualPower::advertiseValue(const string& value)
{
    if (_callback != NULL) {
        _callback(this, value);
    }
}


YDualPower* YDualPower::FindDualPower(const string& func)
{
    if(YAPI::_YFunctionsCaches["YDualPower"].find(func) != YAPI::_YFunctionsCaches["YDualPower"].end())
        return (YDualPower*) YAPI::_YFunctionsCaches["YDualPower"][func];
    
    YDualPower *newDualPower = new YDualPower(func);
    YAPI::_YFunctionsCaches["YDualPower"][func] = newDualPower ;
    return newDualPower;
}

YDualPower* YDualPower::FirstDualPower(void)
{
    vector<YFUN_DESCR>   v_fundescr;
    YDEV_DESCR             ydevice;
    string              serial, funcId, funcName, funcVal, errmsg;
    
    if(YISERR(YapiWrapper::getFunctionsByClass("DualPower", 0, v_fundescr, sizeof(YFUN_DESCR), errmsg)) ||
       v_fundescr.size() == 0 ||
       YISERR(YapiWrapper::getFunctionInfo(v_fundescr[0], ydevice, serial, funcId, funcName, funcVal, errmsg))) {
        return NULL;
    }
    return YDualPower::FindDualPower(serial+"."+funcId);
}

//--- (end of YDualPower implementation)

//--- (DualPower functions)
//--- (end of DualPower functions)
