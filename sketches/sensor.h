#include "bsec.h" // Include the BSEC-BME680 Sensor library

Bsec iaqSensor; // Create an object of the class Bsec

// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);
String output;

void setupSensor() {
  Wire.begin();

  // Initializing BME680 sensor
  iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);  
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };
  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
  
  // Print the header
  if(DEBUG_ENABLED) {
    output = "Timestamp [ms], pressure [hPa], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], CO2 equivalent";
    Serial.println(output);
  }
}
// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}


void logReadings(int32_t time_trigger, int32_t pressure, int32_t iaq, int32_t iaqAccuracy, float temperature, float humidity, int32_t co2Equivalent){
    output = String(time_trigger);
    output += ", " + String(pressure);
    output += ", " + String(iaq);
    output += ", " + String(iaqAccuracy);
    output += ", " + String(temperature);
    output += ", " + String(humidity);
    output += ", " + String(co2Equivalent);
    Serial.println(output);
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
