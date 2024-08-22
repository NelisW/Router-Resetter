
/*
This code is vaguely based on, but extends way beyond:
http://www.whatimade.today/make-an-automatic-router-re-starter-for-3-with-an-esp8266-01-and-single-relay/
https://www.instructables.com/NodeMCU-Internet-Hardware-WatchDog/
https://maker.pro/arduino/projects/internet-hardware-watchdog-using-nodemcu
https://diyodemag.com/projects/modem_restarter
https://www.programmingelectronics.com/esp8266-auto-router-resetter-student-project/

https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/
https://github.com/dancol90/ESP8266Ping/tree/master
https://forum.arduino.cc/t/solved-help-with-simple-ip-ping-program/498296/2
---------------------------------------------------------------------------------------
How to set and get wifi secrets:
https://community.platformio.org/t/providing-credentials-without-the-keys-being-displayed-in-the-code/32437/2
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

-----------------------------------------------------------------------------------
MQTT username and password must sync with value in HA.
1. HS->Settings->Addons->MQTT->config options:
  logins:
    - username: MQTTUserName
      password: MQTTpassword

2. Set the following two windows USER environment variables:
   MQTTUserName=YourMQTTUserName
   MQTTpassword=YourMQTTpassword
   close and open vscode running platformio (required to read the environment)

3. in platformio.ini add this after the SSID and password lines:
        -D ENV_PIO_WIFI_SSID="\"${sysenv.PIO_WIFI_SSID}\""
        -D ENV_PIO_WIFI_PASSWD="\"${sysenv.PIO_WIFI_PASSWD}\""

4. use the variables as follows:
   static const char* MQTTUserName     = ENV_MQTTUserName;     // Your ssid
   static const char* MQTTpassword = ENV_MQTTpassword; // Your Password


-------------------------------------------------------------------------------------
JSON
https://arduinojson.org/
https://arduinojson.org/v7/example/
https://arduinojson.org/v7/tutorial/serialization/
https://arduinojson.org/v5/example/generator/

------------------------------------------------------------------------------------
Set up static IP address
https://randomnerdtutorials.com/esp8266-nodemcu-static-fixed-ip-address-arduino/
but this requires reserving the IP address in the router

-------------------------------------------------------------------------------------
using the knolleary mqtt pubsub library
https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino

-------------------------------------------------------------------------------------
Manual integration of MQTT device into HA
https://resinchemtech.blogspot.com/2023/12/adding-mqtt-devices.html

MQTT Auto discovery in HA general reading
https://gist.github.com/Resinchem/ecd86dfb52bd699c79acfa80cd348d7b
https://youtu.be/VHiCtZqllU8
https://resinchemtech.blogspot.com/2023/12/mqtt-auto-discovery.html
https://roelofjanelsinga.com/articles/mqtt-discovery-with-an-arduino/
https://mpolinowski.github.io/docs/Automation_and_Robotics/Home_Automation/2022-07-10-home-assistant-mqtt-autodiscovery-part-i/2022-07-10
https://www.home-assistant.io/integrations/sensor.mqtt/

This code uses button to activate in HA, with sensor providing state and info
https://www.home-assistant.io/integrations/button/
https://www.home-assistant.io/integrations/button.mqtt/
https://www.home-assistant.io/integrations/sensor
https://www.home-assistant.io/integrations/sensor.mqtt/

----------------------------------------------------------------------------------------
Using MQTT on the command line

create the MQTT auto discovery of sensor (make retain to remember)
mosquitto_pub -h 192.168.68.71 -p 1883 -u "user"  -P "passwd" \
    -t "homeassistant/sensor/routerreset/config"   \
    -m "{\"name\": \"garden\", \"unique_id\": \"garden\",\"state_topic\": \"routerreset/state\"}" -r

delete the retained message by sending empty payload (also retain):
mosquitto_pub -h 192.168.68.71 -p 1883 -u "user"  -P "passwd" \
    -t "homeassistant/sensor/routerreset/config"   \
    -m "" -r

Hint to remove retained message in topic (type this in HA terminal):
https://community.home-assistant.io/t/mqtt-how-to-remove-retained-messages/79029/17
mosquitto_pub -h serverIP -p 1883 -u MQTTusername -P MQTTpassword -t topic -r -n

-------------------------------------------------------------------------------------
ping sends and irritating message, from the code in this file:
C:\Users\%USER%\.platformio\packages\framework-arduinoespressif8266\tools\sdk\lwip2\builder\glue-lwip\esp-ping.c
but commenting it out does not seem to make a difference - probably because it is not compiled again.
https://forum.arduino.cc/t/cant-turn-off-esp8266-dancole90-ping-library-debug-messages/1263126/3

-------------------------------------------------------------------------------------
Relay Keyes_SRly
http://www.techydiy.org/keyes-sr1y-relay-module/
https://arduinomodules.info/ky-019-5v-relay-module/
https://www.robotics.org.za/FE-SR1Y
Counting from the LED:
pin 1 s  connects to pin D1/GPIO5 as input to signal.
pin 2 +  Positive supply, connects to 3V seems to work on 3V only, not 5V
pin 3 -  Negative supply, connects to G

LED connected to  Pin D7 GPIO13
-------------------------------------------------------------------------------------
toogle switch interrupt
https://www.electronicwings.com/nodemcu/nodemcu-gpio-interrupts-with-arduino-ide
https://randomnerdtutorials.com/interrupts-timers-esp8266-arduino-ide-nodemcu/

-------------------------------------------------------------------------------------
Safestring used instead of String

https://github.com/PowerBroker2/SafeString?utm_source=platformio&utm_medium=piohome
https://www.forward.com.au/pfod/ArduinoProgramming/Serial_IO/index.html
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/index.html#static
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/index.html#Printing
https://www.forward.com.au/pfod/ArduinoProgramming/SafeString/SafeString_stoken_Example.ino


-------------------------------------------------------------------------------------
Handling Out-Of-Memory on ESP32 and ESP8266 - Adding Periodic Automatic Reboots

https://www.forward.com.au/pfod/ArduinoProgramming/ArduinoStrings/index.html#reboot

millisDelay** implements a non-blocking, repeatable delay, see the detailed description. 
    
  To use **millisDelay**, create a global instance for each delay you need e.g.
      millisDelay ledDelay;
  Then start the delay running with with say a 1sec (1000ms) delay i.e.
      ledDelay.start(1000);\
  This is often done in setup()
  Then in loop() check if the delay has timed out with
  if (ledDelay.justFinished()) {
   . . .  do stuff here when delay has timed out
   }
   

-----------------------------------------

Recompilation required if any of the variables or settings are changed.

*/

