---
layout: default
title: "Successfully editing MR300/SARK100 Firmware on Windows 10"
permalink: /EditingOnWindows10/
---
# Successfully editing MR300/SARK100 Firmware on Windows 10
The full user manual for the SARK100 - available on [Melchor Varela (EA4FRB)'s website here](https://sites.google.com/view/sark100) - contains instructions on how to edit the firmware. Unfortunately, software environments have moved on a lot since those instructions were written and it is not easy to work out how to get things working. You'll need a copy of PSoC Designer V5.0 as mentioned in the manual, but also need to follow the instructions below.

Don't be tempted to install later versions of PSoC Designer than V5.0. For some reason although versions up to at least 5.4 will successfully build the code, the SARK100 will freeze or behave oddly once the firmware tries to write to the EEPROM (which happens very shortly after power on!). It *may* be possible to fix this, but I've not been able to after spending whole days on it and scouring the Infineon Community Forum.

It's important to get the SARK100 source code from Sourceforge rather than GitHub, because a crucial file is missing from the GitHub archives. The starting point I used was V13 from [SARK100 files at Sourceforge](https://sourceforge.net/projects/sark100swranaly/files/). The missing file is PSocConfig.xml which contains the "wiring diagram" that PSoC Designer needs plus a lot of necessary component parameters.

## How to install PSoC designer V5.0 under Windows 10
What worked for me ...
1. Note that the "make" routines in PSoC designer can't cope with paths that have a space in them *unless* "8dot3" file name creation is enabled in Windows (which provides a space-free version for PD to use). And, the install folder (which is below the one you can set in the installer) has a space in it! In my case, 8dot3 path names weren't enabled and I had to enable it and recreate the install locations (because otherwise they kept their previous path names). So, before you install anything!:

2. Run regedit and find the key HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem, and set the value of NtfsDisable8dot3NameCreation to 0

Then, in this order:
1. Install SVG Viewer (zip file link is [here on Infineon Community Forum](https://community.infineon.com/t5/PSoC-5-3-1/Adobe-SVG-viewer-not-available-and-PD-5-4-SP1-won-t-work/m-p/77237#M10104)) otherwise things don't display properly and if I remember correctly the PD hangs on a script error that can't be fixed.
2. Install [PSoCProgrammerSetup_3.24.2.b2254_RS](https://itools.infineon.com/archive/PSoCProgrammerSetup_3.24.2.b2254_RS.exe) (don't use a later version or PD will infinitely hang when launching on a couple of components).
3. Install [PSoC_Designer_5.0_Service_Pack_6](https://itools.infineon.com/archive/PSoC_Designer_5.0_Service_Pack_6.zip)

Once it's installed, launch PSoC Designer, use open Project/Workspace (not file) and find and open SARK100_SwrAnalyzer_BL.app

In order to program the SARK100, you'll need the Bootloader Host for the PC which can be found on [EA4FRB's website here](https://sites.google.com/view/sark100) (TerminalProgram_v2, right at the bottom of the page). There's a similar one at Infineon [AN2100_Bootloader_PSoC_1](https://www.infineon.com/dgdl/Infineon-AN2100_Bootloader_PSoC_1-ApplicationNotes-v08_00-EN.zip?fileId=8ac78c8c7cdc391c017d0724b214493b) (it looks like an application note document, but it's a zip file with lots of things inside).

## If flashing the firmware goes wrong ...
Twice I thought I'd broken it - here's how I fixed it.

If the flash fails and then the SARK100 won't work, you can't press the button to get the SARK to go into a state to accept any more uploads. Thankfully, a Power-On-Reset will do this, but you can't simply power cycle the SARK because that will break the USB connection and annoy the bootloader host app. So, open up the SARK's case and find JP3 (just to the right of the CY8C29466 chip). It doesn't have a header but for a one-off you need to briefly send the middle pin (which is connected to XRES) high. Vdd is nearby so that's easy with a bit of wire used to bridge the middle pin to the end one (the one with the square pad). If you're goingto be doing lots of updates, it might be worth installing a momentary pushbutton here!

Two circumstances lead to this happening. One, you've made a change to the firmware that causes the SARK to hang, in which case you have to fix the problem and reflash as above. Two, you have a dodgy USB connection. Fixing this might be as simple as changing the USB cable, or simply trying again and sitting very still! But you can also look at the relevant COM port settings in Device Manager under Ports(COM&LPT), and try reducing the baud rate and/or reducing the USB Transfer Sizes under Advanced Settings. I have mine set to 2048 and it all seems stable.
 

## If you can't obtain the files above ...

I've made a copy of as much as I can [here](https://github.com/G1OJS/SARK100-Firmware/tree/4fab97640a3294f12039db623c3936002f6dfa6a/PSoC%20Designer%20V5%20installation%20files) and [here](https://github.com/G1OJS/SARK100-Firmware/tree/4fab97640a3294f12039db623c3936002f6dfa6a/SARK100%20Firmware)

The only ones missing are the installers for PSoC Designer and PSoC Programmer, which are way too big to upload to GitHub.
