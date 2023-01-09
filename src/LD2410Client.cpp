/**
 * Homie Node for LD2410 mmWave Radar Motion Sensor
 * 
 */
#include "LD2410Client.hpp"

LD2410Client::LD2410Client(const char *id, const char *name, const char *nType, const uint8_t rxPin, const uint8_t txPin, const uint8_t ioPin, const bool enableReporting)
    : HomieNode(id, name, nType, false, 0U, 0U),
    _rxPin(rxPin),
    _txPin(txPin),
    _ioPin(ioPin),
    _reporting_enabled(enableReporting)
{
  // Start up the library
  pinMode(_ioPin, INPUT);
}

/**
 * Handles the received MQTT messages from Homie.
 *
 */
bool LD2410Client::handleInput(const HomieRange &range, const String &property, const String &value)
{
    Homie.getLogger() << "LD2410 InputHandler()  " << property << ": " << value << endl;

    if (property.equalsIgnoreCase(cPropertyCommand))
    {
      String _value = value;
      String _response;

      serializeJson(commandProcessor( _value ), _response);
      Homie.getLogger() << cIndent
                        << F("✖Command Response: ") << value << "\n"
                        << _response
                        << endl;
      setProperty(cPropertyCommand).setRetained(false).send(_response.c_str());
    }
   
    return true;
}

/**
 *
 */
void LD2410Client::onReadyToOperate()
{
  _motion = false;

  Homie.getLogger() << cCaption << endl;
  Homie.getLogger() << cIndent << "onReadyToOperate()" << endl;
}

/**
 * Called by Homie when Homie.setup() is called; Once!
*/
void LD2410Client::setup() {
  Homie.getLogger() << cCaption << endl;

  // start path to LD2410
  // radar.debug(Serial);  // enable debug output to console
  Serial2.begin(256000, SERIAL_8N1, _rxPin, _txPin); // UART for monitoring the radar rx, tx  

  if (radar.begin(Serial2)) {
    Homie.getLogger() << cCaption << " Initialized..." << endl;
  } else {
    Homie.getLogger() << cCaption << " was not connected" << endl;
  }

  advertise(cPropertyMotion)
      .setName(cPropertyMotionName)
      .setDatatype(cPropertyMotionDataType)
      .setFormat(cPropertyMotionFormat)
      .setUnit(cPropertyMotionUnit);

  advertise(cPropertyOccupancy)
      .setName(cPropertyOccupancyName)
      .setDatatype(cPropertyMotionDataType)
      .setFormat(cPropertyOccupancyDataType)
      .setUnit(cPropertyOccupancyFormat);

  advertise(cPropertyStatus)
      .setName(cPropertyStatusName)
      .setDatatype(cPropertyStatusDataType)
      .setFormat(cPropertyStatusFormat);

  advertise(cPropertyCommand)
      .setName(cPropertyCommandName)
      .setDatatype(cPropertyCommandDataType)
      .setFormat(cPropertyCommandFormat)
      .settable();

}

/**
 * Called by Homie when homie is connected and in run mode
*/
void LD2410Client::loop() {
  uint32_t timer = millis();
  radar.ld2410_loop();
  
  // Module controls hold time, so all we need is to read the io pin
  if ((_motion != digitalRead(_ioPin)) || ((timer - _lastBroadcast) > _broadcastInterval)) {
        
    _lastBroadcast = timer;
    _motion = digitalRead(_ioPin);

    // process target reporting data
    if (_reporting_enabled) {
      String pData = processTargetData();
      Homie.getLogger() << processTargetData() << endl;      
      setProperty(cPropertyStatus).setRetained(false).send(pData.c_str());
    }

    if (radar.isMoving() && radar.isStationary()) {
        Homie.getLogger() << cIndent
                          << F("✖ Motion Detected: ON ")
                          << endl;
        Homie.getLogger() << cIndent
                          << F("✖ Occupanncy Detected: ON ")
                          << endl;
      setProperty(cPropertyMotion).setRetained(true).send("ON");
      setProperty(cPropertyOccupancy).setRetained(true).send("ON");

    } else {    
      if (radar.isMoving()) {
        Homie.getLogger() << cIndent
                          << F("✖ Motion Detected: ON ")
                          << endl;
        setProperty(cPropertyMotion).setRetained(true).send("ON");
      } else {
        Homie.getLogger() << cIndent
                          << F("✖ Motion Detected: OFF")
                          << endl;
        setProperty(cPropertyMotion).setRetained(true).send("OFF");
      }
      if (radar.isStationary()) {
        Homie.getLogger() << cIndent
                          << F("✖ Occupanncy Detected: ON ")
                          << endl;
        setProperty(cPropertyOccupancy).setRetained(true).send("ON");
      } else {
        Homie.getLogger() << cIndent
                          << F("✖ Occupanncy Detected: OFF ")
                          << endl;
        setProperty(cPropertyOccupancy).setRetained(true).send("OFF");
      }
    }
  }

  // Read from Serial, look for commands
  commandHandler();
}