#define staticip true // requires router setting
// #undef staticip

// #define  DiscoverMQTTInfo True  // is info message discoverable in HA
#undef DiscoverMQTTInfo

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266Ping.h>
#include <ArduinoJson.h>

#include "BufferedOutput.h"
#include "SafeString.h"
#include <millisDelay.h>

// local functions
void publishInfoMQTT();
void publishStateMQTT();

//----------------------------------------------
#ifndef LED_BUILTIN
#define LED_BUILTIN D4 // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED
#endif

#define Switch_Pin 4 // D2 GPIO04, held low, switch (to +Vcc) to activate
#define RELAY_Pin 5  // D1 GPIO05, 'S' on the relay 

#define LED_WAIT_Pin 14 // D5 GPIO14, blue  LED power up & waiting
#define LED_PING_Pin 12 // D6 GPIO12, green LED ping successful
#define LED_DOWN_Pin 13 // D7 GPIO13, red   LED power down

#define LED_OFF HIGH
#define LED_ON LOW

//----------------------------------------
static const char *ssid = ENV_PIO_WIFI_SSID;       // Your ssid
static const char *password = ENV_PIO_WIFI_PASSWD; // Your Password
static const char *MQTTUserName = ENV_MQTTUserName;
static const char *MQTTpassword = ENV_MQTTpassword;

// these three hosts are pinged at regular intervals
const char *testHostname01 = "www.google.com";
const char *testHostname02 = "www.microsoft.com";
const char *testHostname03 = "x.com";


// homeassistant IP 
IPAddress HAserverIP(192, 168, 68, 71);

#ifdef staticip
// only used if setting up a static IP
IPAddress localIP(192, 168, 68, 56);
IPAddress subnet(255, 255, 255, 0);
IPAddress DNSIP(8, 8, 8, 8);
IPAddress GatewayIP(10, 0, 0, 254);
#endif

