/*
* This file is part of the Head Tracker distribution (https://github.com/dlktdr/headtracker)
* Copyright (c) 2022 Cliff Blackburn
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**********************************************
 *
 *  !!! THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT DIRECTLY !!!
 *
 *  Modify /utils/settings.csv and execute buildsettings.py to generate this GUI header
 *
 ***********************************************/

#pragma once

#include <QObject>
#include <QSettings>

class BaseTrackerSettings : public QObject
{
  Q_OBJECT
public:
  static constexpr float FLOAT_MIN = -1000000;
  static constexpr float FLOAT_MAX = 1000000;
  static constexpr uint16_t MIN_PWM = 988;
  static constexpr uint16_t MAX_PWM = 2012;
  static constexpr uint16_t DEF_MIN_PWM = 1050;
  static constexpr uint16_t DEF_MAX_PWM = 1950;
  static constexpr uint16_t PPM_CENTER = 1500;
  static constexpr float MIN_GAIN = 0.01;
  static constexpr float MAX_GAIN = 35;
  static constexpr uint16_t MINMAX_RNG = 242;
  static constexpr uint16_t MIN_CNT = (((MAX_PWM - MIN_PWM) / 2) + MIN_PWM - MINMAX_RNG);
  static constexpr uint16_t MAX_CNT = (((MAX_PWM - MIN_PWM) / 2) + MIN_PWM + MINMAX_RNG);
  static constexpr uint8_t BT_CHANNELS = 8;
  static constexpr uint8_t MAX_CHANNELS = 16;
  static constexpr uint8_t AUX_FUNCTIONS = 7;
  static constexpr uint8_t TILT_REVERSE_BIT = 1;
  static constexpr uint8_t ROLL_REVERSE_BIT = 2;
  static constexpr uint8_t PAN_REVERSE_BIT = 4;
  static constexpr uint16_t SBUS_CENTER = 992;
  static constexpr float SBUS_SCALE = 1.6;
  static constexpr float RESET_ON_TILT_TIME = 1.5;
  static constexpr float RESET_ON_TILT_AFTER = 1;
  static constexpr float RECENTER_PULSE_DURATION = 0.5;
  static constexpr float SBUS_ACTIVE_TIME = 0.1;
  static constexpr float PPM_MIN_FRAMESYNC = 3000;
  static constexpr float PPM_MIN_FRAME = 6666;
  static constexpr float PPM_MAX_FRAME = 40000;

