# SARK100 Bridge Analysis

This folder contains Python scripts and data for analyzing voltage measurements from an RF bridge.

## ID format in CSV files
Format: `Type_R[_L_C]_Source_Processing`
- Units: Ohms (R), uH (L), pF (C)
- Type: R (resistor), RLC (RLC load), X (non-standard load)
- Example: `RLC_100_4.7_22_MR300_avg` → R=100Ω, L=4.7uH, C=22pF, source=MR300, processed=averaged