const unsigned long BAUD_RATE = 115200;

// safestring variables
createBufferedOutput(output, 256, DROP_UNTIL_EMPTY); // modes are DROP_UNTIL_EMPTY, DROP_IF_FULL or BLOCK_IF_FULL
createSafeString(strMsg, 1024);                      // can have any value anywhere, write to this string before use.
createSafeString(strPayload, 1024);                  // reserved for var setting payload tokenising
createSafeString(tokenfield, 10);                    // for the tokenfield strings. Should have capacity > largest field length

//non-blocking timer to force ESP8266 reboot at timed interval.
millisDelay rebootDelay;


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

unsigned long lastStateReportAttempt = 0;
unsigned long lastMQTTReconnectAttempt = 0;
unsigned long lastPingAttempt = 0;

const char *mqttdiscoverytopicreset = "homeassistant/button/routerreset/reset/config";
const char *mqttdiscoverytopicstate = "homeassistant/sensor/routerreset/state/config";
const char *mqttdiscoverytopicinfo = "homeassistant/sensor/routerreset/info/config";
const char *mqtttopicvars = "routerreset/vars";
const char *mqtttopicinfo = "routerreset/info";
const char *mqtttopiccmd = "routerreset/cmd";
const char *mqtttopicstate = "routerreset/state";

int NumPowerCycleEvents = 0;
unsigned int NumPingFails = 0;
int NumPingAttempts = 1;
bool ISRpowerCycle = false;
bool pingnow = true;

enum
{
  TESTING_STATE = 0,
  FAILURE_STATE = 1,
  SUCCESS_STATE = 2,
  INIT_STATE = 3,
};

int CurrentState = INIT_STATE;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

////////////////////////////////////////////////////////////
//
void setVarsMQTT(char *payload, unsigned int length)
{
  //!!! no error warnings, check the values afterward!!!

  // 0 S
  // 1 DELAY_FOR_COMMS
  // 2 DELAY_WAIT_POWER_DOWN
  // 3 DELAY_WAIT_MODEM_POWER_UP
  // 4 PingPeriod
  // 5 StateReportPeriod
  // 6 MQTTReconnectPeriod
  // 7 MaxNumPingFails

  // format for the vars setting command. comma separated, no spaces, comma at end. All times in millisec
  // Topic:  "routerreset/cmd"
  // Payload:
  // S,DELAY_FOR_COMMS,DELAY_WAIT_POWER_DOWN,DELAY_WAIT_MODEM_POWER_UP,PingPeriod,StateReportPeriod,MQTTReconnectPeriod,MaxNumPingFails,

  int vint;
  strPayload.clear();
  tokenfield.clear();
  createSafeStringFromCharPtrWithSize(strPayload, payload, length);

  //'S' zero token
  strPayload.nextToken(tokenfield, ',', true, true, true);
  // first token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      DELAY_FOR_COMMS = vint;
    }
  }
  // second token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      DELAY_WAIT_POWER_DOWN = vint;
    }
  }
  // third token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      DELAY_WAIT_MODEM_POWER_UP = vint;
    }
  }
  // fourth token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      PingPeriod = vint;
    }
  }
  // fifth token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      StateReportPeriod = vint;
    }
  }
  // sixth token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      MQTTReconnectPeriod = vint;
    }
  }
  // seventh token
  if (strPayload.nextToken(tokenfield, ',', true))
  {
    if (tokenfield.toInt(vint))
    {
      MaxNumPingFails = vint;
    }
  }


}

////////////////////////////////////////////////////////////
//
void reportVarsMQTT()
// report the key variables controlling the timing and counts
// this is not part of the discoverable package, do this by raw MQTT
{
  strMsg.clear();
  strMsg = "T,";
  strMsg += DELAY_FOR_COMMS;
  strMsg += ",";
  strMsg += DELAY_WAIT_POWER_DOWN;
  strMsg += ",";
  strMsg += DELAY_WAIT_MODEM_POWER_UP;
  strMsg += ",";
  strMsg += PingPeriod;
  strMsg += ",";
  strMsg += StateReportPeriod;
  strMsg += ",";
  strMsg += MQTTReconnectPeriod;
  strMsg += ",";
  strMsg += MaxNumPingFails;
  strMsg += ",";

  mqttClient.publish(mqtttopicvars, strMsg.c_str());
  output.println();
  strMsg.clear();
}

