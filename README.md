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