// clang-format off
/*
 * Available Commands 
 * [
 *   { 
 *     id: 1,
 *     cmd: "help",
 *     description: "list of available commands"
 *   },...
 * ]
 */
DynamicJsonDocument LD2410Client::availableCommands() {
  DynamicJsonDocument doc(1024);
  JsonArray jCmd = doc.createNestedArray("Commands");

  JsonObject item = jCmd.createNestedObject();
  item["id"] = 1;
  item["command"]= "help";
  item["description"] = "this text";

  JsonObject item2 = jCmd.createNestedObject();
  item2["id"] = 2;
  item2["command"]= "streamstart";
  item2["description"] = "start sending udp data to SerialStudio.";

  JsonObject item3 = jCmd.createNestedObject();
  item3["id"] = 3;
  item3["command"]= "streamstop";
  item3["description"] = "stop sending to SerialStream.";

  JsonObject item4 = jCmd.createNestedObject();
  item4["id"] = 4;
  item4["command"]= "read";
  item4["description"] = "read current values from the sensor";

  JsonObject item5 = jCmd.createNestedObject();
  item5["id"] = 5;
  item5["command"]= "readconfig";
  item5["description"] = "read the configuration from the sensor";

  JsonObject item6 = jCmd.createNestedObject();
  item6["id"] = 6;
  item6["command"]= "setmaxvalues";
  item6["param1"]="<motion gate> (2-8)";
  item6["param2"]="<stationary gate> (2-8)";
  item6["param3"]="<inactivitytimer> (0-65535 seconds)";
  item6["description"] = "sets distance sensitivity and idle hold timer";

  JsonObject item7 = jCmd.createNestedObject();
  item7["id"] = 7;
  item7["command"]= "setsensitivity";
  item7["param1"]="<gate> (2-8|255)";
  item7["param2"]="<motionsensitivity> (0-100)";
  item7["param3"]="<stationarysensitivity> (0-100)";
  item7["description"] = "set sensitivity for individual motion and stationary gate pairs, or all if gate=255";

  JsonObject item8 = jCmd.createNestedObject();
  item8["id"] = 8;
  item8["command"]= "restart";
  item8["description"] = "restart the sensor";

  JsonObject item9 = jCmd.createNestedObject();
  item9["id"] = 9;
  item9["command"]= "readversion";
  item9["description"] = "read firmware version";

  JsonObject item10 = jCmd.createNestedObject();
  item10["id"] = 10;
  item10["command"]= "factoryreset";
  item10["description"] = "factory reset the sensor";

  JsonObject item11 = jCmd.createNestedObject();
  item11["id"] = 11;
  item11["command"]= "deviceinfo";
  item11["description"] = "LD2410 device info";

  JsonObject item12 = jCmd.createNestedObject();
  item12["id"] = 12;
  item12["command"]= "reboot";
  item12["description"] = "reboot hosting micro-controller";

  return doc;
}


// clang-format on


/*
 * JSON Command Processor
 * - there are two commands not implemented here
 * - requestConfigurationModeBegin()
 * - requestConfigurationModeEnd()
 * Otherwise all commands are available as options
 */
