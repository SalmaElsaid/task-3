#include <Timer.h>
Timer mytime;

const int bush_button = 2;
const int red_led = 3;
const int green_led = 4;
const int white_led = 5;
int value_bush = 0;
long int lastMillis = 0;
long previousMillis = 0;

void setup()
{ 
  pinMode(bush_button, INPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(white_led, OUTPUT);
  mytime.every(120000, flash_green); // do function flash_green every 2 minutes
  mytime.after(300000, flash_white);  // do function flash_white once after 5 minutes
  mytime.after(480000, flash_white);    // do function flash_white once after 8 minutes

}
void loop()
{
  int flag = 0;  // int to know if the buch_button get HIGH

  value_bush = digitalRead(bush_button);
  Serial.println(value_bush);

  if (value_bush == 1)
  {

    flag = 1;
    lastMillis = millis();
  }
  while (flag == 1)
  {
    mytime.update();
    if (millis() - lastMillis < 900000) {  // make the led on for 15 minutes
      digitalWrite(red_led, HIGH);

    }
    else

      digitalWrite(red_led, LOW);

  }


}


void flash_green()
{ unsigned long currentMillis = millis();

  while (millis() - currentMillis <= 5000)  // blink for 5 second
  { previousMillis = millis();

    while (millis() - previousMillis  <= 1000) { // led on for 1 sec

      digitalWrite(green_led, HIGH);
    }
    while (millis() - previousMillis  <= 2000) {  // led off for 1 sec

      digitalWrite(green_led, LOW);
    }
  }

}
void flash_white()
{ unsigned long currentMillis = millis();

  while (millis() - currentMillis <= 10000)  // blink for 10 sec
  {
    previousMillis = millis();
    while (millis() - previousMillis  <= 1000) { // led on for 1 sec

      digitalWrite(white_led, HIGH);
    }
    while (millis() - previousMillis  <= 2000) {  // led off for 1 sec

      digitalWrite(white_led, LOW);
    }
  }
}
