//AMG---------------------------------
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
#include <Array.h>
const byte size = 10;
int rawArray[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
Array<int> array = Array<int>(rawArray, size);
Array<float> arrayPixels = Array<float>(pixels, 64);

float maxTemp;


//ULTRASONIC----------------------------
const int pingPin = 13;
int inPin = 12;



void setup() {
  Serial.begin(9600);
  Serial.println(F("AMG88xx pixels"));

  bool status;

  // default settings
  status = amg.begin();
  if (!status) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1);
  }

  //Serial.println("-- Pixels Test --");

  Serial.println();

  delay(100); // let sensor boot up
}


void loop() {
  //AMG------------------------------------------------------------
  //read all the pixels
  amg.readPixels(pixels);

  float thermisterTemp = amg.readThermistor();

  //ULTRASOINC---------------------------------------------------
  long duration, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);

  cm = microsecondsToCentimeters(duration);

  //Serial.print(cm);
  //Serial.print("cm");
  Serial.println();
  delay(100);


  //MAIN FUNCTION-----------------------------------------

  if (cm >= 60 && cm <= 90) {
    float rFactor = (0.016 * cm) + 7.33;
    maxTemp = arrayPixels.getMax() + rFactor;
    if (maxTemp > 37.2) {
      maxTemp = maxTemp * 1.05;
    }
    
    if (maxTemp >= 37.5) {
      Serial.print("High");
      Serial.println();
    }

    else if (maxTemp <= 35.5) {
      Serial.print("Low ");
    }
  
    else {
      Serial.print(maxTemp, 1);
      Serial.println();
    }
  }
  else {
    Serial.print("Error=> sensor will not measure at: ");
    Serial.print(cm);
    Serial.println(" cm");
  }


  delay(1000);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
