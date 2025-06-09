#include <Adafruit_NeoPixel.h>
#include <DHT20.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define ANALOG_INPUT_PIN 1

#define WLAN_SSID "RD-SEAI_2.4G"
#define WLAN_PASS ""

#define AIO_USERNAME  "ThanhTaiNguyen"
#define AIO_KEY       "aio_FnBY39mbIHgeBhtmIjB2wfHLTWbk"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
//Cambiennhietdo
Adafruit_MQTT_Publish cambiennhietdo = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DADNcambiennhietdo");

//Cambiendoam
Adafruit_MQTT_Publish cambiendoam = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DADNcambiendoam");

//Cambienanhsang
Adafruit_MQTT_Publish cambienanhsang = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DADNcambienanhsang");

//Biensoxe
Adafruit_MQTT_Subscribe biensoxe = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/DADNbiensoxe");

//Battatden
Adafruit_MQTT_Subscribe battatden = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/DADNbattatden");

//Battatquat
Adafruit_MQTT_Subscribe battatquat = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/DADNbattatquat");







//Defind task
void TaskTemperatureHumidity(void *pvParameters);
void TaskLightAndLED(void *pvParameters);
void TaskReceiveMessageandControlDevice(void *pvParameters);
// void TaskControlFan(void *pvParameters);
// void TaskControlLight(void *pvParameters);


//Define your components here
Adafruit_NeoPixel pixels3(4, P14, NEO_GRB + NEO_KHZ800);
DHT20 dht20;
LiquidCrystal_I2C lcd(0x21,16,2);


void MQTT_Connect() {
  int8_t ret;
  
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.println();

  Serial.println("Connecting to MQTT...");
  
  // connect will return 0 for connected.
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 10 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds.
  }
  
  Serial.println("MQTT Connected.");
  Serial.println("-------------");
}


// The setup function runs once when you press reset or power on the board.
void setup() {

  // Initialize serial communication at 115200 bits per second:
  

  dht20.begin();
  lcd.begin();
  pixels3.begin();
  Serial.begin(115200);
  if (mqtt.subscribe(&biensoxe)){
    Serial.println(F("Subcribed Car Plate successfully!!"));
  }
  if (mqtt.subscribe(&battatden)){
    Serial.println(F("Subcribed LED successfully!!"));
  }
  if (mqtt.subscribe(&battatquat)){
    Serial.println(F("Subcribed FAN successfully!!"));
  }


WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



  xTaskCreate( TaskTemperatureHumidity, "Task Temperature" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskLightAndLED, "Task Light LED" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskReceiveMessageandControlDevice, "Receive Msg", 2048, NULL, 2, NULL);
  // xTaskCreate( TaskControlFan, "Control Fan", 2048, NULL, 2, NULL);
  // xTaskCreate( TaskControlLight, "Control Light", 2048, NULL, 2, NULL);

}

void loop() {
  MQTT_Connect();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void TaskTemperatureHumidity(void *pvParameters) { 

  while(1) {                          
    dht20.read();
    float Temperature = dht20.getTemperature();
    float Humidity = dht20.getHumidity();
    Serial.println(F("Temperature"));
    Serial.println(Temperature);
    Serial.println(F("Humidity"));
    Serial.println(Humidity);
    if (cambiennhietdo.publish(Temperature)){
      Serial.println(F("Published Temperature Sensor successfully!!"));
    }
    if (cambiendoam.publish(Humidity)){
      Serial.println(F("Published Humidity Sensor successfully!!"));
    }
    delay(30000);
  }
}


void TaskLightAndLED(void *pvParameters) { 

  while(1) { 
    uint32_t light = analogRead(P0_ADC); 
    Serial.println(F("Light"));
    Serial.println(light);

    if (cambienanhsang.publish(light)){
      Serial.println(F("Published Light Sensor successfully!!"));
    }
    delay(25000);
  }
}

void TaskReceiveMessageandControlDevice(void *pvParameters) {
  while (1) {
    Adafruit_MQTT_Subscribe *subscription = mqtt.readSubscription(5000); 

    if (subscription == &biensoxe) {
      char *message = (char *)biensoxe.lastread;
      Serial.print(F("Received message from Adafruit: "));
      Serial.println(message);

      char *status = strtok(message, ":"); 
      char *plate = strtok(NULL, ":");

      if (plate != NULL) {
        while (*plate == ' ') plate++;
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(status);
      lcd.setCursor(0, 1);
      lcd.print(plate);  
      Serial.println(F("Done Cycle!!"));
    }
    if (subscription == &battatden) {
      char *message = (char *)battatden.lastread;
      Serial.print(F("LED Toggle Feed: "));
      Serial.println(message);
      
      if (strcmp(message, "ON") == 0) {
        pixels3.setPixelColor(0, pixels3.Color(0,0,255));
        pixels3.setPixelColor(1, pixels3.Color(0,255,0));
        pixels3.setPixelColor(2, pixels3.Color(255,0,0));
        pixels3.setPixelColor(3, pixels3.Color(128, 0, 128));
        pixels3.show();
        Serial.println(F("LED Turned ON"));
      } else if (strcmp(message, "OFF") == 0) {
        pixels3.setPixelColor(0, pixels3.Color(0,0,0));
        pixels3.setPixelColor(1, pixels3.Color(0,0,0));
        pixels3.setPixelColor(2, pixels3.Color(0,0,0));
        pixels3.setPixelColor(3, pixels3.Color(0,0,0));
        pixels3.show();
        Serial.println(F("LED Turned OFF"));
      }
    }
    if (subscription == &battatquat) {
      char *message = (char *)battatquat.lastread;
      Serial.print(F("Fan Toggle Feed: "));
      Serial.println(message);
      
      if (strcmp(message, "ON") == 0) {
        analogWrite(P1_ADC, 250);
        Serial.println(F("Fan Turned ON"));
      } else if (strcmp(message, "OFF") == 0) {
        analogWrite(P1_ADC, 0);
        Serial.println(F("Fan Turned OFF"));
      }
    }
    delay(100); 
  }
}

