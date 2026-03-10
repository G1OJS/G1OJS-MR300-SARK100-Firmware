---
layout: default
title: "Power Meter"
guide: true
permalink: /Power-meter/
---

![](https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/PowerMeter640px.png)

This mode displays mvRMS and dBm power as measured at the input port. The same internal code is used to display output power in VFO mode.

- Due to memory limitations, the conversion between voltage and dBm is approximate. The voltage reading is derived directly from the internal ADC measurement.
- The calibration is approximate, based on readings at the low end of the frequency range (1 & 10MHz) on a single device. A future version may offer a calibration routine for the power meter.
- Use your own judgement when directly driving the unit to measure power. I would recommend keeping input power below that which the internal DDS is capable of producing, i.e. approx 3 to 4 V peak to peak (not peak or RMS!).

