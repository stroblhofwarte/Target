/****************************************************************************************************************************
 * Target(Astronomy) control box
 * 
 * Software of the handcontrol box connected to a OnStep WiFi board.
 * This program will send the key presses of N,S,W,E,Func and Slew via TCP/IP.
 * 
 * It use the port 9998!
 * 
 * 
 * The startingpoint of this software was the ConfigOnStartup.ino example for ESP8266/ESP32 boards. 
 * 
 * 
 * Orginal Header:
 * 
 * ConfigOnStartup.ino
 * For ESP8266 / ESP32 boards
 *
 * ESP_WiFiManager is a library for the ESP8266/ESP32 platform (https://github.com/esp8266/Arduino) to enable easy
 * configuration and reconfiguration of WiFi credentials using a Captive Portal. Inspired by:
 * http://www.esp8266.com/viewtopic.php?f=29&t=2520
 * https://github.com/chriscook8/esp-arduino-apboot
 * https://github.com/esp8266/Arduino/blob/master/libraries/DNSServer/examples/CaptivePortalAdvanced/
 *
 * Forked from Tzapu https://github.com/tzapu/WiFiManager
 * and from Ken Taylor https://github.com/kentaylor
 * 
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP_WiFiManager
 * Licensed under MIT license
 * Version: 1.0.4
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      07/10/2019 Initial coding
 *  1.0.1   K Hoang      13/12/2019 Fix bug. Add features. Add support for ESP32
 *  1.0.4   K Hoang      07/01/2020 Use ESP_WiFiManager setHostname feature
 *  1.0.5   K Hoang       15/01/2020 Add configurable DNS feature. Thanks to @Amorphous of https://community.blynk.cc
 *****************************************************************************************************************************/

#include <FS.h>  

 /**************************************************************************************
  * 
  * Changelog:
  * 
  * 24.06.2020: Initial setup of the software.
  * 12.09.2020: Branch for development of a OnStep WiFi client.
  * 
  * 
  * ************************************************************************************/

//#define DEBUG //"Schalter" zum aktivieren

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define AP_TIMEOUT 30
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define AP_TIMEOUT 60
#endif
  
//For ESP32, To use ESP32 Dev Module, QIO, Flash 4MB/80MHz, Upload 921600

//Ported to ESP32
#ifdef ESP32
  #include <esp_wifi.h>
  #include <WiFi.h>
  #include <WiFiClient.h>

  #define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

  #define LED_ON      HIGH
  #define LED_OFF     LOW  
#else
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  //needed for library
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>  

  #define ESP_getChipId()   (ESP.getChipId())

  #define LED_ON      HIGH
  #define LED_OFF     LOW
#endif

// SSID and PW for Config Portal
String ssid = "TargetHandbox_" + String(ESP_getChipId(), HEX);
const char* password = "88888888";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h> 
#define USE_AVAILABLE_PAGES     false

#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
#include <ArduinoJson.h>                  //https://github.com/bblanchon/ArduinoJson

/******************************************************************
 * 
 *  Hardwaredefinition for Target(Astronomy):
 *  
 *  
 */

#define K_N           D1
#define K_S           D5
#define K_W           D7
#define K_E           D6
#define K_F           D0
#define K_SLEW        D2
#define LEDGUIDE      D8       // OUT
#define LEDCENTER     D3       // OUT
#define LEDFIND       D4       // OUT

#define KEY_NONE      0
#define KEY_N         1
#define KEY_S         2
#define KEY_W         3
#define KEY_E         4
#define KEY_FUNC      5
#define KEY_F1IN      6
#define KEY_F1OUT     7
#define KEY_F2IN      8
#define KEY_F2OUT     9
#define KEY_SLEW      10