DynamicJsonDocument LD2410Client::commandProcessor(String &cmdStr) {
  DynamicJsonDocument jCMD(1024);
  int iCmd = cmdStr.toInt();
  cmdStr.trim();

  if (cmdStr.equals("help") || iCmd == 1) {
    JsonObject response = jCMD.createNestedObject("HelpResponse");
    response["success"] = true; 
    response["message"] = "Device online";
    response["data"] = availableCommands();

  } else if (cmdStr.equals("streamstart") || iCmd == 2) {
    _reporting_enabled = true;
    JsonObject response = jCMD.createNestedObject("StreamStartResponse");
    response["success"] = true; 
    response["message"] = "Device online";
    JsonObject data = response.createNestedObject("data");

  } else if (cmdStr.equals("streamstop") || iCmd == 3) {
    _reporting_enabled = false;
    JsonObject response = jCMD.createNestedObject("StreamStopResponse");
    response["success"] = true; 
    response["message"] = "Device online";
    JsonObject data = response.createNestedObject("data");

  } else if (cmdStr.equals("read") || iCmd == 4) {
    JsonObject response = jCMD.createNestedObject("ReadResponse");
    if (radar.isConnected()) {
      response["success"] = true; 
      response["message"] = "Device online";
      if (radar.presenceDetected()) {
        response["message"] = "Presence Detected";
        JsonObject item = response.createNestedObject("data");
        if (radar.stationaryTargetDetected() &&
            radar.movingTargetDetected()) {
          item["unitOfMeasure"] = "feet";
          item["target"] = "Stationary and Moving";
         
          item["stationaryTargetDistance"] = String((radar.stationaryTargetDistance() * CM_TO_FEET_FACTOR), 1);
          item["stationaryTargetEnergy"] = radar.stationaryTargetEnergy();
          item["movingTargetDistance"] =  String((radar.movingTargetDistance() * CM_TO_FEET_FACTOR), 1);
          item["movingTargetEnergy"] = radar.movingTargetEnergy();

        } else if (radar.stationaryTargetDetected()) {
          item["target"] = "Stationary";
          item["stationaryTargetDistance"] = String((radar.stationaryTargetDistance() * CM_TO_FEET_FACTOR), 1);
          item["stationaryTargetEnergy"] = radar.stationaryTargetEnergy();

        } else if (radar.movingTargetDetected()) {
          item["target"] = "Moving";
          item["movingTargetDistance"] = String((radar.movingTargetDistance() * CM_TO_FEET_FACTOR), 1);
          item["movingTargetEnergy"] = radar.movingTargetEnergy();
        }
      } else {
        response["message"] = "Presence not detected";
        JsonObject data = response.createNestedObject("data");
      }
    } else {
      response["success"] = false; 
      response["message"] = "Device offline";
      JsonObject data = response.createNestedObject("data");
    }
  } else if (cmdStr.equals("readconfig") || iCmd == 5) {
     JsonObject response = jCMD.createNestedObject("ReadConfigResponse");
    if (radar.requestCurrentConfiguration()) {
      response["success"] = true; 
      response["message"] = "Device online";

      JsonObject item = response.createNestedObject("data");
      item["cfgMaxGate"] = radar.cfgMaxGate();
      item["cfgMaxMovingGate"] = radar.cfgMaxMovingGate();
      item["cfgMaxStationaryGate"] = radar.cfgMaxStationaryGate();
      item["cfgSensorIdleTimeInSeconds"] = radar.cfgSensorIdleTimeInSeconds();
      
      JsonObject cfg = item.createNestedObject("gateConfig");
      for (uint8_t gate = 0; gate < radar.cfgMaxGate(); gate++) {
         JsonObject jEle = cfg.createNestedObject(String(gate));   
         jEle["cfgMovingGateSensitivity"] = radar.cfgMovingGateSensitivity(gate);
         jEle["cfgStationaryGateSensitivity"] = radar.cfgStationaryGateSensitivity(gate);
      }
    } else {
      response["success"] = false; 
      response["message"] = "execution failure";
      JsonObject data = response.createNestedObject("data");
    }
  } else if (cmdStr.startsWith("setmaxvalues") || iCmd == 6) {
    uint8_t firstSpace = cmdStr.indexOf(' ');
    uint8_t secondSpace = cmdStr.indexOf(' ', firstSpace + 1);
    uint8_t thirdSpace = cmdStr.indexOf(' ', secondSpace + 1);
    JsonObject response = jCMD.createNestedObject("SetMaxValuesResponse");

    uint8_t newMovingMaxDistance =
        (cmdStr.substring(firstSpace, secondSpace)).toInt();
    uint8_t newStationaryMaxDistance =
        (cmdStr.substring(secondSpace, thirdSpace)).toInt();
    uint16_t inactivityTimer =
        (cmdStr.substring(thirdSpace, cmdStr.length())).toInt();

    if (newMovingMaxDistance > 0 && newStationaryMaxDistance > 0 &&
        newMovingMaxDistance <= 8 && newStationaryMaxDistance <= 8) {
      if (radar.setMaxValues(newMovingMaxDistance, newStationaryMaxDistance,inactivityTimer)) {
        response["message"] = "Device online";
        if (radar.requestRestart()) {
          delay(1500);
          if(radar.isEngineeringMode()) {
            radar.requestStartEngineeringMode();
          }
          response["success"] = true; 
        } else {
          response["success"] = false; 
          response["message"] = "restart cmd failure";
        }
      } else {
        response["success"] = false; 
        response["message"] = "command failure";
      }
    } else {
        response["success"] = false; 
        response["message"] = "invalid param failure";
    }
    JsonObject data = response.createNestedObject("data");
    
  } else if (cmdStr.startsWith("setsensitivity") || iCmd == 7) {
    uint8_t firstSpace = cmdStr.indexOf(' ');
    uint8_t secondSpace = cmdStr.indexOf(' ', firstSpace + 1);
    uint8_t thirdSpace = cmdStr.indexOf(' ', secondSpace + 1);
    JsonObject response = jCMD.createNestedObject("SetSensitivityResponse");

    uint8_t gate = (cmdStr.substring(firstSpace, secondSpace)).toInt();
    uint8_t motionSensitivity =
        (cmdStr.substring(secondSpace, thirdSpace)).toInt();
    uint8_t stationarySensitivity =
        (cmdStr.substring(thirdSpace, cmdStr.length())).toInt();

    // Command method 1 -- limit gate to 0-8 -- set one gate set
    // Command method 2 -- limit gate to 255 -- set all gates to same
    // sensitivity value
    if (motionSensitivity >= 0 && stationarySensitivity >= 0 &&
        motionSensitivity <= 100 && stationarySensitivity <= 100) {
      if (radar.setGateSensitivityThreshold(gate, motionSensitivity,stationarySensitivity)) {
        if (radar.requestRestart()) {
          delay(1500);
          if(radar.isEngineeringMode()) {
            radar.requestStartEngineeringMode();
          }
          response["success"] = true; 
          response["message"] = "Device online";
        } else {
          response["success"] = false; 
          response["message"] = "device restart failed";
        }
      } else {
        response["success"] = false; 
        response["message"] = "command execution failed";
      }
    } else {
      response["success"] = false; 
      response["message"] = "invalid command params";
    }
    JsonObject data = response.createNestedObject("data");

  } else if (cmdStr.equals("restart") || iCmd == 8) {
    JsonObject response = jCMD.createNestedObject("RestartResponse");
    if (radar.requestRestart()) {
      delay(1500);
      if(radar.isEngineeringMode()) {
        radar.requestStartEngineeringMode();
      }
      response["success"] = true; 
      response["message"] = "Device online";
    } else {
      response["success"] = false; 
      response["message"] = "device restart failed";
    }
    JsonObject data = response.createNestedObject("data");

  } else if (cmdStr.equals("readversion") || iCmd == 9) {
    JsonObject response = jCMD.createNestedObject("ReadVersionResponse");
    JsonObject data = response.createNestedObject("data");
    if (radar.requestFirmwareVersion()) {
      response["success"] = true; 
      response["message"] = "Device online";
      data["cmdFirmwareVersion"] = radar.cmdFirmwareVersion();
    } else {
      response["success"] = false; 
      response["message"] = "command execution failed";
    }

  } else if (cmdStr.equals("factoryreset") || iCmd == 10) {
    JsonObject response = jCMD.createNestedObject("FactoryResetResponse");
    JsonObject data = response.createNestedObject("data");
    if (radar.requestFactoryReset()) {
      response["success"] = true; 
      response["message"] = "Device online";
    } else {
      response["success"] = false; 
      response["message"] = "command execution failed";
    }

  } else if (cmdStr.equals("deviceinfo") || iCmd == 11) {
    JsonObject response = jCMD.createNestedObject("DeviceInfoResponse");
    JsonObject data = response.createNestedObject("data");
    
    response["success"] = true; 
    if(!radar.requestFirmwareVersion()) {
      response["success"] = true; 
    }
    response["message"] = "Device online";
    data["isEngineeringMode"] = radar.isEngineeringMode();
    data["cmdProtocolVersion"] = radar.cmdProtocolVersion();
    data["cmdCommunicationBufferSize"] = radar.cmdCommunicationBufferSize();
    data["cmdFirmwareVersion"] = radar.cmdFirmwareVersion();
    data["engRetainDataValue"] = radar.engRetainDataValue();
    
  } else if (cmdStr.equals("reboot") || iCmd == 12) {
    esp_restart();
  } else {
    JsonObject response = jCMD.createNestedObject("UnknownResponse");
    JsonObject data = response.createNestedObject("data");
    
    response["success"] = false; 
    response["message"] = "Unknown command";
  }

  cmdStr.clear();

  return jCMD;
}


