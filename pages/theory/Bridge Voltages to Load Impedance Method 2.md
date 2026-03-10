---
layout: default
mathjax: true
title: "Converting Bridge Voltages to Load Impedance: Method 2"
permalink: /BridgeVoltagesToImpedance2/
---
There is an alternative way of doing Step 2 as described in  [Converting Bridge Voltages to Load Impedance: Method 1](https://g1ojs.github.io/G1OJS-MR300-SARK100-Firmware/BridgeVoltagesToImpedance1) 

<p>The RHS of Eq4 with $Z_l = R+jX$ is </p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\vert \frac{R-50+jX}{R+50+jX}\vert $$</td></tr></table>
</div>
<div  style='clear:both'></div>

<p>Squaring the modulus $\vert a+jb\vert $ gives $a^2+b^2$, so squaring top and bottom of the equation above gives</p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>Eq6</td><td style='border:none;'>$$\frac{(R-50)^2+X^2}{(R+50)^2+X^2}=\frac{R^2-100R+2500+X^2}{R^2+100R+2500+X^2}$$</td></tr></table>
</div>
<div  style='clear:both'></div>

<p>But $R^2+X^2 = \vert Z_l\vert ^2$ so </p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>Eq7</td><td style='border:none;'>$$\frac{(R-50)^2+X^2}{(R+50)^2+X^2}=\frac{\vert Z_l\vert ^2-100R+2500}{\vert Z_l\vert ^2+100R+2500}$$</td></tr></table>
</div>
<div  style='clear:both'></div>


<p>Which can be solved to give</p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$R = \frac{\vert Z_l\vert ^2+50^2}{100}\frac{1-(\frac{V_r}{V_f})^2}{1+(\frac{V_r}{V_f})^2}$$</td></tr></table>
</div>
<div  style='clear:both'></div>

<p>Now that we know $R$ as well as $\vert Z_l\vert $, we can use $ X=\sqrt{\vert Z_l\vert ^2-R^2} $ to get the complex impedance and VSWR.</p>

<p>We can rewrite these equations to use the bridge voltages directly:</p>

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$R = \frac{(50\frac{V_z}{V_a})^2+50^2}{100}  \frac{1-(\frac{V_r}{V_f})^2}{1+(\frac{V_r}{V_f})^2}$$</td></tr></table>
</div>
<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$X = \pm\sqrt{(50\frac{V_z}{V_a})^2 - R^2}$$</td></tr></table>
</div>
<div  style='clear:both'></div>

