#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif


/*
* Define values to GPIO esp8266 to reference digital pins  
*
*/
# define D0 16
# define D1 5
# define D2 4
# define D3 0
# define D4 2  // lógica invertida
# define D5 14
# define D6 12
# define D7 13
# define D8 15
# define TX 1 // Envia MSG
# define RX 3 // Rebebe msg

int val_digital = 0;


//response command
String operating_normally   = "00";
String with_problem         = "1F";
String analog_entry_measure = "01";
String digital_input_status = "02";


//String comand = "";
const char* ssid = STASSID;
const char* password = STAPSK;

// Nome do ESP na rede
const char* host = "ESP-10.0.0.109";

// Definições de rede
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);


void setup() {
  Serial.begin(115200);
  //Serial.println("Booting");

  // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // Setup project
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);


  // limpa buff
  if(Serial.available() >0 ){
    Serial.readString();
  }

  for(int i =0;i  < 5; i++){
   digitalWrite(LED_BUILTIN, LOW);
     delay(50);                  // Wait for a second
      digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
      delay(50);      
   }
 
}

void loop() {
  ArduinoOTA.handle();

  pinMode(LED_BUILTIN,OUTPUT);

  if(Serial.available()>0){
      String command = Serial.readString();
      command.trim();
   
    if(command[0] == '3' && command[1]=='0'){ // NodeMCU situation
       Serial.print(operating_normally);
    }
   
    else if(command[0] =='4' && command[1]=='0'){ // return analogic input value
        Serial.print(analog_entry_measure);
        Serial.print(analogRead(A0));
    }
   
    else if(command[0] == '5'){
        char address = command[1];  //read sensor address
       
        switch (address) {
          case '1': // input Number one reference pin D0
            val_digital = digitalRead(D0);
            Serial.print(digital_input_status);
            Serial.print(val_digital);                            
            break;
           
          case '2': // input number two one reference pin D1
            val_digital = digitalRead(D1);
            Serial.print(digital_input_status);
            Serial.print(val_digital);  
            break;
           
          case '3' : // input number three on reference pin D2
            val_digital = digitalRead(D2);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;
           
          case '4': // input number four on reference pin D3
            val_digital = digitalRead(D3);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;
           
          case '5': // input number five on reference pin D4
            val_digital = digitalRead(D4);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;

          case '6': // input number six on reference pin D5
            val_digital = digitalRead(D5);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;
           
          case '7': // input number six on reference pin D6
            val_digital=digitalRead(D6);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;
          case '8': // input number seven on reference pin D7
            val_digital = digitalRead(D7);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;

          case '9': // input number eight on reference pin D8
            val_digital = digitalRead(D8);
            Serial.print(digital_input_status);
            Serial.print(val_digital);
            break;                          
          }      
    }
   
    else if(command[0] =='6'){          // LED blinks NodeMCU
         digitalWrite(LED_BUILTIN,LOW);
         Serial.print(operating_normally);
    }

    else if(command[0] =='7'){           // LED blinks NodeMCU
         digitalWrite(LED_BUILTIN,HIGH);
         Serial.print(operating_normally);
    }
   
    else{
      Serial.print(with_problem);
    }
   
  }
 
}