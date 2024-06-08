# FancyLight

FancyLight - an LED lamp powered by a Li-Ion battery, which can be found in used electronic cigarettes. It charges via a micro USB connector and is turned on and off using a touch button. A simple project to learn how to work with the TP4056 battery charging controller, a step-up DC-DC converter based on the SB6286 for powering LEDs, a touch switch controller based on the TTP223-BA6, as well as battery protection elements based on the DW01-P and dual MOSFET 8205A.

A touch switch controller is assembled on a separate PCB to improve touch sensing reliability.

## TODO

* Fix schematic and PCB:
    * Remap MCU pins to use benefits of builtin PWM generator
    * Fix auto-on on charging issue: add LDO for sensor and MCU, decoupling capacitors
    * Replace MURATA coil with some more common analog
    * Rework ISP connector so it compatible with programming clip, place it near the edge
    * Make 4 layers stackup
    * Update BOM and placement files to automatically mount
        * USB-C connector
        * LEDs
        * Inductor for Step-Up converter
    * Replace resistor on SENSE pin to 10k - 100k
    * Design mounting hole for PCB itself
* Fix enclosure
    * Design mounting for the PCB for easier assembly
    * Fix the depth of mode selection pocket
* (Minor) Update schematic and PCB
    * Introduce nRF module for BLE
    * Add wireless charger receiver as an alternative to USB-C