/*
 * Accepts Serial chars and process chars as a command
 * when the newline char is received
 */
void LD2410Client::commandHandler() {
  if (Serial.available()) {
    char typedCharacter = Serial.read();
    if (typedCharacter == '\n') {
      serializeJsonPretty(commandProcessor( command ), Serial);
    } else {
      Serial.print(typedCharacter);
      if (typedCharacter != '\r') { // effectively ignore CRs
        command += typedCharacter;
      }
    }
  }
}

/*
 * Translate current status
*/
const char * LD2410Client::triggeredby() {

  if(radar.isMoving() && radar.isStationary()) {
    triggered = "Stationary and Moving";
  } else if (radar.isStationary()) {
    triggered = "Stationary";
  } else if(radar.isMoving()) {
    triggered = "Moving";
  } else {
    triggered = "Not Present";
  }

  return triggered.c_str();
}

String LD2410Client::processTargetData() {
  String strOut;
  if(radar.isEngineeringMode()) {
    serializeJson(processEngineeringReportData(), strOut);
  } else {
    serializeJson(processTargetReportingData(), strOut);
  }
  return strOut;
}

/*
 * Basic Target Reporting Data
*/
DynamicJsonDocument LD2410Client::processTargetReportingData() {
  DynamicJsonDocument jResponse(256);

  JsonObject item = jResponse.createNestedObject(LD_OCCUPANCY_TARGET_JSON);
  item["unitOfMeasure"] = "feet";
  item["triggeredBy"] = triggeredby();
  item["detectionDistance"] = String((radar.detectionDistance() * CM_TO_FEET_FACTOR), 1);
  item["movingTargetDistance"] = String((radar.movingTargetDistance() * CM_TO_FEET_FACTOR),1);
  item["movingTargetEnergy"] = radar.movingTargetEnergy();
  item["stationaryTargetDistance"] = String((radar.stationaryTargetDistance() * CM_TO_FEET_FACTOR), 1);
  item["stationaryTargetEnergy"] = radar.stationaryTargetEnergy();
  
  return jResponse;
}

