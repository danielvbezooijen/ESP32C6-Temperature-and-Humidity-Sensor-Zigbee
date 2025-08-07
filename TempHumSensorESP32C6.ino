// Code for connecting an ESP32C6 via Zigbee to Home Assistant.
//
// This code is to connect your ESP32C6 with Zigbee support and a DHT22 temperature and humidity sensor
// to Home Assistant. 
//
// Before compiling and uploading, there are a few things that need to be configured:
// - The GPIO pin where the sensor is connected to needs to be defined in the code below under 'DHTTYPE'.
// - The correct board needs to be selected with the right COM port, this information can be found online.
// - In 'Tools', the 'Partition Scheme' needs to be set to : "Zigbee 4MB with spiffs".
// - In 'Tools', the 'Zigbee Mode' needs to be set to : "Zigbee ED (end device)".
//
// When the code is uploaded, restart the ESP32C6, and the sensor will become visible in the 'Add Device' tab of ZHA.

#include "Zigbee.h"
#include <DHT.h>  // Include DHT library

#define DHTPIN 18      // Define the GPIO pin you're using for DHT22 on your ESP32
#define DHTTYPE DHT22  // Define the sensor type
#define TEMP_SENSOR_ENDPOINT_NUMBER 10
                                                    uint8_t button = BOOT_PIN;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Optional Time cluster variables
struct tm timeinfo;
struct tm *localTime;
int32_t timezone;

// Create Zigbee temperature sensor endpoint
ZigbeeTempSensor zbTempSensor = ZigbeeTempSensor(TEMP_SENSOR_ENDPOINT_NUMBER);

/************************ Temp sensor *****************************/
static void temp_sensor_value_update(void *arg) {
  for (;;) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.printf("Temperature: %.2f°C, Humidity: %.2f%%\r\n", temperature, humidity);

      // Update temperature and humidity values
      zbTempSensor.setTemperature(temperature);
      zbTempSensor.setHumidity(humidity);
    }

    delay(30000);  // Updates the sensor every 30 seconds (time is in milliseconds)
  }
}

/********************* Arduino functions **************************/
void setup() {
  Serial.begin(115200);

  dht.begin();  // Start the DHT sensor

  pinMode(button, INPUT_PULLUP);

  zbTempSensor.setManufacturerAndModel("Espressif", "ZigbeeTempHumiditySensor");
  zbTempSensor.setMinMaxValue(10, 50);
  zbTempSensor.setTolerance(1);
  zbTempSensor.addTimeCluster();

  // Add humidity sensor with min, max, tolerance
  zbTempSensor.addHumiditySensor(20.0, 90.0, 5.0);  // humidity from 20% to 90%, tolerance of 5%

  Zigbee.addEndpoint(&zbTempSensor);

  Serial.println("Starting Zigbee...");
  if (!Zigbee.begin()) {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  } else {
    Serial.println("Zigbee started successfully!");
  }

  Serial.println("Connecting to network");
  while (!Zigbee.connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  timeinfo = zbTempSensor.getTime();
  timezone = zbTempSensor.getTimezone();

  Serial.println("UTC time:");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  time_t local = mktime(&timeinfo) + timezone;
  localTime = localtime(&local);

  Serial.println("Local time with timezone:");
  Serial.println(localTime, "%A, %B %d %Y %H:%M:%S");

  xTaskCreate(temp_sensor_value_update, "temp_sensor_update", 2048, NULL, 10, NULL);

  zbTempSensor.setReporting(1, 60, 0.1);        // Temperature reporting
  zbTempSensor.setHumidityReporting(1, 60, 1);  // Humidity reporting
}

void loop() {
  if (digitalRead(button) == LOW) {
    delay(100);
    int startTime = millis();
    while (digitalRead(button) == LOW) {
      delay(50);
      if ((millis() - startTime) > 3000) {
        Serial.println("Resetting Zigbee to factory and rebooting in 1s.");
        delay(1000);
        Zigbee.factoryReset();
      }
    }
    zbTempSensor.reportTemperature();
    zbTempSensor.reportHumidity();
  }
  delay(100);
}