/******************************************************************/
typedef struct
{
    char id[24];
    unsigned char cmd;
} _KEY_DATA;
char MoveNorth[5] = ":Mn#";
char MoveSouth[5] = ":Ms#";
char MoveWest[5] = ":Mw#";
char MoveEast[5] = ":Me#";
char MoveStop[5] = ":Q#";
char Center[5] = ":RC#";
char Guide[5] = ":RG#";
char Find[5] = ":RM#";
char Max[5] = ":RS#";
char Focus1In[5] = ":F+#";
char Focus1Out[5] = ":F-#";
char Focus1Stop[5] = ":FQ#";

int SlewGuide = 1;
int SlewCenter = 2;
int SlewFind = 3;
int SlewMax = 4;

_KEY_DATA g_data;
/******************************************************************
 * Target(Astronomy)
 */
unsigned long g_previousMillis = 0;
const long g_keyScanInterval = 100; //ms
int g_lastKey;

int g_slewrate = SlewMax;

#define ONSTEP_PORT 9998
WiFiClient g_client;
IPAddress IPudp (255, 255, 255, 255); // Sending via broadcast
IPAddress g_host;

char my_ip[32]   = "10.42.0.10";
char my_gw[32]   = "10.42.0.10";
char my_mask[32] = "255.255.255.0";
char my_host[32] = "192.168.0.1";


/*******************************************************************/

//default custom static IP
char static_ip[16] = "192.168.0.42";
char static_gw[16] = "192.168.0.1";
char static_sn[16] = "255.255.255.0";
char static_host[16] = "192.168.0.1";

//flag for saving data
bool g_shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  DEBUG_PRINTLN("Should save config");
  g_shouldSaveConfig = true;
}


void heartBeatPrint(void)
{
  static int num = 1;

#ifdef DEBUG
  if (WiFi.status() == WL_CONNECTED)
    DEBUG_PRINT("H");        // H means connected to WiFi
  else
    DEBUG_PRINT("F");        // F means not connected to WiFi
#endif

  if (num == 80) 
  {
    DEBUG_PRINTLN();
    num = 1;
  }
  else if (num++ % 10 == 0) 
  {
    DEBUG_PRINT(" ");
  }
} 

