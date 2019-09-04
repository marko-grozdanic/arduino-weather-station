#include "Air_Quality_Sensor.h"
#include<stdlib.h> //dtostrf
#include <LiquidCrystal.h> //LED
#include <Seeed_BME280.h> //Barometer
#include <SI114X.h> //Light sensor

AirQualitySensor sensor(A0);
BME280 bme280;
SI114X SI1145 = SI114X();

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable(E), d4, d5, d6, d7) 
//Note; Don't use pins 0, 1 for RS, E since they're used for serial communication

void setup(void) {
  Serial.begin(9600);
  
  /*Device setup*/
  Serial.println("Initialising Air Quality Sensor!");
  while (!sensor.init()) {
    Serial.println("Air Quality Sensor is not ready!");
    delay(1000);
  }    
  Serial.println("Air Quality Sensor ready!");

  Serial.println("Initialising LCD Display!");  
  lcd.begin(16,2); // Initialises the LCD Display, passed parameters are dimensions 
  Serial.println("LCD Display ready!");

  Serial.println("Initialising Barometer(BME280).");  
  while (!bme280.init()) {
    Serial.println("BME280 device error!");
    delay(1000);
  }
  Serial.println("BME280 ready!");

  Serial.println("Initialising Sunlight Sensor(Si1145)!");
  while (!SI1145.Begin()) {
    Serial.println("Si1145 is not ready!");
    delay(1000);
  }  
  Serial.println("Si1145 ready!");
}

void loop(void) {
  //Store sensor readings
  float airQuality = sensor.getValue();
  float temperature = bme280.getTemperature(), pressure = bme280.getPressure(), altitude = bme280.calcAltitude(pressure), humidity = bme280.getHumidity();
  float visibleLight = SI1145.ReadVisible(), infraredLight= SI1145.ReadIR(), uvLight = (float)SI1145.ReadUV()/100;

  //Declare char arrays
  char temperatureString[16], pressureString[16], altitudeString[16], humidityString[8], visibleLightString[32], infraredLightString[32], uvLightString[16];

  //Convert all stored sensor readings into strings and append their units of measure
  //char *   dtostrf (double __val, signed char __width, unsigned char __prec, char *__s)
  dtostrf(temperature, 6, 2, temperatureString);
  strcat(temperatureString, " C");
  dtostrf(pressure, 6, 2, pressureString);
  strcat(pressureString, " Pa");
  dtostrf(altitude, 6, 2, altitudeString);
  strcat(altitudeString, " m");
  dtostrf(humidity, 6, 2, humidityString);
  strcat(humidityString, " %");
  dtostrf(visibleLight, 6, 2, visibleLightString);
  strcat(visibleLightString, " lux");
  dtostrf(infraredLight, 6, 2, infraredLightString);
  strcat(infraredLightString, " lux");
  dtostrf(uvLight, 6, 2, uvLightString);
  strcat(uvLightString, "");
   
  //---------------
  //Print values
  lcd.setCursor(0,0);
    lcd.print("Air quality:");
  lcd.setCursor(2,1);
    if (airQuality == AirQualitySensor::FORCE_SIGNAL) {
      lcd.print("High pollution! Force signal active.");
    }
    else if (airQuality == AirQualitySensor::HIGH_POLLUTION) {
      lcd.print("High pollution!");
    }
    else if (airQuality == AirQualitySensor::LOW_POLLUTION) {
      lcd.print("Low pollution!");
    }
    else  {
      lcd.print("Fresh air!");
    }
  delay(5000); 

  lcd.clear();  
  lcd.setCursor(0,0);
    lcd.print("Temperature:");
  lcd.setCursor(2,1);
    lcd.print(temperatureString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Air pressure:");
  lcd.setCursor(2,1);
    lcd.print(pressureString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Altitude:");
  lcd.setCursor(2,1);
    lcd.print(altitudeString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Humidity:");
  lcd.setCursor(2,1);
    lcd.print(humidityString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Visible light");
  lcd.setCursor(2,1);
    lcd.print(visibleLightString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("IR light");
  lcd.setCursor(2,1);
    lcd.print(infraredLightString);
  delay(5000); 

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("UV index:");
  lcd.setCursor(2,1);
    lcd.print(uvLight);
  delay(5000); 
  
  lcd.clear();  
}