  BaseTrackerSettings(QObject *parent=nullptr) : 
    QObject(parent)
  {
    _setting["rll_min"] = DEF_MIN_PWM;
    _setting["rll_max"] = DEF_MAX_PWM;
    _setting["rll_cnt"] = PPM_CENTER;
    _setting["rll_gain"] = 5;
    _setting["tlt_min"] = DEF_MIN_PWM;
    _setting["tlt_max"] = DEF_MAX_PWM;
    _setting["tlt_cnt"] = PPM_CENTER;
    _setting["tlt_gain"] = 5;
    _setting["pan_min"] = DEF_MIN_PWM;
    _setting["pan_max"] = DEF_MAX_PWM;
    _setting["pan_cnt"] = PPM_CENTER;
    _setting["pan_gain"] = 5;
    _setting["tltch"] = -1;
    _setting["rllch"] = -1;
    _setting["panch"] = -1;
    _setting["alertch"] = -1;
    _setting["pwm0"] = -1;
    _setting["pwm1"] = -1;
    _setting["pwm2"] = -1;
    _setting["pwm3"] = -1;
    _setting["an0ch"] = -1;
    _setting["an1ch"] = -1;
    _setting["an2ch"] = -1;
    _setting["an3ch"] = -1;
    _setting["aux0ch"] = -1;
    _setting["aux1ch"] = -1;
    _setting["aux2ch"] = -1;
    _setting["rstppm"] = -1;
    _setting["aux0func"] = 0;
    _setting["aux1func"] = 0;
    _setting["aux2func"] = 0;
    _setting["an0gain"] = 310;
    _setting["an1gain"] = 310;
    _setting["an2gain"] = 310;
    _setting["an3gain"] = 310;
    _setting["an0off"] = 0;
    _setting["an1off"] = 0;
    _setting["an2off"] = 0;
    _setting["an3off"] = 0;
    _setting["servoreverse"] = 0;
    _setting["magxoff"] = 0;
    _setting["magyoff"] = 0;
    _setting["magzoff"] = 0;
    _setting["accxoff"] = 0;
    _setting["accyoff"] = 0;
    _setting["acczoff"] = 0;
    _setting["gyrxoff"] = 0;
    _setting["gyryoff"] = 0;
    _setting["gyrzoff"] = 0;
    _setting["so00"] = 1;
    _setting["so01"] = 0;
    _setting["so02"] = 0;
    _setting["so10"] = 0;
    _setting["so11"] = 1;
    _setting["so12"] = 0;
    _setting["so20"] = 0;
    _setting["so21"] = 0;
    _setting["so22"] = 1;
    _setting["rotx"] = 0;
    _setting["roty"] = 0;
    _setting["rotz"] = 0;
    _setting["buttonpin"] = 2;
    _setting["ppmoutpin"] = 10;
    _setting["ppminpin"] = -1;
    _setting["uartmode"] = 0;
    _setting["sbrate"] = 80;
    _setting["sbininv"] = true;
    _setting["sboutinv"] = true;
    _setting["btmode"] = 0;
    _setting["rstonwave"] = false;
    _setting["butlngps"] = false;
    _setting["rstontlt"] = false;
    _setting["ppmoutinvert"] = false;
    _setting["ppmininvert"] = false;
    _setting["ppmframe"] = 22500;
    _setting["ppmsync"] = 350;
    _setting["ppmchcnt"] = 8;
    _setting["lppan"] = 100;
    _setting["lptiltroll"] = 100;
    _setting["btpairedaddress[17]"] = QString("");
    _dataItems["magx"] = false;
    _dataItems["magy"] = false;
    _dataItems["magz"] = false;
    _dataItems["gyrox"] = false;
    _dataItems["gyroy"] = false;
    _dataItems["gyroz"] = false;
    _dataItems["accx"] = false;
    _dataItems["accy"] = false;
    _dataItems["accz"] = false;
    _dataItems["off_magx"] = false;
    _dataItems["off_magy"] = false;
    _dataItems["off_magz"] = false;
    _dataItems["off_gyrox"] = false;
    _dataItems["off_gyroy"] = false;
    _dataItems["off_gyroz"] = false;
    _dataItems["off_accx"] = false;
    _dataItems["off_accy"] = false;
    _dataItems["off_accz"] = false;
    _dataItems["tiltout"] = false;
    _dataItems["rollout"] = false;
    _dataItems["panout"] = false;
    _dataItems["iscal"] = false;
    _dataItems["btcon"] = false;
    _dataItems["trpenabled"] = false;
    _dataItems["tilt"] = false;
    _dataItems["roll"] = false;
    _dataItems["pan"] = false;
    _dataItems["tiltoff"] = false;
    _dataItems["rolloff"] = false;
    _dataItems["panoff"] = false;
    _dataItems["issense"] = false;
    _dataItems["chout"] = false;
    _dataItems["btch"] = false;
    _dataItems["ppmch"] = false;
    _dataItems["uartch"] = false;
    _dataItems["quat"] = false;
    _dataItems["btaddr"] = false;
    _dataItems["btrmt"] = false;
    _deviceDataItems = _dataItems;
  }