////////////////////////////////////////////////////////////
//
void reset_router()
{
  // keep track of number of router resets

  NumPowerCycleEvents++;
  strMsg.clear();
  strMsg = "Initiating router power cycle ";
  strMsg += NumPowerCycleEvents;
  strMsg += "\n";
  publishInfoMQTT();
  output.println();
  strMsg.clear();

  // we can use blocking delays here, because no network anyway
  digitalWrite(LED_PING_Pin, LED_OFF);
  mqttClient.publish(mqtttopicstate, "WAIT_BEFORE_RESET");
  delay(DELAY_FOR_COMMS); // allow notification time
  digitalWrite(RELAY_Pin, HIGH);

  mqttClient.publish(mqtttopicstate, "WAIT_POWER_DOWN");
  digitalWrite(LED_DOWN_Pin, LED_ON);
  delay(DELAY_FOR_COMMS); // allow notification time

  delay(DELAY_WAIT_POWER_DOWN);
  digitalWrite(RELAY_Pin, LOW);

  digitalWrite(LED_DOWN_Pin, LED_OFF);

  mqttClient.publish(mqtttopicstate, "WAIT_MODEM_POWER_UP");
  digitalWrite(LED_WAIT_Pin, LED_ON);
  delay(DELAY_FOR_COMMS); // allow notification time

  delay(DELAY_WAIT_MODEM_POWER_UP);

  digitalWrite(LED_WAIT_Pin, LED_OFF);

  delay(DELAY_FOR_COMMS); // allow notification time

  mqttClient.publish(mqtttopicstate, "MODEM_POWER_UP");
  digitalWrite(LED_PING_Pin, LED_OFF);

  strMsg.clear();
  strMsg = "Completed router power cycle ";
  output.println(strMsg);
  strMsg.clear();

  pingnow = true;
  ISRpowerCycle = false;
}

////////////////////////////////////////////////////////////
//
// IRAM_ATTR to keep the ISR function in memory required for ESP core 2.5.1 and later.
IRAM_ATTR void switchCallback()
{
  output.print("ISR Interrupt forces modem power cycle");
  strMsg.clear();
  strMsg = "ISR Interrupt forces modem power cycle\n";
  publishInfoMQTT();
  strMsg.clear();

  ISRpowerCycle = true;
}

////////////////////////////////////////////////////////////
//
void setup_wifi()
{
  // Connecting to a WiFi network
  WiFi.hostname("Routerreset");
#ifdef staticip
  output.println("Attempting to configure static IP");
  WiFi.config(localIP, DNSIP, GatewayIP, subnet);
#else
  output.println("STA IP given by DHCP");
#endif

  strMsg.clear();
  strMsg = "Connecting to ";
  strMsg += ssid;
  strMsg += "\n";
  output.print(strMsg);
  strMsg.clear();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    output.print(".");
    delay(250);
  }

  // https://randomnerdtutorials.com/solved-reconnect-esp8266-nodemcu-to-wifi/
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  output.println("\nWiFi connected");
  output.print("MAC address: ");
  output.println(WiFi.macAddress());
  output.print("IP address: ");
  output.println(WiFi.localIP());
  output.print("IHostname: ");
  output.println(WiFi.hostname());
}

////////////////////////////////////////////////////////////
//

