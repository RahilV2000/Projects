/**************************************************************************************************
In this project Multpile Sensors are interfaced with the Node MCU(ESP8266) 
and the output of the sensors are seen in BLYNK application
The Sensors Used here are as follows:
1)Proximity sensor :-  To detect or sense the approach or presence of nearby objects and for this it does not need physical contact
2)Light Sensor     :-  To detect the intensity of the light in the room .
3)Gas sensor       :-  To detect the various types of gases (i.e. in this case CO2 only is detected)
4)Motion Sensor    :- To detect the motion 
A multiplexer module is also Configured to switch between the deifferent sensors and get the output of all the sensors on 1 Analog pin(A0) only.

Author and created By : - Rahil Vahora

************************************************************************************************************/









#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


/********* Pin Definition ************/

//DHT11 for reading temperature and humidity value
#define DHTPIN            D7

//buzzer to use it for alert
#define buzzer            D0

// Here i have connceted all the sensors to nodeMCU using Multiplexer
//Selection pins for multiplexer module to switch between different sensors and give data on a single analog pin(A0).
#define S0                D3
#define S1                D4

//Analog pin to read the incoming analog value from different sensors.
#define analogpin         A0

int proximity;
int light;
int gas;
int motion;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Blynk_Token";

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301



DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "******";
char pass[] = "*****";
// This function sends MCU's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or for Fahrenheit pass the "true" arguement

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, proximity);
  Blynk.virtualWrite(V3, gas);
  Blynk.virtualWrite(V4, motion);
  Blynk.virtualWrite(V5, light);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  dht.begin();
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(buzzer, OUTPUT);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  // Now we can publish stuff!
  // Address 00
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  gas = analogRead(analogpin);
  Serial.print("Gas - "); Serial.println(gas);

// Address 10
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  motion = analogRead(analogpin);
  if (motion > 512)
  {
    motion = 1;
    
  }
  else
  {
    motion = 0;
    
  }
// Address 01
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
  proximity = analogRead(analogpin);
  Serial.print("Proximity - "); Serial.println(proximity);
  if (proximity < 512)
  {
    proximity = 1;
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    proximity = 0;
    digitalWrite(buzzer, LOW); 
  }
  


// Address 11
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  int raw_light = analogRead(analogpin);
  light = map(raw_light, 1024, 0, 0, 100);
  Serial.print("Light - "); Serial.println(light);

  Blynk.run();
  timer.run();
}