  // Roll Minimum
  uint16_t getRll_Min() {
    return _setting["rll_min"].toUInt();
  }
  bool setRll_Min(uint16_t val=DEF_MIN_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["rll_min"] = val;
      return true;
    }
    return false;
  }

  // Roll Maximum
  uint16_t getRll_Max() {
    return _setting["rll_max"].toUInt();
  }
  bool setRll_Max(uint16_t val=DEF_MAX_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["rll_max"] = val;
      return true;
    }
    return false;
  }

  // Roll Center
  uint16_t getRll_Cnt() {
    return _setting["rll_cnt"].toUInt();
  }
  bool setRll_Cnt(uint16_t val=PPM_CENTER) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["rll_cnt"] = val;
      return true;
    }
    return false;
  }

  // Roll Gain
  uint16_t getRll_Gain() {
    return _setting["rll_gain"].toUInt();
  }
  bool setRll_Gain(uint16_t val=5) {
    if(val >= MIN_GAIN && val <= MAX_GAIN) {
      _setting["rll_gain"] = val;
      return true;
    }
    return false;
  }

  // Tilt Minimum
  uint16_t getTlt_Min() {
    return _setting["tlt_min"].toUInt();
  }
  bool setTlt_Min(uint16_t val=DEF_MIN_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["tlt_min"] = val;
      return true;
    }
    return false;
  }

  // Tilt Maximum
  uint16_t getTlt_Max() {
    return _setting["tlt_max"].toUInt();
  }
  bool setTlt_Max(uint16_t val=DEF_MAX_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["tlt_max"] = val;
      return true;
    }
    return false;
  }

  // Tilt Center
  uint16_t getTlt_Cnt() {
    return _setting["tlt_cnt"].toUInt();
  }
  bool setTlt_Cnt(uint16_t val=PPM_CENTER) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["tlt_cnt"] = val;
      return true;
    }
    return false;
  }

  // Tilt Gain
  uint16_t getTlt_Gain() {
    return _setting["tlt_gain"].toUInt();
  }
  bool setTlt_Gain(uint16_t val=5) {
    if(val >= MIN_GAIN && val <= MAX_GAIN) {
      _setting["tlt_gain"] = val;
      return true;
    }
    return false;
  }

  // Pan Minimum
  uint16_t getPan_Min() {
    return _setting["pan_min"].toUInt();
  }
  bool setPan_Min(uint16_t val=DEF_MIN_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["pan_min"] = val;
      return true;
    }
    return false;
  }

  // Pan Maximum
  uint16_t getPan_Max() {
    return _setting["pan_max"].toUInt();
  }
  bool setPan_Max(uint16_t val=DEF_MAX_PWM) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["pan_max"] = val;
      return true;
    }
    return false;
  }

  // Pan Center
  uint16_t getPan_Cnt() {
    return _setting["pan_cnt"].toUInt();
  }
  bool setPan_Cnt(uint16_t val=PPM_CENTER) {
    if(val >= MIN_PWM && val <= MAX_PWM) {
      _setting["pan_cnt"] = val;
      return true;
    }
    return false;
  }

  // Pan Gain
  uint16_t getPan_Gain() {
    return _setting["pan_gain"].toUInt();
  }
  bool setPan_Gain(uint16_t val=5) {
    if(val >= MIN_GAIN && val <= MAX_GAIN) {
      _setting["pan_gain"] = val;
      return true;
    }
    return false;
  }

  // Tilt Output Channel
  int8_t getTltCh() {
    return _setting["tltch"].toInt();
  }
  bool setTltCh(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["tltch"] = val;
      return true;
    }
    return false;
  }

  // Roll Output Channel
  int8_t getRllCh() {
    return _setting["rllch"].toInt();
  }
  bool setRllCh(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["rllch"] = val;
      return true;
    }
    return false;
  }

  // Pan Output Channel
  int8_t getPanCh() {
    return _setting["panch"].toInt();
  }
  bool setPanCh(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["panch"] = val;
      return true;
    }
    return false;
  }

  // Alert Output Channel
  int8_t getAlertCh() {
    return _setting["alertch"].toInt();
  }
  bool setAlertCh(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["alertch"] = val;
      return true;
    }
    return false;
  }

  // PWM 0 Channel
  int8_t getPwm0() {
    return _setting["pwm0"].toInt();
  }
  bool setPwm0(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["pwm0"] = val;
      return true;
    }
    return false;
  }

  // PWM 1 Channel
  int8_t getPwm1() {
    return _setting["pwm1"].toInt();
  }
  bool setPwm1(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["pwm1"] = val;
      return true;
    }
    return false;
  }

  // PWM 2 Channel
  int8_t getPwm2() {
    return _setting["pwm2"].toInt();
  }
  bool setPwm2(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["pwm2"] = val;
      return true;
    }
    return false;
  }

  // PWM 3 Channel
  int8_t getPwm3() {
    return _setting["pwm3"].toInt();
  }
  bool setPwm3(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["pwm3"] = val;
      return true;
    }
    return false;
  }

  // Analog 0 Channel
  int8_t getAn0Ch() {
    return _setting["an0ch"].toInt();
  }
  bool setAn0Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["an0ch"] = val;
      return true;
    }
    return false;
  }

  // Analog 1 Channel
  int8_t getAn1Ch() {
    return _setting["an1ch"].toInt();
  }
  bool setAn1Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["an1ch"] = val;
      return true;
    }
    return false;
  }

  // Analog 2 Channel
  int8_t getAn2Ch() {
    return _setting["an2ch"].toInt();
  }
  bool setAn2Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["an2ch"] = val;
      return true;
    }
    return false;
  }

  // Analog 3 Channel
  int8_t getAn3Ch() {
    return _setting["an3ch"].toInt();
  }
  bool setAn3Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["an3ch"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 0 Channel
  int8_t getAux0Ch() {
    return _setting["aux0ch"].toInt();
  }
  bool setAux0Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["aux0ch"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 1 Channel
  int8_t getAux1Ch() {
    return _setting["aux1ch"].toInt();
  }
  bool setAux1Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["aux1ch"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 2 Channel
  int8_t getAux2Ch() {
    return _setting["aux2ch"].toInt();
  }
  bool setAux2Ch(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["aux2ch"] = val;
      return true;
    }
    return false;
  }

  // Reset Center on PPM Input Channel
  int8_t getRstPpm() {
    return _setting["rstppm"].toInt();
  }
  bool setRstPpm(int8_t val=-1) {
    if(val >= -1 && val <= MAX_CHANNELS) {
      _setting["rstppm"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 0
  uint8_t getAux0Func() {
    return _setting["aux0func"].toUInt();
  }
  bool setAux0Func(uint8_t val=0) {
    if(val <= AUX_FUNCTIONS) {
      _setting["aux0func"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 1
  uint8_t getAux1Func() {
    return _setting["aux1func"].toUInt();
  }
  bool setAux1Func(uint8_t val=0) {
    if(val <= AUX_FUNCTIONS) {
      _setting["aux1func"] = val;
      return true;
    }
    return false;
  }

  // Auxilary Function 2
  uint8_t getAux2Func() {
    return _setting["aux2func"].toUInt();
  }
  bool setAux2Func(uint8_t val=0) {
    if(val <= AUX_FUNCTIONS) {
      _setting["aux2func"] = val;
      return true;
    }
    return false;
  }

  // Analog 0 Gain
  float getAn0Gain() {
    return _setting["an0gain"].toFloat();
  }
  bool setAn0Gain(float val=310) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an0gain"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 1 Gain
  float getAn1Gain() {
    return _setting["an1gain"].toFloat();
  }
  bool setAn1Gain(float val=310) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an1gain"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 2 Gain
  float getAn2Gain() {
    return _setting["an2gain"].toFloat();
  }
  bool setAn2Gain(float val=310) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an2gain"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 3 Gain
  float getAn3Gain() {
    return _setting["an3gain"].toFloat();
  }
  bool setAn3Gain(float val=310) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an3gain"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 0 Offset
  float getAn0Off() {
    return _setting["an0off"].toFloat();
  }
  bool setAn0Off(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an0off"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 1 Offset
  float getAn1Off() {
    return _setting["an1off"].toFloat();
  }
  bool setAn1Off(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an1off"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 2 Offset
  float getAn2Off() {
    return _setting["an2off"].toFloat();
  }
  bool setAn2Off(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an2off"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Analog 3 Offset
  float getAn3Off() {
    return _setting["an3off"].toFloat();
  }
  bool setAn3Off(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["an3off"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Servo Reverse (BitMask)
  uint8_t getServoReverse() {
    return _setting["servoreverse"].toUInt();
  }
  bool setServoReverse(uint8_t val=0) {
    if(val <= 7) {
      _setting["servoreverse"] = val;
      return true;
    }
    return false;
  }

  // Magnetometer X Calibration Offset
  float getMagXOff() {
    return _setting["magxoff"].toFloat();
  }
  bool setMagXOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["magxoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Magnetometer Y Calibration Offset
  float getMagYOff() {
    return _setting["magyoff"].toFloat();
  }
  bool setMagYOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["magyoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Magnetometer Z Calibration Offset
  float getMagZOff() {
    return _setting["magzoff"].toFloat();
  }
  bool setMagZOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["magzoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Accelerometer X Calibration Offset
  float getAccXOff() {
    return _setting["accxoff"].toFloat();
  }
  bool setAccXOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["accxoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Accelerometer Y Calibration Offset
  float getAccYOff() {
    return _setting["accyoff"].toFloat();
  }
  bool setAccYOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["accyoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Accelerometer Z Calibration Offset
  float getAccZOff() {
    return _setting["acczoff"].toFloat();
  }
  bool setAccZOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["acczoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Gyrometer X Calibration Offset
  float getGyrXOff() {
    return _setting["gyrxoff"].toFloat();
  }
  bool setGyrXOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["gyrxoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Gyrometer Y Calibration Offset
  float getGyrYOff() {
    return _setting["gyryoff"].toFloat();
  }
  bool setGyrYOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["gyryoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Gyrometer Z Calibration Offset
  float getGyrZOff() {
    return _setting["gyrzoff"].toFloat();
  }
  bool setGyrZOff(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["gyrzoff"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 00
  float getso00() {
    return _setting["so00"].toFloat();
  }
  bool setso00(float val=1) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so00"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 01
  float getso01() {
    return _setting["so01"].toFloat();
  }
  bool setso01(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so01"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 02
  float getso02() {
    return _setting["so02"].toFloat();
  }
  bool setso02(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so02"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 10
  float getso10() {
    return _setting["so10"].toFloat();
  }
  bool setso10(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so10"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 11
  float getso11() {
    return _setting["so11"].toFloat();
  }
  bool setso11(float val=1) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so11"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 12
  float getso12() {
    return _setting["so12"].toFloat();
  }
  bool setso12(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so12"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 20
  float getso20() {
    return _setting["so20"].toFloat();
  }
  bool setso20(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so20"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 21
  float getso21() {
    return _setting["so21"].toFloat();
  }
  bool setso21(float val=0) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so21"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Soft Iron Offset 22
  float getso22() {
    return _setting["so22"].toFloat();
  }
  bool setso22(float val=1) {
    if(val >= FLOAT_MIN && val <= FLOAT_MAX) {
      _setting["so22"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Board Rotation X
  float getRotX() {
    return _setting["rotx"].toFloat();
  }
  bool setRotX(float val=0) {
    if(val >= -360 && val <= 360) {
      _setting["rotx"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Board Rotation Y
  float getRotY() {
    return _setting["roty"].toFloat();
  }
  bool setRotY(float val=0) {
    if(val >= -360 && val <= 360) {
      _setting["roty"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Board Rotation Z
  float getRotZ() {
    return _setting["rotz"].toFloat();
  }
  bool setRotZ(float val=0) {
    if(val >= -360 && val <= 360) {
      _setting["rotz"] = QString::number(val,'g',4);
      return true;
    }
    return false;
  }


  // Button Pin
  int8_t getButtonPin() {
    return _setting["buttonpin"].toInt();
  }
  bool setButtonPin(int8_t val=2) {
    if(val >= -1 && val <= 64) {
      _setting["buttonpin"] = val;
      return true;
    }
    return false;
  }

  // PPM Output Pin
  int8_t getPpmOutPin() {
    return _setting["ppmoutpin"].toInt();
  }
  bool setPpmOutPin(int8_t val=10) {
    if(val >= -1 && val <= 64) {
      _setting["ppmoutpin"] = val;
      return true;
    }
    return false;
  }

  // PPM Input Pin
  int8_t getPpmInPin() {
    return _setting["ppminpin"].toInt();
  }
  bool setPpmInPin(int8_t val=-1) {
    if(val >= -1 && val <= 64) {
      _setting["ppminpin"] = val;
      return true;
    }
    return false;
  }

  // Uart Mode (0- Off, 1-SBUS, 2-CRSFIN, 3-CRSFOUT)
  uint8_t getUartMode() {
    return _setting["uartmode"].toUInt();
  }
  bool setUartMode(uint8_t val=0) {
    if(val <= 3) {
      _setting["uartmode"] = val;
      return true;
    }
    return false;
  }

  // SBUS Transmit Freqency
  uint8_t getSbRate() {
    return _setting["sbrate"].toUInt();
  }
  bool setSbRate(uint8_t val=80) {
    if(val >= 30 && val <= 140) {
      _setting["sbrate"] = val;
      return true;
    }
    return false;
  }

  // SBUS Receieve Inverted
  bool getSbInInv() {return _setting["sbininv"].toBool();}
  void setSbInInv(bool val=true) { _setting["sbininv"] = val; }

  // SBUS Transmit Inverted
  bool getSbOutInv() {return _setting["sboutinv"].toBool();}
  void setSbOutInv(bool val=true) { _setting["sboutinv"] = val; }

  // Bluetooth Mode (0-Off, 1- Head, 2-Receive, 3-Scanner)
  uint8_t getBtMode() {
    return _setting["btmode"].toUInt();
  }
  bool setBtMode(uint8_t val=0) {
    if(val <= 3) {
      _setting["btmode"] = val;
      return true;
    }
    return false;
  }

  // Reset on Proximity Sense
  bool getRstOnWave() {return _setting["rstonwave"].toBool();}
  void setRstOnWave(bool val=false) { _setting["rstonwave"] = val; }

  // Long Press on the Button to Enable/Disable Tilt Roll and Pan
  bool getButLngPs() {return _setting["butlngps"].toBool();}
  void setButLngPs(bool val=false) { _setting["butlngps"] = val; }

  // Reset Center on a Head Tilt
  bool getRstOnTlt() {return _setting["rstontlt"].toBool();}
  void setRstOnTlt(bool val=false) { _setting["rstontlt"] = val; }

  // Invert PPM Output
  bool getPpmOutInvert() {return _setting["ppmoutinvert"].toBool();}
  void setPpmOutInvert(bool val=false) { _setting["ppmoutinvert"] = val; }

  // Invert PPM Output
  bool getPpmInInvert() {return _setting["ppmininvert"].toBool();}
  void setPpmInInvert(bool val=false) { _setting["ppmininvert"] = val; }

  // PPM Frame Length (us)
  uint16_t getPpmFrame() {
    return _setting["ppmframe"].toUInt();
  }
  bool setPpmFrame(uint16_t val=22500) {
    if(val >= PPM_MIN_FRAME && val <= PPM_MAX_FRAME) {
      _setting["ppmframe"] = val;
      return true;
    }
    return false;
  }

  // PPM Sync Pulse Length (us)
  uint16_t getPpmSync() {
    return _setting["ppmsync"].toUInt();
  }
  bool setPpmSync(uint16_t val=350) {
    if(val >= 100 && val <= 800) {
      _setting["ppmsync"] = val;
      return true;
    }
    return false;
  }

  // PPM channels to output
  uint8_t getPpmChCnt() {
    return _setting["ppmchcnt"].toUInt();
  }
  bool setPpmChCnt(uint8_t val=8) {
    if(val >= 1 && val <= 16) {
      _setting["ppmchcnt"] = val;
      return true;
    }
    return false;
  }

  // Low Pass filter For Pan
  uint8_t getLpPan() {
    return _setting["lppan"].toUInt();
  }
  bool setLpPan(uint8_t val=100) {
    if(val >= 1 && val <= 100) {
      _setting["lppan"] = val;
      return true;
    }
    return false;
  }

  // Low Pass filter For Tilt + Roll
  uint8_t getLpTiltRoll() {
    return _setting["lptiltroll"].toUInt();
  }
  bool setLpTiltRoll(uint8_t val=100) {
    if(val >= 1 && val <= 100) {
      _setting["lptiltroll"] = val;
      return true;
    }
    return false;
  }

  // Bluetooth Remote address to Pair With
  QString getBtPairedAddress() {
    return _setting["btpairedaddress"].toString();
  }
  void setBtPairedAddress(const QString &val) {
    _setting["btpairedaddress"] = val;
  }

  // Raw Sensor Mag X
  float getDataMagX() { return _data["magx"].toFloat(); }

  // Raw Sensor Mag Y
  float getDataMagY() { return _data["magy"].toFloat(); }

  // Raw Sensor Mag Z
  float getDataMagZ() { return _data["magz"].toFloat(); }

  // Raw Sensor Gyro X
  float getDataGyroX() { return _data["gyrox"].toFloat(); }

  // Raw Sensor Gyro Y
  float getDataGyroY() { return _data["gyroy"].toFloat(); }

  // Raw Sensor Gyro Z
  float getDataGyroZ() { return _data["gyroz"].toFloat(); }

  // Raw Sensor Accel X
  float getDataAccX() { return _data["accx"].toFloat(); }

  // Raw Sensor Accel Y
  float getDataAccY() { return _data["accy"].toFloat(); }

  // Raw Sensor Accel Z
  float getDataAccZ() { return _data["accz"].toFloat(); }

  // Calibrated Mag X
  float getDataOff_MagX() { return _data["off_magx"].toFloat(); }

  // Calibrated Mag Y
  float getDataOff_MagY() { return _data["off_magy"].toFloat(); }

  // Calibrated Mag Z
  float getDataOff_MagZ() { return _data["off_magz"].toFloat(); }

  // Calibrated Gyro X
  float getDataOff_GyroX() { return _data["off_gyrox"].toFloat(); }

  // Calibrated Gyro Y
  float getDataOff_GyroY() { return _data["off_gyroy"].toFloat(); }

  // Calibrated Gyro Z
  float getDataOff_GyroZ() { return _data["off_gyroz"].toFloat(); }

  // Calibrated Accel X
  float getDataOff_AccX() { return _data["off_accx"].toFloat(); }

  // Calibrated Accel Y
  float getDataOff_AccY() { return _data["off_accy"].toFloat(); }

  // Calibrated Accel Z
  float getDataOff_AccZ() { return _data["off_accz"].toFloat(); }

  // Board Tilt Output (us)
  uint16_t getDataTiltOut() { return _data["tiltout"].toUInt(); }

  // Board Roll Output (us)
  uint16_t getDataRollOut() { return _data["rollout"].toUInt(); }

  // Board Pan Output (us)
  uint16_t getDataPanOut() { return _data["panout"].toUInt(); }

  // Has the board been Calibrated
  bool getDataIsCal() { return _data["iscal"].toBool(); }

  // Bluetooth is connected
  bool getDataBtCon() { return _data["btcon"].toBool(); }

  // Tilt/Roll/Pan Output Enabled
  bool getDataTrpEnabled() { return _data["trpenabled"].toBool(); }

  // Raw Tilt in Degrees
  float getDataTilt() { return _data["tilt"].toFloat(); }

  // Raw Roll in Degrees
  float getDataRoll() { return _data["roll"].toFloat(); }

  // Raw Pan in Degrees
  float getDataPan() { return _data["pan"].toFloat(); }

  // Offset Tilt in Degrees
  float getDataTiltOff() { return _data["tiltoff"].toFloat(); }

  // Offset Roll in Degrees
  float getDataRollOff() { return _data["rolloff"].toFloat(); }

  // Offset Pan in Degrees
  float getDataPanOff() { return _data["panoff"].toFloat(); }

  // Is Board a Nano33 Sense Variant
  bool getDataisSense() { return _data["issense"].toBool(); }

  // Local Bluetooth Address
  QString getDataBtAddr() { return _data["btaddr"].toString(); }

  // Remote Bluetooth Address
  QString getDataBtRmt() { return _data["btrmt"].toString(); }

  QStringList allDataItems() {
    QStringList rv;
    rv.append("magx");
    rv.append("magy");
    rv.append("magz");
    rv.append("gyrox");
    rv.append("gyroy");
    rv.append("gyroz");
    rv.append("accx");
    rv.append("accy");
    rv.append("accz");
    rv.append("off_magx");
    rv.append("off_magy");
    rv.append("off_magz");
    rv.append("off_gyrox");
    rv.append("off_gyroy");
    rv.append("off_gyroz");
    rv.append("off_accx");
    rv.append("off_accy");
    rv.append("off_accz");
    rv.append("tiltout");
    rv.append("rollout");
    rv.append("panout");
    rv.append("iscal");
    rv.append("btcon");
    rv.append("trpenabled");
    rv.append("tilt");
    rv.append("roll");
    rv.append("pan");
    rv.append("tiltoff");
    rv.append("rolloff");
    rv.append("panoff");
    rv.append("issense");
    rv.append("chout[0]");
    rv.append("chout[1]");
    rv.append("chout[2]");
    rv.append("chout[3]");
    rv.append("chout[4]");
    rv.append("chout[5]");
    rv.append("chout[6]");
    rv.append("chout[7]");
    rv.append("chout[8]");
    rv.append("chout[9]");
    rv.append("chout[10]");
    rv.append("chout[11]");
    rv.append("chout[12]");
    rv.append("chout[13]");
    rv.append("chout[14]");
    rv.append("chout[15]");
    rv.append("btch[0]");
    rv.append("btch[1]");
    rv.append("btch[2]");
    rv.append("btch[3]");
    rv.append("btch[4]");
    rv.append("btch[5]");
    rv.append("btch[6]");
    rv.append("btch[7]");
    rv.append("ppmch[0]");
    rv.append("ppmch[1]");
    rv.append("ppmch[2]");
    rv.append("ppmch[3]");
    rv.append("ppmch[4]");
    rv.append("ppmch[5]");
    rv.append("ppmch[6]");
    rv.append("ppmch[7]");
    rv.append("ppmch[8]");
    rv.append("ppmch[9]");
    rv.append("ppmch[10]");
    rv.append("ppmch[11]");
    rv.append("ppmch[12]");
    rv.append("ppmch[13]");
    rv.append("ppmch[14]");
    rv.append("ppmch[15]");
    rv.append("uartch[0]");
    rv.append("uartch[1]");
    rv.append("uartch[2]");
    rv.append("uartch[3]");
    rv.append("uartch[4]");
    rv.append("uartch[5]");
    rv.append("uartch[6]");
    rv.append("uartch[7]");
    rv.append("uartch[8]");
    rv.append("uartch[9]");
    rv.append("uartch[10]");
    rv.append("uartch[11]");
    rv.append("uartch[12]");
    rv.append("uartch[13]");
    rv.append("uartch[14]");
    rv.append("uartch[15]");
    rv.append("quat[0]");
    rv.append("quat[1]");
    rv.append("quat[2]");
    rv.append("quat[3]");
    rv.append("btaddr");
    rv.append("btrmt");
    return rv;
  }
protected:
  QVariantMap _setting; // Data in the GUI
  QVariantMap _deviceSettings; // Data stored on the device
  QVariantMap _data; // Realtime data
  QMap<QString, bool> _dataItems; // Data items that requested to be sent
  QMap<QString, bool> _deviceDataItems; // What items the device is currently sending

  
};
