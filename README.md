# EnvironmentMonitor_DHT
<a href="https://homieiot.github.io/">
  <img src="https://homieiot.github.io/img/works-with-homie.png" alt="works with MQTT Homie">
</a>

HomieNode V3 featuring:
- DHT Temperature Class, using HomeNode with auto detect sensor of DHT11, DHT22, AM2302, RHT03.
- LD2410 mmWave Radar Motion Class, using HomieNode.

Pin Out for ESP32
- #define PIN_DHT  4
- #define LD_IO   18
- #define LD_RX   16
- #define LD_TX   17


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

sknSensors/HomeOffice/Occupancy/system/set 5
sknSensors/HomeOffice/Occupancy/system {"ReadConfigResponse":{"success":true,"message":"Device online","data":{"cfgMaxGate":8,"cfgMaxMovingGate":8,"cfgMaxStationaryGate":8,"cfgSensorIdleTimeInSeconds":15000,"gateConfig":{"0":{"cfgMovingGateSensitivity":50,"cfgStationaryGateSensitivity":0},"1":{"cfgMovingGateSensitivity":50,"cfgStationaryGateSensitivity":0},"2":{"cfgMovingGateSensitivity":40,"cfgStationaryGateSensitivity":40},"3":{"cfgMovingGateSensitivity":30,"cfgStationaryGateSensitivity":40},"4":{"cfgMovingGateSensitivity":20,"cfgStationaryGateSensitivity":30},"5":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":30},"6":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":20},"7":{"cfgMovingGateSensitivity":15,"cfgStationaryGateSensitivity":20}}}}}

sknSensors/HomeOffice/Occupancy/system/set 4
sknSensors/HomeOffice/Occupancy/system {"ReadResponse":{"success":true,"message":"Presence not detected","data":{}}}

sknSensors/HomeOffice/Occupancy/occupancy {"OccupancyTarget":{"unitOfMeasure":"feet","triggeredBy":"Not Present","detectionDistance":"0.0","movingTargetDistance":"0.0","movingTargetEnergy":0,"stationaryTargetDistance":"0.0","stationaryTargetEnergy":8}}
sknSensors/HomeOffice/Occupancy/motion OFF

```