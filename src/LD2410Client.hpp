/**
 * Homie Node for LD2410 mmWave Radar Motion Sensor
 * 
 * GPIO pin for RCWL should have a pull down resistor to keep inactive state low
 * GPIO 16 has the INPUT_PULLDOWN_16 capability, but it is also the wakeup pin
 * 
 */

#pragma once

#include <Homie.hpp>
#include <ld2410.h>
#include <ArduinoJson.h>
#include <Preferences.h>
    

#define CM_TO_FEET_FACTOR  0.0328084
#define LD_OCCUPANCY_TARGET_JSON "TargetReport"
#define LD_OCCUPANCY_ENGINEERING_JSON "EngineeringTargetReport"

class LD2410Client : public HomieNode {

public:

  LD2410Client(const char *id, const char *name, const char *nType, const uint8_t rxPin, const uint8_t txPin, const uint8_t ioPin, const bool enableReporting = false);

  void setTargetReporting(bool enabled) { _reporting_enabled = enabled; }
  bool isTargetReportingEnabled() const { return _reporting_enabled; }
  void setEngineeringModeTargetReporting(bool enabled) { _engineering_mode = enabled; if(_engineering_mode) {radar.requestStartEngineeringMode();} }
  bool isEngineeringModeTargetReporting() const { return _engineering_mode; }
  
protected:
  virtual void setup() override;
  virtual void loop() override;
  virtual void onReadyToOperate() override;
  virtual bool handleInput(const HomieRange &range, const String &property, const String &value);
  
  void commandHandler();
  DynamicJsonDocument commandProcessor(String &cmdStr);
  DynamicJsonDocument availableCommands();
  String processTargetData();
  DynamicJsonDocument processTargetReportingData();
  DynamicJsonDocument processEngineeringReportData();
  const char * triggeredby();

  bool initSensor(bool debug);
  unsigned long loadBaudRate();
  unsigned long saveBaudRate(uint8_t baudIndex);
  unsigned long serialSpeed(uint8_t choice);

private:
  const char *cCaption = "LD2410 mmWave Radar Motion Sensor:";
  const char* cIndent  = "  ◦ ";

  // Motion Node Properties
  const uint8_t _rxPin;
  const uint8_t _txPin;
  const uint8_t _ioPin;
  bool _reporting_enabled = false;
  bool _engineering_mode = false;

  const char *cPropertyMotion = "motion";
  const char *cPropertyMotionName = "Motion";
  const char *cPropertyMotionDataType = "enum";
  const char *cPropertyMotionFormat = "ON,OFF";
  const char *cPropertyMotionUnit = "";

  const char *cPropertyOccupancy = "occupancy";
  const char *cPropertyOccupancyName = "Occupancy";
  const char *cPropertyOccupancyDataType = "enum";
  const char *cPropertyOccupancyFormat = "ON,OFF";

  const char *cPropertyCommand = "system";
  const char *cPropertyCommandName = "Command Handler";
  const char *cPropertyCommandDataType = "enum";
  const char *cPropertyCommandFormat = "1:12,help,streamStart,streamStop,read,readConfig,setMaxValues,setSensitivity,restart,readVersion,factoryReset,deviceInfo,rebootNode";

  const char *cPropertyStatus = "deviceStatus";
  const char *cPropertyStatusName = "Device Status";
  const char *cPropertyStatusDataType = "json";
  const char *cPropertyStatusFormat = "";

  // Command Processor Parms
  String command = "";
  String triggered = "";

  // LD2410 Interface
  volatile bool _motion = false;
  unsigned long _savedBaudRate = 256000;
  ld2410 radar;
  Preferences prefs;

};
