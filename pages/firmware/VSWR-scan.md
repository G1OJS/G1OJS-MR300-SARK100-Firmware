---
layout: default
title: "VSWR Scanner"
guide: true
permalink: /VSWR-scan/
---
Scans the current band and shows, to the nearest 1kHz
  * Minimum SWR
  * Minimum SWR frequency
  * Lower limit frequency for SWR <2:1
  * Upper limit frequency for SWR <2:1

<img src='https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/assets/img/SWRScan640px.png' width=340px>

To activate the VSWR Scan, simply press the Scan button when in Impedance mode.

The scan routine uses an internally-set step to find the minimum VSWR in the current band segment, then refines this to a resolution of 1 kHz, and displays an impedance display at that frequency. Exit the scan mode by pressing the UP key or waiting 30 seconds. On exit, the frequency is set to the frequency of minimum VSWR found.

If the scan routine finds the minimum VSWR to be lower than a threshold value (currently set at 2:1 but may be a user option in future), a second step in the scan is activated. The scan searches down and then up, each time starting from the minimum SWR frequency, and finds the first frequencies at which the VSWR rises above the threshold. Again, each of these frequencies is refined to a resolution of 1 kHz. The two frequencies are then displayed as in the screen above. These two edge frequencies are allowed to be outside the starting band segment, and enable calculation of the SWR bandwidth.
