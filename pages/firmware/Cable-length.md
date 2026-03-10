---
layout: default
title: "Cable length Measurement"
guide: true
permalink: /Cable-length/
--- 

<img src="https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/CableLengthScreen640px.png">
  
This feature of the firmware is not guaranteed to produce accurate results every time, but is an interesting use of the frequency scanning bridge and will undoubtedly be useful. 

It works by finding the frequencies at which the impedance seen at the input of an attached transmission line is at either a minimum or a maximum point. These minima and maxima occur regularly in frequency and by counting the number of them and recording the highest frequency at which one occurs, it is possible to get an accurate measurement of the frequency separation of these points. The longer the transmission line, the more minima and maxima there will be, and dividing the speed of the wave along the transmission line by that frequency period it's possible to calculate the length of the cable.

The graph below shows the bridge voltages Vz and Va against frequency for a sample of RG58 coax. By taking the min value of these voltages, which are proxies for mod(Z) and 1/mod(Z), we get a useful curve to count the minima of. Because we're taking the minimum of these two voltages, and these voltages simply swap places (approximately speaking) if we swap a short-circuited line for an open-circuit line, this counting method works whether the far end of the line is a short or open circuit.

To make use of this feature, first set the velocity factor of your transmission line via the Config menu, then connect your line and access the Cable Length scan again via the Config menu. To start the scan, press either the DOWN key or the SCAN key.

Once the scan is done, press the up key to exit the routine, or press the DOWN or SCAN key to scan again. Scans automatically repeat every 30 seconds whilst in this menu item.

![](https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/CableLengthExample640px.png)

