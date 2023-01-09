/**
 * Homie Node for DHT Temperature and Humidity  Sensor
 * 
 */
#include "DHTNode.hpp"

DHTNode::DHTNode(const uint8_t dhtPin, DHTesp::DHT_MODEL_t dhtModel, const char *id, const char *name, const char *nType, const int measurementInterval = MEASUREMENT_INTERVAL)
    : HomieNode(id, name, nType)
{
  _measurementInterval = (measurementInterval > MIN_INTERVAL) ? measurementInterval : MIN_INTERVAL;
  _lastMeasurement     = 0;
  _dhtPin = dhtPin;
  _model = dhtModel;

  // Start up the library
  sensor = new DHTesp();
}


String DHTNode::getModelName() {
  String res;

  switch (sensor->getModel()) {
    case DHTesp::DHT_MODEL_t::DHT11:
      res = "DHT11";
      break;
    case DHTesp::DHT_MODEL_t::DHT22:
      res = "DHT22";
      break;
    case DHTesp::DHT_MODEL_t::AM2302:
      res = "AM2302";
      break;
    case DHTesp::DHT_MODEL_t::RHT03:
      res = "RHT03";
      break;
    case DHTesp::DHT_MODEL_t::AUTO_DETECT:
      res = "AUTO_DETECT";
      break;
    default:
        break;
  }
  return res;
}

/**
    * Called by Homie when Homie.setup() is called; Once!
  */
void DHTNode::setup()
{
  sensor->setup(_dhtPin, _model);

  Homie.getLogger() << cIndent 
                    << F("Sensor Model:  ") 
                    << getModelName() 
                    << endl;
  Homie.getLogger() << cIndent 
                    << F("Sensor Status: ") 
                    << sensor->getStatusString() 
                    << endl;

  advertise(cHumidity)
      .setName(cHumidityName)
      .setDatatype("float")
      .setUnit(cHumidityUnit);

  advertise(cTemperature)
      .setName(cTemperatureName)
      .setDatatype("float")
      .setUnit(cTemperatureUnit);
  }

  /**
   * Called by Homie when homie is connected and in run mode
  */
  void DHTNode::loop() {
    if (!(millis() - _lastMeasurement >= _measurementInterval * 1000UL || _lastMeasurement == 0)) {
      return;
    }
    _lastMeasurement = millis();

    Homie.getLogger() << F("〽 Sending Temperature: ") << getId() << endl;
    
    _sensorResults = sensor->getTempAndHumidity();
    _sensorStatus  = sensor->getStatus();
    if (_sensorStatus == DHTesp::ERROR_NONE)
    {
      Homie.getLogger() << cIndent
                        << F("Temperature=")
                        << getTemperatureF()
                        << F(", Humidity=")
                        << getTHumidity()
                        << endl;
      setProperty(cTemperature).send(String( getTemperatureF() ));
      setProperty(cHumidity).send(String( getTHumidity() ));
    }
    else
    {
      Homie.getLogger() << cIndent
                        << F("✖ Error reading sensor: ")
                        << sensor->getStatusString()
                        << ", value (F) read=" << _sensorResults.temperature
                        << endl;
    }
  }

