## Equail

## Description

Audubon society plush toys modified to activate on BTLE beacons.

![Equail](http://www.sjdquwf.org/birds86.gif)

## DEMO

[![youtube video](http://img.youtube.com/vi/A8WPcTgCzXs/0.jpg)](http://www.youtube.com/watch?v=A8WPcTgCzXs)

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

See the `schematic_eagle` folder for my first attempt at a schematic ever.

## Server-side

I use OpenWrt on a [TP-Link WR703N](http://wiki.openwrt.org/toh/tp-link/tl-wr703n)
plus an inexpensive [Bluetooth 4.0 dongle](http://www.ebay.com/itm/171213530142?ssPageName=STRK:MEWNX:IT&_trksid=p3984.m1497.l2649).

Combine this with an HTTP interface on an xinetd script and you have an API!

See `openwrt_setup` for more details on this. Having a dedicate computer that 
can respond to requests to generate beacons is cool.

Once you have server, you can emulate the [Audubon Singing Bird Clock](http://marketplace.audubon.org/products/audubon-singing-bird-clock)!

````
0 7  * * * /usr/sbin/send_beacon CalQuail
0 8  * * * /usr/sbin/send_beacon CaliGull
0 9  * * * /usr/sbin/send_beacon NrthCard
0 10 * * * /usr/sbin/send_beacon PrplFnch
0 11 * * * /usr/sbin/send_beacon EastBlue
0 12 * * * /usr/sbin/send_beacon WestMead
0 13 * * * /usr/sbin/send_beacon CaliWren
0 14 * * * /usr/sbin/send_beacon WillowPt
0 15 * * * /usr/sbin/send_beacon BrnPelic
0 16 * * * /usr/sbin/send_beacon CactusWr
0 17 * * * /usr/sbin/send_beacon WestMead
0 18 * * * /usr/sbin/send_beacon HermitTh
````