void sendMQTTDiscoveryMsg()
{

  output.println("Publishing discovery");
  // don't try to be fancy, adding more to this breaks it!
  // these entries seem to get the job done

  JsonDocument doc;
  char buffer1[1024];

  doc.clear();
  doc["unique_id"] = "RouterResetter-4c4908fc-ff48reset";
  doc["name"] = "Reset the router";
  doc["command_topic"] = mqtttopiccmd; //[required] value OFF: do nothing, value ON: reset
  JsonObject device = doc["device"].to<JsonObject>();
  device["identifiers"] = "mqttRouterResetter";
  device["name"] = "Router Resetter";
  // serializeJsonPretty(doc, Serial);
  // serializeJson(doc, Serial);
  serializeJson(doc, buffer1);
  mqttClient.publish(mqttdiscoverytopicreset, buffer1, true);

  char buffer2[1024];
  doc.clear();
  doc["unique_id"] = "RouterResetter-4c4908fc-ff48state";
  doc["name"] = "Status";
  doc["state_topic"] = mqtttopicstate; //[required]
  JsonObject device2 = doc["device"].to<JsonObject>();
  device2["identifiers"] = "mqttRouterResetter";
  device2["name"] = "Router Resetter";
  // serializeJsonPretty(doc, Serial);
  // serializeJson(doc, Serial);
  serializeJson(doc, buffer2);
  mqttClient.publish(mqttdiscoverytopicstate, buffer2, true);

#ifdef DiscoverMQTTInfo
  char buffer3[1024];
  doc.clear();
  doc["unique_id"] = "RouterResetter-4c4908fc-ff48info";
  doc["state_topic"] = mqtttopicinfo; //[required]
  JsonObject device3 = doc["device"].to<JsonObject>();
  device3["identifiers"] = "mqttRouterResetter";
  device3["name"] = "Router Resetter";
  // serializeJsonPretty(doc, Serial);
  // serializeJson(doc, Serial);
  serializeJson(doc, buffer3);
  mqttClient.publish(mqttdiscoverytopicinfo, buffer3, true);
#endif
}

////////////////////////////////////////////////////////////
//
void clearMQTTDiscoveryMsg()
{
  output.println("Removing discovery");

  mqttClient.publish(mqttdiscoverytopicreset, "", true);
  mqttClient.publish(mqttdiscoverytopicstate, "", true);
  mqttClient.publish(mqttdiscoverytopicinfo, "", true);
}

////////////////////////////////////////////////////////////
//
void mqttcallback(char *topic, byte *payload, unsigned int length)
{
  output.print("Message arrived [");
  output.print(topic);
  output.print("] ");
  payload[length] = 0;

  if (strcmp(topic, mqtttopiccmd) == 0)
  {

    output.println((char *)payload);
    if ((char)payload[0] == 'P') // ON or OFF
    {
      reset_router();
    }
    else if ((char)payload[0] == 'T') // to get varables
    {
      reportVarsMQTT();
    }
    else if ((char)payload[0] == 'S') // to set varables
    {
      setVarsMQTT((char *)payload, length);
    }

    else
    {
    }
  }
}

