---
layout: default
mathjax: true
title: "Equations compendium"
permalink: /Equations/
---

## Port Capacitance Correction in |Z|-|Γ| Space

### Corrected |Z|
\[
|Z_{\text{true}}| \approx |Z_{\text{meas}}| + \omega C_p |Z_{\text{meas}}|^2 \sin\theta
\]

### Corrected |\Gamma|
\[
|\Gamma_{\text{true}}| \approx |\Gamma_{\text{meas}}| + \frac{2Z_0}{(|Z_{\text{meas}}| + Z_0)^2} \cdot \omega C_p |Z_{\text{meas}}|^2 \sin\theta
\]

### Notes
- These equations correct for port capacitance directly in **magnitude space** without needing (R, X).
- \(\theta\) is the phase of measured impedance \( Z_{\text{meas}} \).
- This avoids invalid (R, X) points before correction.
