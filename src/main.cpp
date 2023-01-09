/**
 * @file main.cpp
 * @author James Scott, Jr.  aka(Skoona) (skoona@gmail.com)
 * @brief HomieNode featuring an DHT (11/22) Temperature sensor.
 * @version 1.0.0
 * @date 2021-01-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>
#include "DHTNode.hpp"
#include "LD2410Client.hpp"

#ifdef ESP8266
extern "C"
{
#include "user_interface.h" // to set CPU Freq for Non-Huzzah's
}
#endif

/*
 * SKN_NODE_ID becomes the base name when ranges are used
 * ex: sknSensors/deviceName/DHT_0/temperature -> 72.3 degress
 * Note: HomieNode(...range,lower,upper) manages this array suffix change; i.e no more name fixups
*/
#define SKN_MOD_NAME "MultiSensor"
#define SKN_MOD_VERSION "3.1.0"
#define SKN_MOD_BRAND "SknSensors"

#define SKN_TNODE_TITLE "Temperature and Humidity Sensor"
#define SKN_MNODE_TITLE "Occupancy and Motion Sensor"

#define SKN_TNODE_TYPE "Sensor"
#define SKN_MNODE_TYPE "Sensor"

#define SKN_TNODE_ID "Ambient"    
#define SKN_MNODE_ID "Occupancy"

// Select pins for Temperature and Motion
#define PIN_DHT  4         // D4  4
#define LD_IO   18         // D18        
#define LD_RX   16         // D6
#define LD_TX   17         // D7
#define DHT_TYPE DHTesp::DHT_MODEL_t::DHT22 // DHTesp::DHT22

#define SENSOR_READ_INTERVAL 300
#define LD2410_TARGET_REPORTING true   // enables status property


HomieSetting<long> sensorsIntervalSetting("sensorInterval", "The interval in seconds to wait between sending commands.");
HomieSetting<long> dhtType("dhtType", "Type os Humidty Sensor where; DHTesp::DHT_MODEL_t::DHT11 = 1. DHTesp::DHT_MODEL_t::DHT22 = 2");
HomieSetting<long> broadcastInterval("broadcastInterval", "how frequently to send presence status in milliseconds.");
HomieSetting<boolean> modeEngineering("engineeringMode", "Use engineering mode for expanded reporting data");

DHTNode temperatureMonitor(PIN_DHT, DHT_TYPE, SKN_TNODE_ID, SKN_TNODE_TITLE, SKN_TNODE_TYPE, SENSOR_READ_INTERVAL);
LD2410Client  occupancyMonitor(SKN_MNODE_ID, SKN_MNODE_TITLE, SKN_MNODE_TYPE, LD_RX, LD_TX, LD_IO, LD2410_TARGET_REPORTING);

bool bRunOnce = true;

/* 
 * Settings are not read until after setup() runs
 * so value would only return default values
 * this once function overcomes that short fall
*/
void readyToOperate() {
  if(bRunOnce) {
    bRunOnce = false;
    temperatureMonitor.setMeasurementInterval(sensorsIntervalSetting.get());
    temperatureMonitor.setModel((DHTesp::DHT_MODEL_t)dhtType.get());
    occupancyMonitor.setBroadcastInterval(broadcastInterval.get());
    occupancyMonitor.setEngineeringModeTargetReporting(modeEngineering.get());
  }
}

void setup()
{  
  delay(250);  
  Serial.begin(115200);
  delay(250);  
  Serial << endl << "Starting..." << endl;

  sensorsIntervalSetting.setDefaultValue(180).setValidator([](long candidate) {
    return (candidate >= 10) && (candidate <= 1200);
  });
  dhtType.setDefaultValue(2).setValidator([](long candidate) {
    return (candidate >= 0) && (candidate <= 4);
  });
  broadcastInterval.setDefaultValue(5000).setValidator([](long candidate) {
    return (candidate >= 5000) && (candidate <= 128000);
  });
  modeEngineering.setDefaultValue(false).setValidator([](boolean candidate) {
    return true; // validation only, not the value
  });
  
  Homie_setFirmware(SKN_MOD_NAME, SKN_MOD_VERSION);
  Homie_setBrand(SKN_MOD_BRAND);

  Homie.setup();
}

void loop()
{
  readyToOperate();
  Homie.loop();
}
