---
layout: default
mathjax: true
title: "Converting Bridge Voltages to Load Impedance: Method 1"
permalink: /BridgeVoltagesToImpedance1/
---

The equations for the bridge voltages that can be measured (see [Output Voltages on RF Wheatstone Bridges](https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/BridgeVoltages/) ) are:

  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>Eq1</td><td style='border:none;'>$$V_a=2V_f\frac{50}{\vert Z_l+50\vert }$$</td></tr></table>
  </div>
  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>Eq2</td><td style='border:none;'>$$V_z=2V_f\frac{\vert Z_l\vert }{\vert Z_l+50\vert }$$</td></tr></table>
  </div>
  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>Eq3</td><td style='border:none;'>$$V_r = \vert  2V_f \frac{Z_l}{Z_l+50} - V_f\vert $$</td></tr></table>
  </div>
  <div  style='clear:both'></div>


## Step 1 - three voltages to two voltage ratios
<p>Looking at the equations for $V_z$ and $V_a$, we can see that they have the same denominator, and both share the multiplier $2V_f$ , so dividing one equation by the other will get rid of these quantities and leave us with $\frac{V_z}{V_a}=\frac{\vert Z_l\vert }{50}$which gives us the magnitude of the unknown impedance. </p>

<p>To get the complex impedance, we need an equation that contains it directly rather than inside \vert mod\vert bars. 
  If we look at the equation for $V_r$ above, we can rearrange to get a single fraction as follows:</p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\frac{V_r}{V_f} = \vert  2 \frac{Z_l}{Z_l+50} - 1 \vert  = \vert  \frac{2Z_l - (Z_l+50)}{Z_l+50}\vert  = \vert \frac{Z_l-50}{Z_l+50}\vert  $$</td></tr></table>
</div>
<div  style='clear:both'></div>

<p>So we have:</p>
<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>Eq4</td><td style='border:none;'>$$\frac{V_r}{V_f}=\vert \frac{Z_l-50}{Z_l+50}\vert $$</td></tr></table>
</div>
<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>Eq5</td><td style='border:none;'>$$\frac{V_z}{V_a}=\frac{\vert Z_l\vert }{50} $$</td></tr></table>
</div>
<div  style='clear:both'></div>


## Step 2 - Calculating the Load Impedance
  <p>Note that the RHS of Eq4 is the magnitude of the reflection coefficient, or $\rho$ </p>

  <p>So Eq7 from the explanation above (expand it if it's closed) is </p>
  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>$$\frac{(R-50)^2+X^2}{(R+50)^2+X^2}=\frac{\vert Z_l\vert ^2-100R+2500}{\vert Z_l\vert ^2+100R+2500}=\rho^2$$</td></tr></table>
  </div>
  <div  style='clear:both'></div>

  <p> $\rho$ is related to VSWR by $\rho=\frac{VSWR-1}{VSWR+1}$ so $\rho^2$ is </p>
   
  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>$$\frac{(VSWR^2+1)-2VSWR}{(VSWR^2+1)+2VSWR} = \frac{(R-50)^2+X^2}{(R+50)^2+X^2}=\frac{\vert Z_l\vert ^2-100R+2500}{\vert Z_l\vert ^2+100R+2500}$$</td></tr></table>
  </div>
  <div  style='clear:both'></div>

  <p>Which can be rearranged to give</p>

  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>$$R = (2500+\vert Z_l\vert ^2)\frac{VSWR}{50(VSWR^2+1)}$$</td></tr></table>
  </div>
  <div  style='clear:both'></div>

  <p>VSWR is given by $\frac{1+\rho}{1-\rho}$ which can be combined with Eq4 to give</p>

  <div class='equation'>
    <table style='border:none;'><tr><td style='border:none;'>$$VSWR = \frac{V_f+V_r}{V_f-V_r}$$</td></tr></table>
  </div>
  <div  style='clear:both'></div>

  <p> So, with an interim step of calculating VSWR, we have:</p>

</details>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$R = (2500+\vert Z_l\vert ^2)\frac{VSWR}{50(VSWR^2+1)}$$</td></tr></table>
</div>
<div  style='clear:both'></div>

<p>where</p> 

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$VSWR = \frac{V_f+V_r}{V_f-V_r}$$</td></tr></table>
</div>
<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$X = \pm\sqrt{(50\frac{V_z}{V_a})^2 - R^2}$$</td></tr></table>
</div>
<div  style='clear:both'></div>

