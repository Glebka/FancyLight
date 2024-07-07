# FancyLight

FancyLight - an LED lamp powered by a Li-Ion battery, which can be found in used electronic cigarettes. It charges via a micro USB connector and is turned on and off using a touch button. A simple project to learn how to work with the TP4056 battery charging controller, a step-up DC-DC converter based on the SB6286 for powering LEDs, a touch switch controller based on the TTP223-BA6, as well as battery protection elements based on the DW01-P and dual MOSFET 8205A.

A touch switch controller is assembled on a separate PCB to improve touch sensing reliability.

## TODO

* Fix schematic and PCB:
    * Connect Q1's (SENSE) gate to GND via resistor
* Fix enclosure
    * Design mounting for the PCB for easier assembly
    * Fix the depth of mode selection pocket
    * Adjust heights because of PCB thickness increase
* Fix firmware
    * Adjust pin mappings
* (Minor) Update schematic and PCB
    * Introduce nRF module for BLE
    * Add wireless charger receiver as an alternative to USB-C
