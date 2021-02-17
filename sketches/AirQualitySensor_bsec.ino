bool DEBUG_ENABLED = false;

#include "graphs.h"
#include "wifi.h"
#include "sensor.h"
#include "httpRequest.h"

// global references
static uint16_t loopCounter = 1; // Display iterations

void setup(void)
{
  Serial.begin(115200); // Start serial port at Baud rate

  connectToWiFi(); //connect to wi-fi
  setupSensor();   // Initialize sensor
  setupLCD();      // LCD Init and main table draw
}

void loop(void)
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run())
  { // If new data is available
    if (DEBUG_ENABLED)
    {
      logReadings(time_trigger, iaqSensor.pressure, iaqSensor.iaq, iaqSensor.iaqAccuracy, iaqSensor.temperature, iaqSensor.humidity, iaqSensor.co2Equivalent);
    }
    temperature_to_lcd(iaqSensor.temperature, 4);
    humidity_to_lcd(iaqSensor.humidity, 55);
    gas_to_lcd(iaqSensor.iaq, 105);

    if (loopCounter >= HTTP_TRIGGER)
    { // only send requests every X readings.
      sendHttpRequest(iaqSensor.pressure, iaqSensor.iaq, iaqSensor.iaqAccuracy, iaqSensor.temperature, iaqSensor.humidity, iaqSensor.co2Equivalent);
      loopCounter = 0;
    }
    loopCounter++;
  }
  else
  {
    checkIaqSensorStatus();
  }
}
