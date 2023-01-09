# EnvironmentMonitor_DHT
<a href="https://homieiot.github.io/">
  <img src="https://homieiot.github.io/img/works-with-homie.png" alt="works with MQTT Homie">
</a>

HomieNode V3 featuring:
- DHT Temperature Class, using HomeNode with auto detect sensor of DHT11, DHT22, AM2302, RHT03.
- LD2410 mmWave Radar Motion Class, using HomieNode.
  - Can be configured using the "system" channel

Pin Out for ESP32
- #define PIN_DHT  4
- #define LD_IO   18
- #define LD_RX   16
- #define LD_TX   17
- Power LD2410 at 5VDC (100uf capacitor is helpful)
- Power DHT at 3.3VDC


### Homie Config

```
{
  "name": "Environment Monitor",
  "device_id": "<device-location-name>",
  "device_stats_interval": 900,  
  "wifi": {
    "ssid": "<wifi-host>",
    "password": "<wifi-password>"
  },
  "mqtt": {
    "host": "<mqtt-hostname-or-ip>",
    "port": 1883,
	"base_topic": "sknSensors/",
    "auth": true,
    "username": "<mqtt-username>",
    "password": "<mqtt-password>"
  },
  "ota": {
    "enabled": true
  },
  "settings": {
      "sensorInterval": 900,
      "broadcastInterval": 30000,
      "engineeringMode": true,
		  "dhtType": 1 
    }
}
```


