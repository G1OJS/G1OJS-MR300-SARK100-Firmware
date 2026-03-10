DONE after current version on here
 - Refactored code & eliminated utoa calls saving about 700 bytes
 - Updated displays a little after finding right justified utoa is easier than left justified. e.g. |50+j0|=50 in main display, units after values in power readings not before.

TO DO
   - Add a guide page for L and C measurement
   - Dizzling during PClink scan
   - Look at dynamic range of Vz/Va error correction (high impedances >~1000R)
   - Something on applicability limits for cable measurements   

IDEAS - firmware
  - Convert spot freq measurement of L and C to scan and analysis
  - User choice of cal loads
  - User choice of cal frequencies
  - True ham band limits as well as HF dividers
      - Band mid freqs only purpose is to allow quick hopping up the spectrum via the band key. Doesn't have to be ham bands but it's convenient.
      - Band dividers only purpose is to stop VSWR scan taking forever (its initial search is constrained to current segment).
      - Arrow key freq change is also currently constrained to band segments, but this isn't vital.
      - Taking bands away would mean defining a max +/- search (could standardise at +/- 1MHz from current freq?). Adding true ham band edges would allow beeps when crossing boundaries but little else.
  - User choice of VSWR threshold
  - Investigate cal which includes reactive loads - does it help?
  - Make use of sine fitting to Va and Vz for cable measurement? Can we measure cable loss?
  - 

Bigger Ideas
 - Convert to WSPR beacon or FT8 transceiver or beacon
 - Convert to station peak and average power monitor
