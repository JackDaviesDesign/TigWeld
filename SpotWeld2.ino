/*

Connects to a tig welder to weld copper bus bars to lithium ion batteries.
Supports triggering a pulse and controling the gas solenoid for weld shielding.

Written by Jack Davies - http://jackdavies.co
30/10/2023 - Denver CO

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeMonoBold12pt7b.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// INPUT: Potentiometer should be connected to 5V, GND and pin defined below.
int potPin = A0; // Potentiometer output connected to analog pin 3
int potVal = 0; // Variable to store the input from the potentiometer

const int triggerSwitch = 2; // Connected to the trigger on the torch
const int gasSolenoid = 3; // Connected to the SSR which triggers the gas solenoid
const int weldOutput = 5; // Connected to the welder switch input

const int gasPreflow = 1; // Hard coded gas preflow time
const int gasPostflow = 1; // Hard coded gas postflow time

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE, BLACK);
    pinMode(triggerSwitch, INPUT);
    pinMode(gasSolenoid, OUTPUT);
    pinMode(weldOutput, OUTPUT);
}

void loop() 
{
  potVal = analogRead(potPin);                // Reads the value of the potentiometer (value between 0 and 1023)
  potVal = map(potVal, 0, 1023, 500, 1);     // Scale it to use it get the right time (value between 1 and 500)

  display.clearDisplay();
  display.setFont();
  display.setCursor(14, 5);
  display.print("Batt Pulse Welder");

  display.setCursor(28, 55);
  display.print("Milliseconds");

  display.setFont(&FreeMonoBold12pt7b); // Sets larger font for the number display

// Move curser based on number of digits to ensure the value is centred
  if (potVal <10){ // Single digits
    display.setCursor(54, 40);
  }
  else if (potVal <100){ // Double digits
    display.setCursor(46, 40);
  }
  else{ // Tripple digits
    display.setCursor(40, 40);
  }
  display.print(potVal);

  display.display();

  if (digitalRead(triggerSwitch) == HIGH){ // Trigger on the button press
        digitalWrite(gasSolenoid, HIGH); // Start the gas flow
        delay(gasPreflow*1000); // Mulitply the seconds to get milliseconds
        digitalWrite(weldOutput, HIGH); // Start the welding pulse
        delay(potVal); // Duration of weld pulse
        digitalWrite(weldOutput, LOW); // Stop the weld pulse
        delay(gasPostflow*1000); // Mulitply the seconds to get milliseconds
        digitalWrite(gasSolenoid, LOW); // Stop the gas flow
    }
}