////////////////////////////////////////////////////////////
//
void publishStateMQTT()
{
  if (CurrentState == TESTING_STATE)
  {
    mqttClient.publish(mqtttopicstate, "PING TESTING");
  }
  else
  {
    if (CurrentState == FAILURE_STATE)
    {
      mqttClient.publish(mqtttopicstate, "OFFLINE");
    }
    else
    {
      if (CurrentState == SUCCESS_STATE)
      {
        mqttClient.publish(mqtttopicstate, "ONLINE");
      }
      else
      {
        if (CurrentState == INIT_STATE)
        {
          mqttClient.publish(mqtttopicstate, "INIT");
        }
        else
        {
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////
//
void publishInfoMQTT()
{
  mqttClient.publish(mqtttopicinfo, strMsg.c_str());
}

////////////////////////////////////////////////////////////
//
boolean mqttReconnect()
{
  if (mqttClient.connect("routerreset", MQTTUserName, MQTTpassword))
  {

    publishStateMQTT();
    strMsg.clear();
    strMsg = "connecting\n";
    publishInfoMQTT();
    strMsg.clear();

    mqttClient.subscribe(mqtttopiccmd);
  }
  else
  {
    output.print("MQTT routerreset connect failed, rc=");
    output.println(mqttClient.state());
  }

  return mqttClient.connected();
}

////////////////////////////////////////////////////////////
//
void setup()
{
  pinMode(RELAY_Pin, OUTPUT);
  digitalWrite(RELAY_Pin, LOW);
  pinMode(Switch_Pin, INPUT);

  pinMode(LED_DOWN_Pin, OUTPUT);
  pinMode(LED_WAIT_Pin, OUTPUT);
  pinMode(LED_PING_Pin, OUTPUT);
  digitalWrite(LED_PING_Pin, LED_OFF);
  digitalWrite(LED_DOWN_Pin, LED_OFF);
  digitalWrite(LED_WAIT_Pin, LED_OFF);

  Serial.begin(BAUD_RATE);

  output.connect(Serial); // <<<<< connect the buffered output to Serial

  delay(10);
  output.print("\n\nStarting routerreset ");
  output.println(__FILE__);
  delay(10);

  setup_wifi();
  delay(1500);

  mqttClient.setServer(HAserverIP, 1883);
  mqttClient.setCallback(mqttcallback);

  attachInterrupt(digitalPinToInterrupt(Switch_Pin), switchCallback, RISING);

  lastMQTTReconnectAttempt = 0;

  mqttReconnect();

  clearMQTTDiscoveryMsg();
  sendMQTTDiscoveryMsg();

  //https://www.forward.com.au/pfod/ArduinoProgramming/ArduinoStrings/index.html#reboot
  rebootDelay.start(REBOOT_DELAY_MS); // start reboot timer
  ESP.wdtEnable(5000); // arg ignored :-( default appears to be 3sec

  strMsg.clear();
  strMsg = "setup complete\n";
  publishInfoMQTT();
  strMsg.clear();
  output.println("\nSetup completed");
  delay(500);
}

////////////////////////////////////////////////////////////
//
void loop()
{
  output.nextByteOut(); // <<<<<<<<<< need to call this each loop to release next byte from buffer

 //https://www.forward.com.au/pfod/ArduinoProgramming/ArduinoStrings/index.html#reboot
  if (rebootDelay.justFinished()) {
    while (1) {} // force watch dog timer reboot
  }

  if (ISRpowerCycle == true) // triggered by ISR
  {
    reset_router();
    ISRpowerCycle = false;
  }

  unsigned long now = millis(); // nonblocking time since power on

  if (mqttClient.connected())
  {
    mqttClient.loop();
  }
  else // if reconnect fails, fall through and try again later next loop cycle
  {
    if (now - lastMQTTReconnectAttempt > MQTTReconnectPeriod)
    {
      lastMQTTReconnectAttempt = now;
      if (mqttReconnect())
      {
        lastMQTTReconnectAttempt = 0;
      }
    }
  }

  if (now - lastStateReportAttempt > StateReportPeriod)
  {
    lastStateReportAttempt = now;
    publishStateMQTT();
  }

  // now check for internet connectivity
  switch (CurrentState)
  {
  case TESTING_STATE:
    // publishStateMQTT();
    // output.println("Pinging hosts");

    if (
        (!Ping.ping(testHostname01, NumPingAttempts)) & (!Ping.ping(testHostname02, NumPingAttempts)) & (!Ping.ping(testHostname03, NumPingAttempts)))
    {
      if (NumPingFails == MaxNumPingFails)
      {
        CurrentState = FAILURE_STATE;
        strMsg.clear();
        strMsg = "Ping fail MaxNumPingFails times, resetting\n";
        publishInfoMQTT();
        strMsg.clear();

        NumPingFails = 0;
      }
      else
      {
        strMsg.clear();
        strMsg = "Ping fail: ";
        strMsg += NumPowerCycleEvents;
        strMsg += " out of ";
        strMsg += MaxNumPingFails;
        strMsg += "\n";
        publishInfoMQTT();
        strMsg.clear();

        NumPingFails++;
      }
    }
    else
    {
      digitalWrite(LED_PING_Pin, LED_ON);
      CurrentState = SUCCESS_STATE;
      NumPowerCycleEvents = 0; // reset counter since all is working now
      // output.println("Ping successful");
      // strMsg = "Ping successful\n";
      // publishInfoMQTT();
      // strMsg = "";
    }
    break;

  case FAILURE_STATE:
    output.println("\nReset initiated");
    // publishStateMQTT();
    reset_router();
    CurrentState = TESTING_STATE;
    output.println("\nReset completed");
    digitalWrite(LED_PING_Pin, LED_ON);
    break;

  case SUCCESS_STATE:
    //  publishStateMQTT();
    if (pingnow || now - lastPingAttempt > PingPeriod)
    {
      lastPingAttempt = now;
      CurrentState = TESTING_STATE;
    }
    break;

  case INIT_STATE:
    //  publishStateMQTT();
    lastPingAttempt = now;
    CurrentState = TESTING_STATE;
    break;
  }
  pingnow = false;
}