void check_status()
{
  static ulong checkstatus_timeout = 0;

  //KH
  #define HEARTBEAT_INTERVAL    10000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

void ReadFilesystem()
{
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) 
  {
    DEBUG_PRINTLN("mounted file system");
    if (SPIFFS.exists("/config.json")) 
    {
      //file exists, reading and loading
      DEBUG_PRINTLN("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) 
      {
        DEBUG_PRINTLN("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) 
        {
          DEBUG_PRINTLN("\nparsed json");
          if(json["ip"]) 
          {
            DEBUG_PRINTLN("setting custom ip from config");
            strcpy(static_ip, json["ip"]);
            strcpy(static_gw, json["gateway"]);
            strcpy(static_sn, json["subnet"]);
            strcpy(static_host, json["host"]);
            DEBUG_PRINTLN(static_ip);
          } 
#ifdef DEBUG
          else {
            DEBUG_PRINTLN("no custom ip in config");
          }
#endif
        } 
#ifdef DEBUG
        else {
          DEBUG_PRINTLN("failed to load json config");
        }
#endif
      }
    }
  } 
#ifdef DEBUG
  else {
    Serial.println("failed to mount FS");
  }
#endif
  //end read
}

void setup() 
{
  /* Target(Astronomy) hardware setup */


  pinMode(K_N,      INPUT_PULLUP);
  pinMode(K_S,      INPUT_PULLUP);
  pinMode(K_W,      INPUT_PULLUP);
  pinMode(K_E,      INPUT_PULLUP);
  pinMode(K_SLEW,   INPUT_PULLUP);
  pinMode(K_F,      INPUT_PULLDOWN_16);

  pinMode(LEDGUIDE,   OUTPUT);
  pinMode(LEDCENTER,  OUTPUT);
  pinMode(LEDFIND,     OUTPUT);

  /************************************/
  
  Serial.begin(115200);
  DEBUG_PRINTLN("\nStarting");
  ReadFilesystem();

 
  unsigned long startedAt = millis();

  // Turn on all LED's in AP mode
  digitalWrite(LEDGUIDE, LED_ON); 
  digitalWrite(LEDCENTER, LED_ON); 
  digitalWrite(LEDFIND, LED_ON); 
  
  //Local intialization. Once its business is done, there is no need to keep it around
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  ESP_WiFiManager ESP_wifiManager("Target");
  
  IPAddress _ip,_gw,_sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
  g_host.fromString(static_host);
  DEBUG_PRINTLN(_ip);
  DEBUG_PRINTLN(_gw);
  DEBUG_PRINTLN(_sn);
  DEBUG_PRINTLN(g_host);
  
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();
  //Check if there is stored WiFi router/password credentials.
  //If not found, device will remain in configuration mode until switched off via webserver.
  DEBUG_PRINTLN("Opening configuration portal.");
  
  // SSID to uppercase 
  ssid.toUpperCase();  
 
  // For some unknown reason webserver can only be started once per boot up 
  // so webserver can not be used again in the sketch.
  #define WIFI_CONNECT_TIMEOUT        30000L
  #define WHILE_LOOP_DELAY            200L
  #define WHILE_LOOP_STEPS            (WIFI_CONNECT_TIMEOUT / ( 3 * WHILE_LOOP_DELAY ))
  
  startedAt = millis();
  
  while ( (WiFi.status() != WL_CONNECTED) && (millis() - startedAt < WIFI_CONNECT_TIMEOUT ) )
  {   
   
    WiFi.mode(WIFI_STA);
    WiFi.persistent (true);
    // We start by connecting to a WiFi network
    
    DEBUG_PRINT("Connecting to ");
    DEBUG_PRINTLN(Router_SSID);
    WiFi.config(_ip, _gw, _sn);
    WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());
    

    int i = 0;
    while((!WiFi.status() || WiFi.status() >= WL_DISCONNECTED) && i++ < WHILE_LOOP_STEPS)
    {
      delay(WHILE_LOOP_DELAY);
    }    
  }

  DEBUG_PRINT("After waiting ");
  DEBUG_PRINT((millis()- startedAt) / 1000);
  DEBUG_PRINTLN(" secs more in setup(), connection result is ");

  if (WiFi.status() == WL_CONNECTED)
  {
    DEBUG_PRINT("connected. Local IP: ");
    DEBUG_PRINTLN(WiFi.localIP());
    SetSlewLed();
    
    /*****************************************************
     * Startup UDP communication
     */
    ssid.toCharArray(g_data.id, sizeof(g_data.id)); // Fill the unique id of this handbox to the command structure 
    g_client.connect(g_host, ONSTEP_PORT);
  }
  //else
  //  Serial.println(ESP_wifiManager.getStatus(WiFi.status()));
}

#define KEY_NONE      0
#define KEY_N         1
#define KEY_S         2
#define KEY_W         3
#define KEY_E         4
#define KEY_FUNC      5
#define KEY_F1IN      6
#define KEY_F1OUT     7
#define KEY_F2IN      8
#define KEY_F2OUT     9
#define KEY_SLEW      10

int ScanKeyboard()
{
    if(digitalRead(K_F) == HIGH)
    {
      // Function key is pressed, this key has no meaning
      // without another key but change the meaning of the
      // other keys.
      if(digitalRead(K_N) == LOW)
        return KEY_F1IN;
      if(digitalRead(K_S) == LOW)
        return KEY_F1OUT;
      if(digitalRead(K_W) == LOW)
        return KEY_F2IN;
      if(digitalRead(K_E) == LOW)
        return KEY_F2OUT;
      
    }
    else
    {
      if(digitalRead(K_N) == LOW)
        return KEY_N;
      if(digitalRead(K_S) == LOW)
        return KEY_S;
      if(digitalRead(K_W) == LOW)
        return KEY_W;
      if(digitalRead(K_E) == LOW)
        return KEY_E;
      if(digitalRead(K_E) == LOW)
        return KEY_E;
      if(digitalRead(K_SLEW) == LOW)
        return KEY_SLEW;
    } 
    return KEY_NONE;
}

void SendCommand(char *cmd)
{
  if(!g_client.connected())
  {
    DEBUG_PRINTLN("Client not connected:");
    digitalWrite(LEDGUIDE, LED_ON); 
    digitalWrite(LEDCENTER, LED_ON); 
    digitalWrite(LEDFIND, LED_ON); 
    g_client.connect(g_host, ONSTEP_PORT);
  }
  if(g_client.connected())
  {
    char buf[200];
    DEBUG_PRINTLN("Connected!");
    SetSlewLed();
    // Client handling code
    DEBUG_PRINTLN(cmd);
    g_client.println(cmd);
  }
#ifdef DEBUG
  else
    DEBUG_PRINTLN("no!");
#endif
}

void SetSlewLed()
{
  if(g_slewrate == SlewGuide)
  {
    digitalWrite(LEDGUIDE, LED_ON); 
    digitalWrite(LEDCENTER, LED_OFF); 
    digitalWrite(LEDFIND, LED_OFF); 
  }
  if(g_slewrate == SlewCenter)
  {
    digitalWrite(LEDGUIDE, LED_OFF); 
    digitalWrite(LEDCENTER, LED_ON); 
    digitalWrite(LEDFIND, LED_OFF); 
  }
  if(g_slewrate == SlewFind)
  {
    digitalWrite(LEDGUIDE, LED_OFF); 
    digitalWrite(LEDCENTER, LED_OFF); 
    digitalWrite(LEDFIND, LED_ON); 
  }
  if(g_slewrate == SlewMax)
  {
    digitalWrite(LEDGUIDE, LED_OFF); 
    digitalWrite(LEDCENTER, LED_OFF); 
    digitalWrite(LEDFIND, LED_OFF); 
  }
}


int SetSlewRate()
{
  if(g_slewrate == SlewGuide)
  {
    g_slewrate = SlewCenter;
  }
  else if(g_slewrate == SlewCenter)
  {
    g_slewrate = SlewFind;
  }
  else if(g_slewrate == SlewFind)
  {
    g_slewrate = SlewMax;
  }
  else if(g_slewrate == SlewMax)
  {
    g_slewrate = SlewGuide;
  }

  if(g_slewrate == SlewGuide)
  {
    DEBUG_PRINTLN("GUIDE");
    SendCommand(Guide);
  }
  if(g_slewrate == SlewCenter)
  {
    DEBUG_PRINTLN("CENTER");
    SendCommand(Center);
  }
  if(g_slewrate == SlewFind)
  {
    DEBUG_PRINTLN("FIND");
    SendCommand(Find);
  }
  if(g_slewrate == SlewMax)
  {
    DEBUG_PRINTLN("MAX");
    SendCommand(Max);
  }
  SetSlewLed();
  return g_slewrate;
}

void loop() 
{
  unsigned long currentMillis = millis();

  // Check for configuration portal startup
  if(digitalRead(K_N) == LOW && digitalRead(K_S) == LOW && digitalRead(K_SLEW) == LOW)
  {
      for(int fl = 0; fl < 5; fl++)
      {
         // Flash for user information
         digitalWrite(LEDGUIDE, LED_OFF); 
         digitalWrite(LEDCENTER, LED_OFF); 
         digitalWrite(LEDFIND, LED_OFF); 
         delay(300);
         // Turn led on.
         digitalWrite(LEDGUIDE, LED_ON); 
         digitalWrite(LEDCENTER, LED_ON); 
         digitalWrite(LEDFIND, LED_ON); 
         delay(300);
      }
      
      ESP_WiFiManager ESP_wifiManager("Target");
      ESP_WMParameter param_my_ip  ("IP Address", "IP Address", my_ip,   33);
      ESP_WMParameter param_my_gw  ("Gateway",    "Gateway",    my_gw,   33);
      ESP_WMParameter param_my_mask("Netmask",    "Netmask",    my_mask, 33);
      ESP_WMParameter param_my_host("Host",    "Host",    my_host, 33);
  
      //set config save notify callback
      ESP_wifiManager.setSaveConfigCallback(saveConfigCallback);
      ESP_wifiManager.addParameter(&param_my_ip);
      ESP_wifiManager.addParameter(&param_my_gw);
      ESP_wifiManager.addParameter(&param_my_mask);
      ESP_wifiManager.addParameter(&param_my_host);
  
      ESP_wifiManager.setMinimumSignalQuality(-1);
      //Check if there is stored WiFi router/password credentials.
      //If not found, device will remain in configuration mode until switched off via webserver.
      DEBUG_PRINTLN("Opening configuration portal. ");
      Router_SSID = ESP_wifiManager.WiFi_SSID();
      if (Router_SSID != "")
      {
        ESP_wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.
        DEBUG_PRINTLN("Got stored Credentials. Timeout 60s");
      }
      
      //it starts an access point
      //and goes into a blocking loop awaiting configuration
      ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password);

      strncpy(my_ip,    param_my_ip.getValue(),   sizeof(my_ip));
      strncpy(my_gw,    param_my_gw.getValue(),   sizeof(my_gw));
      strncpy(my_mask,  param_my_mask.getValue(),  sizeof(my_mask));
      strncpy(my_host,  param_my_host.getValue(),  sizeof(my_host));

      DEBUG_PRINTLN("saving config");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
     
      json["ip"]      = my_ip;
      json["gateway"] = my_gw;
      json["subnet"]  = my_mask;
      json["host"]  = my_host;
  
      File configFile = SPIFFS.open("/config.json", "w");
#ifdef DEBUG
      if (!configFile) {
        DEBUG_PRINTLN("failed to open config file for writing");
      }
#endif
      json.prettyPrintTo(Serial);
      json.printTo(configFile);
      configFile.close();
      //end save
 
      // Turn led off.
      digitalWrite(LEDGUIDE, LED_OFF); 
      digitalWrite(LEDCENTER, LED_OFF); 
      digitalWrite(LEDFIND, LED_OFF); 
    
  }
  if (currentMillis - g_previousMillis >= g_keyScanInterval)
  {
      g_previousMillis = currentMillis;
      int key = ScanKeyboard();
      
      if(key == KEY_N && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_N");
        // New key press for North
        SendCommand(MoveNorth);
        g_lastKey = key;
      }
      else if(key == KEY_S && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_S");
        // New key press for South
        SendCommand(MoveSouth);
        g_lastKey = key;
      }
      else if(key == KEY_W && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_W");
        // New key press for West
        SendCommand(MoveWest);
        g_lastKey = key;
      }
      else if(key == KEY_E && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_E");
        // New key press for East
        SendCommand(MoveEast);
        g_lastKey = key;
      }
      else if(key == KEY_SLEW && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_SLEW");
        // Handle slew rate 
        SetSlewRate();
        g_lastKey = key;
      }
      else if(key == KEY_F1IN && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_F1IN");
        // New key press for Focus1 IN
        SendCommand(Focus1In);
        g_lastKey = key;
      }
      else if(key == KEY_F1OUT && g_lastKey != key)
      {
        DEBUG_PRINTLN("KEY_F1OUT");
        // New key press for Focus1 OUT
        SendCommand(Focus1Out);
        g_lastKey = key;
      }
      else if(key == KEY_NONE && g_lastKey != KEY_NONE)
      {
        if(g_lastKey == KEY_N || g_lastKey == KEY_S || g_lastKey == KEY_W || g_lastKey == KEY_E)
        {
          DEBUG_PRINTLN("STOP");
          // A mount movement must be stopped
          SendCommand(MoveStop);
        }
        if(g_lastKey == KEY_F1IN || g_lastKey == KEY_F1OUT)
        {
          // A focuser movement must be stopped
          SendCommand(Focus1Stop);
        }
        g_lastKey = key;
      }
  }
  check_status();
}
