//AMG---------------------------------
#include <Wire.h>
#include <Adafruit_AMG88xx.h>
Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

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

  //FINDING HIGH
  float maxTemp = 0;
  for (int j = 1; j <= AMG88xx_PIXEL_ARRAY_SIZE; j++) {
    float nowTemp = pixels[j];
    float previousTemp = pixels[j - 1];
    if (nowTemp >= previousTemp) {
      maxTemp = nowTemp;
    }
  }

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


  if (cm >= 70 && cm <= 90) {
    //CALIBRATE HERE
    //maxTemp = (6.509*pow(maxTemp,3))-(545.54*pow(maxTemp,2))+(15087*maxTemp)-137865;
    //maxTemp = (-2.3746*maxTemp)+104;
    maxTemp = maxTemp + 9.5;

    if (maxTemp >= 37.5) {
      Serial.println("High");
    }
    else {
      Serial.println(maxTemp);
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
