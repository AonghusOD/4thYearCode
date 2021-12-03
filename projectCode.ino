/* Aonghus O Domhnaill
 * Student ID: G00293306 
 * Project - Automate - Climate Monitor
 * Started code Week 10, used Arduino libraries to get base code for each sensor
 * Problems with PH module not giving correct reading
 * LDR,DHT22,CS811 - Are working as expected
 */

#include "Adafruit_CCS811.h"
#include "DHT.h"


///DHT 
#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

///PH 
const int phPin = 34;
float calibration_value = 21.34;
int voltage[10],temp;
float phValue = 0;
float phval;

//LDR CODE
//int sensorPin = 2; // select the input pin for LDR
/** Pin number for LDR analog input pin */
//int ldrPin = 35;
/** LDR light value = 0 if not updated */
long newLDRValue = 0;
#define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0)

////CCS811
Adafruit_CCS811 ccs;

int sensorValue = 0; // variable to store the value coming from the sensor
/////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Serial.println(("DHTxx test!"));
  Serial.println("CCS811 test");
//////////LDR ////////////////////
 // Initialize analog port for LDR
  //pinMode(ldrPin,INPUT);
 // adcAttachPin(ldrPin);
  //analogReadResolution(11);
  //analogSetAttenuation(ADC_6db);
  
  ///ph code///
  //pinMode(phValue, INPUT);
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  
  /////////PH Code//////////////////

  for(int i=0;i<10;i++) 
 { 
 voltage[i]=analogRead(phPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(voltage[i]>voltage[j])
 {
 temp=voltage[i];
 voltage[i]=voltage[j];
 voltage[j]=temp;
 }
 }
 }
 long int avgval=0;
 for(int i=2;i<8;i++)
 avgval+=voltage[i];

 phValue = (float)avgval * (5.0/1024/6);
 phval = 5.70 * phValue + calibration_value;
 Serial.print("phValue: ");
 Serial.println(phval);
  
  //LDR Code//
  //sensorValue = analogRead(sensorPin); // read the value from the sensor
  //Serial.println(sensorValue); //prints the values coming from the sensor on the screen

  // Read analog value of LDR
  //newLDRValue = analogRead(ldrPin);
 // Serial.println(newLDRValue);

  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  Serial.print("Analog Value = ");
  Serial.println(analogValue);   // the raw analog reading
  ///////////////////////////////////

  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
