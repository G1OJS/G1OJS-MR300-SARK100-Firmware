---
layout: default
title: "Calibration"
guide: true
permalink: /Calibration/
---
# How to calibrate the device
<img src= 'https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/CalibrationPL259s640px.png'>

The procedure is very straightforward once you have the required calibration loads, which are:
  - open circuit (nothing connected)
  - 10 Ohm resistor
  - 50 Ohm resistor or dummy load
  - 220 Ohm resistor
  - 560 Ohm resistor

A good way of provding these is by soldering them inside spare PL259 or BNC connectors. The calibration routine is quite fast, and with the PL259 connectors you can just hold the inner in place - you don't have to screw on the outer if you don't want to. The messages on the LCD screen will guide you through the process. On completion, the unit automatically reboots.

The theory behind the calibration is described here: [Correcting measurement errors with calibration data](https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/CorrectingMeasurementErrors/), covering both the techniques used in EA4FRB's V13, and my V1.0.


