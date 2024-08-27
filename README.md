# Router-Resetter

ESP8266 code to power cycle a router if three different website pings all fails. Integrates with Home Assistant (HA) with auto-discovery via MQTT. Developed using the Arduino framework 

The ESP pings three different web sites every ten seconds. If all three sites do not return replies for four tries (40 seconds), it is assumed that the internet is not connected and the power cycle is initiated. Power is down for some time, and then the ESP waits for a reconnect period before testing pings. The ESP is reboot every day.   Times are as follows: 

      // ESP8266 times in milliseconds
      #define second 1000
      #define minute 60000
      unsigned long DELAY_FOR_COMMS = 1 * second;           // wait for whatever on network (1)
      unsigned long DELAY_WAIT_POWER_DOWN = 30 * second;     // this long no power (60)
      unsigned long DELAY_WAIT_MODEM_POWER_UP = 150* second;   // wait for fibre and wifi network stable (300)
      unsigned long PingPeriod = 10 * second;                // how long between pings (120))
      unsigned long StateReportPeriod = 1 * second;
      unsigned long MQTTReconnectPeriod = 5 * second;
      unsigned long REBOOT_DELAY_MS = 24ul * 60 * 60 * 1000; // one day in mS (86 400 000)

      unsigned long MaxNumPingFails = 4; // how many times must ping fail before reset

The state is displayed on three LEDS. LED_ON is low, so LED output pin is connected to +Vcc via a suitable resistor. The states are as follows:

1. LED_WAIT (blue)  power up & waiting for ping respond
1. LED_PING (green) ping successful
1. LED_DOWN (red) power out output relay is down

The relay normally-closed contacts are used, hence the relay is not consuming power and the system fails safe (keeping power on the output).

## Enclosure 

See https://www.printables.com/model/987325/

## Background

My fibre ONT and/or router sometimes drop and fail to reconnect automatically.  My solution is to power cycle the ONT and router. Doing this manually is not ideal, being some distance from my study. So it made sense to use an ESP8266 to monitor internet connection, to then power cycle if connection is lost.

This solution uses MQTT to integrate with HA, so that you can check the status of the device and even activate the power cycle from HA.  

The device responds to MQTT from any pub/sub client, it does not have to be HA. So you can control the resetter from any MQTT friendly application (or the command line).

## How to set and get wifi secrets:

1. Set the following two windows USER environment variables:

         PIO_WIFI_SSID=YOURSSID
         PIO_WIFI_PASSWD=YOURPASSWORD

   close and open vscode running platformio (required to read the environment)
2. in platformio.ini add this

         build_flags =
            -I include
            -D ENV_PIO_WIFI_SSID="\"${sysenv.PIO_WIFI_SSID}\""
            -D ENV_PIO_WIFI_PASSWD="\"${sysenv.PIO_WIFI_PASSWD}\""
3. use the variables as follows:

         static const char* ssid     = ENV_PIO_WIFI_SSID;     // Your ssid
         static const char* password = ENV_PIO_WIFI_PASSWD; // Your Password

## MQTT Setup  for HA

MQTT username and password must sync with value in HA.
1. HS->Settings->Addons->MQTT->config options:'

         logins:
            - username: MQTTUserName
               password: MQTTpassword

2. Set the following two windows `USER` environment variables:

         MQTTUserName=YourMQTTUserName
         MQTTpassword=YourMQTTpassword

   close and open vscode running platformio (required to read the environment)

3. in platformio.ini add this after the SSID and password lines:'

        -D ENV_PIO_WIFI_SSID="\"${sysenv.PIO_WIFI_SSID}\""
        -D ENV_PIO_WIFI_PASSWD="\"${sysenv.PIO_WIFI_PASSWD}\""

4. use the variables as follows:

      static const char* MQTTUserName     = ENV_MQTTUserName;     // Your ssid
      static const char* MQTTpassword = ENV_MQTTpassword; // Your Password

The Home Assistant MQTT comms is serialised in JSON.

The knolleary mqtt pubsub library is used.

This code uses button in Home Assistant to activate a power cycle, with sensor providing state and info  

## Set up static IP address

When you compile the code you can either select static IP address or DHCP.  A static IP address requires reserving the IP address in the router.

## MQTT topics

      const char *mqttdiscoverytopicreset = "homeassistant/button/routerreset/reset/config";
      const char *mqttdiscoverytopicstate = "homeassistant/sensor/routerreset/state/config";
      const char *mqttdiscoverytopicinfo = "homeassistant/sensor/routerreset/info/config";
      const char *mqtttopicvars = "routerreset/vars";
      const char *mqtttopicinfo = "routerreset/info";
      const char *mqtttopiccmd = "routerreset/cmd";
      const char *mqtttopicstate = "routerreset/state";


## Undesired `ping` debug message
ping sends and irritating message, from the code in this file:  

      C:\Users\%USER%\.platformio\packages\framework-arduinoespressif8266\tools\sdk\lwip2\builder\glue-lwip\esp-ping.c
but commenting it out does not seem to make a difference - probably because it is not compiled again.

https://forum.arduino.cc/t/cant-turn-off-esp8266-dancole90-ping-library-debug-messages/1263126/3  

## Safestring used instead of String

SafeString does not cause memory leaks.

## Adding Periodic Automatic Reboots

Handling Out-Of-Memory on ESP32 and ESP8266, by adding Periodic Automatic Reboots

## OTA

The code does not support OTA updates.

## Hardware

### Pins

      //Input pin, hardware activation of the power cycle:
      #define Switch_Pin 4 // D2 GPIO04, held low, switch (to +Vcc) to activate

      //Output pins
      #define RELAY_Pin 5  // D1 GPIO05, 'S' on the relay 

      #define LED_WAIT_Pin 14 // D5 GPIO14, blue  LED power up & waiting
      #define LED_PING_Pin 12 // D6 GPIO12, green LED ping successful
      #define LED_DOWN_Pin 13 // D7 GPIO13, red   LED power down
