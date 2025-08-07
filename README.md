Code for connecting an ESP32C6 via Zigbee to Home Assistant.


This code is to connect your ESP32C6 with Zigbee support and a DHT22 temperature and humidity sensor
to Home Assistant. 

Before compiling and uploading, there are a few things that need to be configured:
- The GPIO pin where the sensor is connected to needs to be defined in the code below under 'DHTTYPE'.
- The correct board needs to be selected with the right COM port, this information can be found online.
- In 'Tools', the 'Partition Scheme' needs to be set to : "Zigbee 4MB with spiffs".
- In 'Tools', the 'Zigbee Mode' needs to be set to : "Zigbee ED (end device)".

When the code is uploaded, restart the ESP32C6, and the sensor will become visible in the 'Add Device' tab of ZHA.

In my case, i am using a Seeed Studio XAOI ESP32C6 with an DHT22 Module. The DHT22 is connected via jumber cables to the ESP32. 
The code is made in Arduino IDE 2.3.6 with the help of the exsisting Temperature Sensor example from the ESP32 library under the header Zigbee. With the use of an AI model the code is made to fit this purpose.
