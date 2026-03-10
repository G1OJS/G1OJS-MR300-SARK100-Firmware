---
layout: default
title: "VFO Mode"
guide: true
permalink: /VFO-mode/
---

<img src='https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/VFOMode640.png'>

The VFO mode shown above is intended to produce a continuous output free from interference by periodic measurements. It is accessed via the main 'mode' menu key.

To change frequency, use arrow keys as normal (using the 'select increment' feature if necessary to change the cursor position to increment a different increment digit).

To change output power level, toggle into VFO power setting mode by pressing the Config key once whilst in VFO mode. A cursor will appear next to the dBm reading as shown below to confirm this mode. Then use the arrow keys to select power. Once the desired power is set, exit power setting mode by pressing the Config key again. Note that the Config menu cannot be accessed from the VFO mode because that key is used to enter and exit power setting mode. Simply leave VFO mode by pressing the Mode key if the Config mode is needed.

<img src='https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/VFOPowerSettingMode640.png'>

The range of output powers available is quite limited due to both memory and hardware limitations, but somewhere between -15 and +12 dBm is possible depending on load conditions.

If any kind of measurement is made after setting the VFO power as above, the new output power will remain in use until the next power cycle. 

**Caution: on lower frequency bands, the output power may be sufficient to cause waveform distortion; in these cicumstances, do not connect the VFO directly or indirectly to an antenna, and exercise caution in interpreting measurement results.**