/*
 * Engineering level Rreporting Data
*/
DynamicJsonDocument LD2410Client::processEngineeringReportData() {
  DynamicJsonDocument jResponse(756);

  JsonObject item = jResponse.createNestedObject(LD_OCCUPANCY_ENGINEERING_JSON);
  item["unitOfMeasure"]             = "feet";
  item["triggeredBy"]               = triggeredby();
  item["detectionDistance"]         = String((radar.detectionDistance() * CM_TO_FEET_FACTOR), 1);
  item["movingTargetDistance"]      = String((radar.movingTargetDistance() * CM_TO_FEET_FACTOR), 1);
  item["movingTargetEnergy"]        = radar.movingTargetEnergy();
  item["stationaryTargetDistance"]  = String((radar.stationaryTargetDistance() * CM_TO_FEET_FACTOR), 1);
  item["stationaryTargetEnergy"]    = radar.stationaryTargetEnergy();
  item["maxMovingDistanceGate"]     = radar.engMaxMovingDistanceGate();
  item["maxStationaryDistanceGate"] = radar.engMaxStaticDistanceGate();

  JsonObject jGates = item.createNestedObject("gates");

  for (int x = 0; x < radar.cfgMaxGate(); ++x) {
    JsonObject gate = jGates.createNestedObject(String(x));    
    gate["movingDistanceGateEnergy"]     = radar.engMovingDistanceGateEnergy(x);
    gate["movingSensitivity"]            = radar.cfgMovingGateSensitivity(x);
    gate["StationaryDistanceGateEnergy"] = radar.engStaticDistanceGateEnergy(x);
    gate["StationarySensitivity"]        = radar.cfgStationaryGateSensitivity(x);
  }

  return jResponse;
}
