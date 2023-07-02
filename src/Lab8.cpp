/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/Users/ashleylibasci/IoTFolderLabs/Lab8/src/Lab8.ino"
#include "env.h"

#include <blynk.h>
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "Particle.h"

void setup();
void loop();
#line 9 "/Users/ashleylibasci/IoTFolderLabs/Lab8/src/Lab8.ino"
SYSTEM_THREAD(ENABLED);

VCNL4040 proximitySensor;
OledWingAdafruit display;

#define greenLED D7
#define yellowLED D6
#define blueLED D5

long startingProxValue = 0;
long deltaNeeded = 0;
boolean nothingThere = false;

bool isProximityThere = false;
bool isAmbientThere = false;

void setup()
{
  Blynk.begin(BLYNK_AUTH_TOKEN);

  Serial.begin(9600);
  Wire.begin();

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  display.setup();
  display.clearDisplay();
  display.display();

  proximitySensor.begin();             // Initialize the sensor
  proximitySensor.powerOffProximity(); // Power down the proximity portion of the sensor
  proximitySensor.powerOnAmbient();    // Power up the ambient sensor

  if (proximitySensor.begin() == false)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Device not found. Please check wiring.");
    display.display();
    while (1)
      ;
  }
}
void loop()
{
  Blynk.run();

  display.loop();

  unsigned int ambientValue = proximitySensor.getAmbient();
  unsigned int proxValue = proximitySensor.getProximity();

  if (display.pressedA())
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(proxValue);
    display.display();
    isProximityThere = true;
    isAmbientThere = false;
  }

  else if (display.pressedB())
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(ambientValue);
    display.display();
    isAmbientThere = true;
    isProximityThere = false;
  }

  else if (display.pressedC() && isProximityThere == true)
  {
    Blynk.virtualWrite(V0, proxValue);
    Serial.println(proxValue);
  }

  else if (display.pressedC() && isAmbientThere == true)
  {
    Blynk.virtualWrite(V0, ambientValue);
    Serial.println(ambientValue);
  }
  if (proxValue <= 5)
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(blueLED, LOW);
  }
  else if (proxValue >= 2000)
  {
    digitalWrite(blueLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  else
  {
    digitalWrite(yellowLED, HIGH);
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, LOW);
  }
}