### Home MQTT Log
```
sknSensors/HomeOffice/$state init
sknSensors/HomeOffice/$homie 3.0.1
sknSensors/HomeOffice/$name Home Office
sknSensors/HomeOffice/$mac 24:0A:C4:59:42:54
sknSensors/HomeOffice/$localip 10.100.1.244
sknSensors/HomeOffice/$nodes Ambient,Occupancy
sknSensors/HomeOffice/$stats uptime
sknSensors/HomeOffice/$stats/interval 185
sknSensors/HomeOffice/$fw/name MultiSensor
sknSensors/HomeOffice/$fw/version 3.0.1
sknSensors/HomeOffice/$fw/checksum ed1a289e4f7a38e730cc202a8bc4004b
sknSensors/HomeOffice/$implementation esp32
sknSensors/HomeOffice/$implementation/config {"name":"Home Office","device_id":"HomeOffice","device_stats_interval":180,"wifi":{"ssid":"SFNSS1-24G"},"mqtt":{"host":"openhabianpi.local.skoona.net","port":1883,"base_topic":"sknSensors/","auth":true},"ota":{"enabled":true},"settings":{"sensorInterval":900,"targetReportingInterval":60000,"broadcastInterval":60000,"dhtType":2}}
sknSensors/HomeOffice/$implementation/version 3.0.0
sknSensors/HomeOffice/$implementation/ota/enabled true
sknSensors/HomeOffice/Ambient/$name Temperature and Humidity Sensor
sknSensors/HomeOffice/Ambient/$type Sensor
sknSensors/HomeOffice/Ambient/$properties humidity,temperature
sknSensors/HomeOffice/Ambient/humidity/$name Humidity
sknSensors/HomeOffice/Ambient/humidity/$datatype float
sknSensors/HomeOffice/Ambient/humidity/$unit %rH
sknSensors/HomeOffice/Ambient/temperature/$name Temperature
sknSensors/HomeOffice/Ambient/temperature/$datatype float
sknSensors/HomeOffice/Ambient/temperature/$unit °F
sknSensors/HomeOffice/Occupancy/$name Occupancy and Motion Sensor
sknSensors/HomeOffice/Occupancy/$type Sensor
sknSensors/HomeOffice/Occupancy/$properties motion,occupancy,system
sknSensors/HomeOffice/Occupancy/motion/$name Motion
sknSensors/HomeOffice/Occupancy/motion/$datatype enum
sknSensors/HomeOffice/Occupancy/motion/$format ON,OFF
sknSensors/HomeOffice/Occupancy/occupancy/$name Occupancy
sknSensors/HomeOffice/Occupancy/occupancy/$datatype json
sknSensors/HomeOffice/Occupancy/system/$name Command Handler
sknSensors/HomeOffice/Occupancy/system/$settable true
sknSensors/HomeOffice/Occupancy/system/$datatype string
sknSensors/HomeOffice/$state ready
sknSensors/HomeOffice/$stats/interval 185
sknSensors/HomeOffice/$stats/signal 84
sknSensors/HomeOffice/$stats/uptime 4
sknSensors/HomeOffice/$stats/freeheap 220336

sknSensors/HomeOffice/Occupancy/system/set 1
sknSensors/HomeOffice/Occupancy/system {"HelpResponse":{"success":true,"message":"Device online","data":{"Commands":[{"id":1,"command":"help","description":"this text"},{"id":2,"command":"streamstart","description":"start sending udp data to SerialStudio."},{"id":3,"command":"streamstop","description":"stop sending to SerialStream."},{"id":4,"command":"read","description":"read current values from the sensor"},{"id":5,"command":"readconfig","description":"read the configuration from the sensor"},{"id":6,"command":"setmaxvalues","param1":"<motion gate> (2-8)","param2":"<stationary gate> (2-8)","param3":"<inactivitytimer> (0-65535 seconds)","description":"sets distance sensitivity and idle hold timer"},{"id":7,"command":"setsensitivity","param1":"<gate> (2-8|255)","param2":"<motionsensitivity> (0-100)","param3":"<stationarysensitivity> (0-100)","description":"set sensitivity for individual motion and stationary gate pairs, or all if gate=255"},{"id":8,"command":"restart","description":"restart the sensor"},{"id":9,"command":"readversion","description":"read firmware version"},{"id":10,"command":"factoryreset","description":"factory reset the sensor"},{"id":11,"command":"deviceinfo","description":"LD2410 device info"},{"id":12,"command":"reboot","description":"reboot hosting micro-controller"}]}}}

sknSensors/HomeOffice/Occupancy/system/set 2
sknSensors/HomeOffice/Occupancy/system {"StreamStartResponse":{"success":true,"message":"Device online","data":{}}}

sknSensors/HomeOffice/Occupancy/system/set 3
sknSensors/HomeOffice/Occupancy/system {"StreamStopResponse":{"success":true,"message":"Device online","data":{}}}

sknSensors/HomeOffice/Occupancy/system/set 4
sknSensors/HomeOffice/Occupancy/system {"ReadResponse":{"success":true,"message":"Presence Detected","data":{"target":"Stationary","stationaryTargetDistance":"9.3","stationaryTargetEnergy":9}}}

sknSensors/HomeOffice/Occupancy/system/set 5
sknSensors/HomeOffice/Occupancy/system {"ReadConfigResponse":{"success":true,"message":"Device online","data":{"cfgMaxGate":8,"cfgMaxMovingGate":8,"cfgMaxStationaryGate":8,"cfgSensorIdleTimeInSeconds":15000,"gateConfig":{"0":{"cfgMovingGateSensitivity":50,"cfgStationaryGateSensitivity":0},"1":{"cfgMovingGateSensitivity":50,"cfgStationaryGateSensitivity":0},"2":{"cfgMovingGateSensitivity":40,"cfgStationaryGateSensitivity":40},"3":{"cfgMovingGateSensitivity":30,"cfgStationaryGateSensitivity":40},"4":{"cfgMovingGateSensitivity":20,"cfgStationaryGateSensitivity":30},"5":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":30},"6":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":20},"7":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":20}}}}}

sknSensors/HomeOffice/Occupancy/system/set 9
sknSensors/HomeOffice/Occupancy/system {"ReadVersionResponse":{"data":{},"success":false,"message":"command execution failed"}}

sknSensors/HomeOffice/Occupancy/system/set 11
sknSensors/HomeOffice/Occupancy/system {"DeviceInfoResponse":{"data":{"isEngineeringMode":true,"cmdProtocolVersion":1,"cmdCommunicationBufferSize":64,"cmdFirmwareVersion":"v1.7.22091516\n","engRetainDataValue":772},"success":true,"message":"Device online"}}

sknSensors/HomeOffice/Occupancy/deviceStatus {"TargetReport":{"unitOfMeasure":"feet","triggeredBy":"Moving","detectionDistance":"0.0","movingTargetDistance":"2.7","movingTargetEnergy":100,"stationaryTargetDistance":"0.0","stationaryTargetEnergy":0}}
sknSensors/HomeOffice/Occupancy/motion ON
sknSensors/HomeOffice/Occupancy/occupancy OFF



```