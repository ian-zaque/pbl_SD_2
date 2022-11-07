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
int operating_normally = 0x00;
int  with_problem= 0x1F;
int analog_entry_measure= 0x01;
int digital_input_status=0x02;


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
  Serial.println("Booting");

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

   Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  //  Defined input pins
  pinMode(D0,INPUT );
  pinMode(D1,INPUT );
  pinMode(D2,INPUT );
  pinMode(D3,INPUT );
  pinMode(D4,INPUT );
  pinMode(D5,INPUT );
  pinMode(D6,INPUT );
  pinMode(D7,INPUT );
  pinMode(D8,INPUT );
  
  pinMode(A0,INPUT);

  
}

void loop() {
  ArduinoOTA.handle();


  if(Serial.available()>0){
  
    int command = Serial.read();
    Serial.println(command);   
    switch (command) {
      
      case 0x03: // situation NodeMCU
        Serial.write(operating_normally);   
        break;
      case 0x04 : // return analogic input value
        {
          
        int val_analog = analogRead(A0);
        Serial.write(analog_entry_measure);
        Serial.write(val_analog);
        }
        //analogWrite(TX,val_analog);  
        break;
      case 0x05: // return digital input value
      {
      Serial.write("Ola");
         int address = Serial.read();  //read sensor address
         
            switch (address) {
              case 49: // input Number one reference pin D0
                val_digital = digitalRead(D0); 
                Serial.write(digital_input_status);
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital);                            
                break;
              case 50: // input number two one reference pin D1
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D1); 
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital);    
                break;
              case 51 : // input number three on reference pin D2
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D2);
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital)
                break;
              case 52: // input number four on reference pin D3
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D3);
                Serial.write(val_digital);
                //digitalWrite(TX,val_digital);
                break;
              case 53: // input number five on reference pin D4
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D4);
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital);
                break;

              case 54: // input number six on reference pin D5
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D5);
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital);
                break;
              case 56: // input number six on reference pin D6
                Serial.write(digital_input_status);
                 val_digital=digitalRead(D6);
                Serial.write(val_digital);
                //digitalWrite(TX, val_digital);
                break; 
              case 57: // input number seven on reference pin D7
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D7);
                Serial.write(val_digital);
                //digitalWrite(TX,val_digital);
                break;

              case 58: // input number eight on reference pin D8
                Serial.write(digital_input_status);
                 val_digital = digitalRead(D8);
                Serial.write(val_digital);
                //digitalWrite(TX,val_digital);
                break;   
              default:

                 Serial.write(with_problem);
                //digitalWrite(TX,with_problem);
                break;                           
              }       
          }  
        break;
      case 0x06: // LED blinks NodeMCU
         digitalWrite(LED_BUILTIN,HIGH);
         break;
      default: 
        Serial.write(with_problem);
        // digitalWrite(TX,with_problem);
        break;                   
        
    }
  
    
  } 

 
  
}
