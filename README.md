## Equail

## Description

This is AVR (Arduino) code for a device to take actions (HIGH on a pin)
when a BlueTooth Low Energy beacon is received.

![Equail](http://www.sjdquwf.org/birds86.gif)

## Explanation 

This is designed to interface with toys that run on batteries. The device uses
a NRF24L01+, set with a very specific destination address, on channels 37-39, 
which happen to overlap with some of the Bluetooth LE spec.

By configuring the radio to ignore stock (Nordic) crcs, and by implementing the 
BTLE CRC and whitening algorithms, we can interpret legitimate beacons and
respond to them.

## Prior Art

- Dmitry Grinberg's Bit Banging to [*send* a BTLE beacon](http://dmitry.gr/index.php?r=05.Projects&proj=15&proj=11.%20Bluetooth%20LE%20fakery)
- Florian Echtler's [BTLE library](https://github.com/floe/BTLE)
- Stanley Seow's [ATtiny85 port of the nrf library](https://github.com/stanleyseow/arduino-nrf24l01/)
- Nerd Ralph's hacks to run the nrf24 on [3 pins instead of 5](http://nerdralph.blogspot.ca/2014/01/nrf24l01-control-with-3-attiny85-pins.html) 

## Software Prerequisites

- [ATTiny Arduino Core](https://code.google.com/p/arduino-tiny/)
- Included Libraries installed
- Something to send a BTLE beacon (a linux computer)

## Hardware

- ATTiny85
- nrf24l01+
- An NPN transistor to switch on your device (2N3904)
- 3.3V Power supply (2X AA batteries)
- Some sort of ISP programmer

## Schematic

TODO:
I tried to make a schematic in Eagle and gave up :(
