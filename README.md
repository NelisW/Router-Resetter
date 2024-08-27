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

## Enclosure 

See https://www.printables.com/model/987325/

## Background

My fibre ONT and/or router sometimes drop and fail to reconnect automatically.  My solution is to power cycle the ONT and router. Doing this manually is not ideal, being some distance from my study. So it made sense to use an ESP8266 to monitor internet connection, to then power cycle if connection is lost.

This solution uses MQTT to integrate with HA, so that you can check the status of the device and even activate the power cycle from HA.  

The device responds to MQTT from any pub/sub client, it does not have to be HA. So you can control the resetter from any MQTT friendly application (or the command line).

I used the following sources in putting together this solution:

1. [make-an-automatic-router-re-starter](http://www.whatimade.today/make-an-automatic-router-re-starter-for-3-with-an-esp8266-01-and-single-relay/)  
1. [NodeMCU-Internet-Hardware-WatchDog](https://www.instructables.com/NodeMCU-Internet-Hardware-WatchDog/)
1. [internet-hardware-watchdog-using-nodemcu](https://maker.pro/arduino/projects/internet-hardware-watchdog-using-nodemcu)  
1. [modem_restarter](https://diyodemag.com/projects/modem_restarter)
1. [esp8266-auto-router-resetter](https://www.programmingelectronics.com/esp8266-auto-router-resetter-student-project/)
1. [reconnect-esp8266-nodemcu-to-wifi](https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/)
1. [ESP8266Ping](https://github.com/dancol90/ESP8266Ping/tree/master)
1. [help-with-simple-ip-ping-program](https://forum.arduino.cc/t/solved-help-with-simple-ip-ping-program/498296/2)

## How to set and get wifi secrets:
[providing-credentials-without-the-keys-being-displayed-in-the-code](https://community.platformio.org/t/providing-credentials-without-the-keys-being-displayed-in-the-code/32437/2)

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


## Serialise with JSON

https://arduinojson.org/  
https://arduinojson.org/v7/example/  
https://arduinojson.org/v7/tutorial/serialization/  
https://arduinojson.org/v5/example/generator/  


## Set up static IP address

https://randomnerdtutorials.com/esp8266-nodemcu-static-fixed-ip-address-arduino/
but this requires reserving the IP address in the router.

## knolleary mqtt pubsub library

https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino


## Home Assistant Integration

Manual integration of MQTT device into HA  
https://resinchemtech.blogspot.com/2023/12/adding-mqtt-devices.html

MQTT Auto discovery in HA general reading    
https://gist.github.com/Resinchem/ecd86dfb52bd699c79acfa80cd348d7b  
https://youtu.be/VHiCtZqllU8  
https://resinchemtech.blogspot.com/2023/12/mqtt-auto-discovery.html  
https://roelofjanelsinga.com/articles/mqtt-discovery-with-an-arduino/  
https://mpolinowski.github.io/docs/Automation_and_Robotics/Home_Automation/  2022-07-10-home-assistant-mqtt-autodiscovery-part-i/2022-07-10  
https://www.home-assistant.io/integrations/sensor.mqtt/  

This code uses button to activate in HA, with sensor providing state and info  
https://www.home-assistant.io/integrations/button/  
https://www.home-assistant.io/integrations/button.mqtt/  
https://www.home-assistant.io/integrations/sensor  
https://www.home-assistant.io/integrations/sensor.mqtt/  

## Using MQTT on the command line

create the MQTT auto discovery of sensor (make retain to remember)

      mosquitto_pub -h 192.168.68.71 -p 1883 -u "user"  -P "passwd" \
         -t "homeassistant/sensor/routerreset/config"   \
         -m "{\"name\": \"garden\", \"unique_id\": \"garden\",\"state_topic\": \"routerreset/state\"}" -r

delete the retained message by sending empty payload (also retain):  

      mosquitto_pub -h 192.168.68.71 -p 1883 -u "user"  -P "passwd" \
         -t "homeassistant/sensor/routerreset/config"   \
         -m "" -r

Hint to remove retained message in topic (type this in terminal):  
https://community.home-assistant.io/t/mqtt-how-to-remove-retained-messages/79029/17  

      mosquitto_pub -h serverIP -p 1883 -u MQTTusername -P MQTTpassword -t topic -r -n

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

https://github.com/PowerBroker2/SafeString?utm_source=platformio&utm_medium=piohome  
https://www.forward.com.au/pfod/ArduinoProgramming/Serial_IO/index.html  
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/index.html#static  
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/index.html#Printing  
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/SafeString_stoken_Example.ino  

## Adding Periodic Automatic Reboots

Handling Out-Of-Memory on ESP32 and ESP8266, by adding Periodic Automatic Reboots

https://www.forward.com.au/pfod/ArduinoProgramming/ArduinoStrings/index.html#reboot

`millisDelay` implements a non-blocking, repeatable delay, see the detailed description. 
    
To use millisDelay, create a global instance for each delay you need e.g.

            millisDelay ledDelay;

Then start the delay in Setup(), running with with say a 1sec (1000ms) delay i.e.

            ledDelay.start(1000);\

Then in loop() check if the delay has timed out with

      if (ledDelay.justFinished()) {
         . . .  do stuff here when delay has timed out
         }

## OTA

The code does not support OTA updates.


## Hardware

### Relay Keyes_SRly 

http://www.techydiy.org/keyes-sr1y-relay-module/  
https://arduinomodules.info/ky-019-5v-relay-module/  
https://www.robotics.org.za/FE-SR1Y  
Counting from the LED:  

      pin 1 s  connects to pin D1/GPIO5 as input to signal.  
      pin 2 +  Positive supply, connects to 3V seems to work on 3V only, not 5V  
      pin 3 -  Negative supply, connects to G  

LED connected to  Pin D7 GPIO13

### Interrupt
toggle switch interrupt  
https://www.electronicwings.com/nodemcu/nodemcu-gpio-interrupts-with-arduino-ide  
https://randomnerdtutorials.com/interrupts-timers-esp8266-arduino-ide-nodemcu/  

### Pins

      //Input pin
      #define Switch_Pin 4 // D2 GPIO04, held low, switch (to +Vcc) to activate

      //Output pins
      #define RELAY_Pin 5  // D1 GPIO05, 'S' on the relay 

      #define LED_WAIT_Pin 14 // D5 GPIO14, blue  LED power up & waiting
      #define LED_PING_Pin 12 // D6 GPIO12, green LED ping successful
      #define LED_DOWN_Pin 13 // D7 GPIO13, red   LED power down
