#include <SPI85.h>
#include <RF24.h>
#include <BTLE.h>
//#include "printf.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

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

char NAME[9] = "MEDOLARK";

void setup() {

  // Disabling the ADC saves us ~.300ma in sleep
  ADCSRA = 0;  

  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  wdt_enable(WDTO_2S); 

  // Serial.begin(9600);
  // Serial.println("Powering on...");
  //  printf_begin();

  btle.begin("");
  radio.setPALevel(RF24_PA_MIN);
  // radio.printDetails();
  //Serial.println();
  //pinMode(3, OUTPUT);
}

void loop() {

  // The transmitter is on 3 channels, at 100ms apart.
  // That means we must be listening for 300ms, worst case
  // in order to recieve the beacon
  //flash();
  for ( int count = 0; count < 25; count++ ) {
    int ret = btle.listen(5);
    // We respond to beacons, even if they don't have a good CRC
    if (ret == 0 || ret == 1) {
      Serial.println();
      if (ret == 0 ) {
        Serial.print("VALID payload for: ");
      } 
      else if (ret == 1) {
        Serial.print("BAD CRC payload for: ");
        // don't let bad payloads get us down, give us another try in the next loop
        //count = 0;
      }
      print_destination_addr();
      Serial.println(" !");
      int score = name_closeness_score();
      Serial.print("Name score: ");
      Serial.println(score);
      if ( score >= 6) {
        wdt_reset();
        Serial.print("********* Was in the beacon on hop ");
        Serial.print(count);
        Serial.println("******************************");
        activate_output();
        // If we have been activated, go right back to sleep after resetting the counter
        // No need to activate twice
        count = 0;
        wait_it_out();
        enterSleep();
      } 
      else {
        Serial.println();
        Serial.print("Wasn't for me. Was for: ");
        print_destination_addr();
        Serial.println();
      }
    } 
    else if (ret == 2) {
      // This indicates the radio had nothing available
      Serial.print(".");
    } 
    else {
      // This should never happen. The code only returns 0,1,2
      Serial.println("?");
    }
    btle.hopChannel();
  }
  //flash();
  enterSleep();
}


int name_closeness_score() {
  int name_score = 0;
  for (uint8_t i = 3; i < 11; i++) { 
    if (btle.buffer.payload[i] == NAME[i-3]) {
      name_score++;
    }
  }
  return name_score;
}


void print_destination_addr() {
  for (uint8_t i = 3; i < 11; i++) { 
    Serial.print(btle.buffer.payload[i]);
  }
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  Serial.println("Now sleeping");
  // Serial.println();
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
  wdt_reset();
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); 
  delay(1000);
  pinMode(3, INPUT);
  wdt_reset();
}

void flash() {
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); 
  delay(10);
  pinMode(3, INPUT);
  delay(10);
}

void wait_it_out(){
  for (int c = 0 ; c < 10; c++) {
    wdt_reset();
    delay(1000);
  }
}


