#include <SPI85.h>
#include <RF24.h>
#include <BTLE.h>
//#include "printf.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

//volatile int f_wdt=1;



ISR(WDT_vect)
{
  /*
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
  */
}


// We set the CE pin to 7, which is bogus of course, we actually just 
// lock it HIGH to save a pin.
RF24 radio(7,4);
BTLE btle(&radio);

char NAME[9] = "foobar";

void setup() {
  
  // Disabling the ADC saves us ~.300ma in sleep
  ADCSRA = 0;  
    
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCR |= (1<<WDCE) | (1<<WDE);
  /* set new watchdog timeout prescaler value */
  WDTCR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  /* Enable the WD interrupt (note the reset). */
  WDTCR |=  _BV(WDIE);
  
  Serial.begin(9600);
//  printf_begin();
  
  btle.begin("");
  radio.setPALevel(RF24_PA_MIN);
 // radio.printDetails();
  Serial.println();
  pinMode(3, OUTPUT);
}

void loop() {
  
  // The transmitter is on 3 channels, at 100ms apart.
  // That means we must be listening for 300ms, worst case
  // in order to recieve the beacon
  for ( int count = 0; count < 80; count++ ) {
    int ret = btle.listen(5);
    // We respond to beacons, even if they don't have a good CRC
    if (ret == 0 || ret == 1) {
     Serial.print("p");
     //Serial.println();
     if (ret == 0 ) {
       Serial.println("Got a Legit payload                                                              ");
     } else if (ret == 1) {
       Serial.println("Got a bad CRC payload                                                              ");
       // don't let bad payloads get us down, give us another try in the next loop
       count--;
     }
     // delay(50);
      if (is_my_name()) {
        Serial.print("********* Was in the beacon on hop ");
        Serial.print(count);
        Serial.println("******************************");
        activate_output();
        // If we have been activated, go right back to sleep after resetting the counter
        // No need to activate twice
        count = 0;
        enterSleep();
      }
    } else if (ret == 2) {
      // This indicates the radio had nothing available
      Serial.print(".");
    } else {
      // This should never happen. The code only returns 0,1,2
      Serial.println("?");
    }
  }
  enterSleep();
}

bool is_my_name() {
  // Function to determine if the destination section of the payload
  // matches the NAME const of this device
  for (uint8_t i = 3; i < 11; i++) { 
    if (btle.buffer.payload[i] != NAME[i-3]) {
      return false;
    }
  }
  return true;
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  Serial.println("Now sleeping");
  Serial.println();
  radio.powerDown();
  digitalWrite(3, LOW);
  sleep_enable();
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  Serial.println();
  Serial.println("Waking up from sleep");
  sleep_disable(); /* First thing to do is disable sleep. */
  /* Re-enable the peripherals. */
  power_all_enable();
  radio.powerUp();
}

void activate_output() {
  // This code toggles the output line to make the bird sing
  // LED light up, or whatever
  digitalWrite(3, HIGH);
  delay(100);
  digitalWrite(3, LOW);
  delay(100);
}
