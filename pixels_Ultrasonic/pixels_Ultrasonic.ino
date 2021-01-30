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
  /*
     for (int i = 1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++) {
       Serial.print(pixels[i - 1]);
       Serial.print(", ");
       if ( i % 8 == 0 ) Serial.println();
     }
     Serial.println();
  */


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

  /*
    maxTemp = maxTemp + 7.5;
    Serial.println(maxTemp);
  */


  if (cm >= 70) {
    Serial.print(arrayPixels.getMax());
    Serial.println();
    if (arrayPixels.getMax() >= 37.5) {
      Serial.print("High");
      Serial.println();
    }
  }
  else {
    Serial.print("Error: out of range at: ");
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
