#include <SPI85.h>
#include <RF24.h>
#include <BTLE.h>
#include "printf.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

volatile int f_wdt=1;

ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
}

// We set the CE pin to 7, which is bogus of course, we actually just 
// lock it HIGH to save a pin.
RF24 radio(7,4);
BTLE btle(&radio);

char NAME[9] = "foobar";

void setup() {

  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCR |= (1<<WDCE) | (1<<WDE);
  /* set new watchdog timeout prescaler value */
  WDTCR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  /* Enable the WD interrupt (note no reset). */
  WDTCR |= _BV(WDIE);
  
  Serial.begin(115200);
  printf_begin();
  
  Serial.println("BTLE advertisement receiver");

  btle.begin("");
  radio.printDetails();
  
  delay(500);
  
  Serial.println("Going into the listen loop...");
}

void loop() {
    
//  Serial.println("Listening... ");
  
  if (btle.listen()) {
    

    Serial.println("Got payload: ");
  
      /*  
    Serial.print(" Flags:");
    for (uint8_t i = 0; i < 3; i++) { 
      Serial.print(btle.buffer.payload[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print(" Name:");
    for (uint8_t i = 3; i < 11; i++) { 
      Serial.print(btle.buffer.payload[i],HEX);
      Serial.print(" ");
    }
    for (uint8_t i = 3; i < 11; i++) { 
      Serial.write(btle.buffer.payload[i]);
    }
    Serial.println();
    Serial.print(" Data:");
    for (uint8_t i = 11; i < (btle.buffer.pl_size)-6; i++) { 
      Serial.print(btle.buffer.payload[i],HEX);
     Serial.print(" ");
    }
    Serial.println();
    */
    if (is_my_name()) {
      Serial.println("My name was in the beacon!");
    }
  }
 
  Serial.println("entering sleep...");
  f_wdt = 0;
  enterSleep();
  
  btle.hopChannel();
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
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  /* Now enter sleep mode. */
  sleep_mode();
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  /* Re-enable the peripherals. */
  power_all_enable();
}
